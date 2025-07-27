#include <iostream>
#include <cstdint>

#include <SDL3_ttf/SDL_ttf.h>
#include <harfbuzz/hb.h>
#include <json/json.h>

#include "sdl/window.hh"
#include "arg_parser.hh"
#include "config.hh"
#include "utils.hh"
#include "log.hh"
#include "sdl.hh"

#define NEW_SHARED()


namespace
{
    void
    print_help( std::ostream &p_stream, std::string_view p_bin )
    {
        std::string top { std::format("Usage: {} <options>", p_bin) };
        std::println(p_stream, "{}", top);
        std::println(p_stream, "{}\n", std::string(top.length(), '-'));
        std::println(p_stream,
            "  -h --help                   Show this message.\n"
            "  -V --version                Show version info.\n"
            "\n"
            "  -c --config     <file>      Define a custom config path.            \n"
            "  -l --log        <int,dir>   Set log level and or log file. (warn/2) \n"
        );
    }


    void
    print_version( std::ostream &p_stream )
    {
        std::string top { std::format("{} {}", APP_NAME, APP_VERSION) };
        std::println(p_stream, "{}", top);
        std::println(p_stream, "{}\n", std::string(top.length(), '-'));
        std::println(p_stream, "  SDL      {}.{}.{}", SDL_MAJOR_VERSION,
                                                      SDL_MINOR_VERSION,
                                                      SDL_MICRO_VERSION);
        std::println(p_stream, "  SDL_TTF  {}.{}.{}", SDL_TTF_MAJOR_VERSION,
                                                      SDL_TTF_MINOR_VERSION,
                                                      SDL_TTF_MICRO_VERSION);
        std::println(p_stream, "  HarfBuzz {}", HB_VERSION_STRING);
        std::println(p_stream, "  JsonCpp  {}", JSONCPP_VERSION_STRING);
    }


    auto
    get_args( int32_t &p_argc, char **&p_argv )
    {
        const std::string HOME { utils::getenv("HOME") };

        ArgParser arg_parser({ p_argv, p_argv + p_argc });

        arg_parser.add_flag({ "-h", "--help"    });
        arg_parser.add_flag({ "-V", "--version" });

        arg_parser.add_option<std::string>({ ""  , "--config"    }, "");
        arg_parser.add_option<std::string>({ "-l", "--log"       }, "warn");

        return arg_parser.get();
    }


    auto
    start_app( const std::shared_ptr<Logger> &p_logger,
               const std::shared_ptr<Config> &p_config ) -> int32_t
    {
        p_logger->log(debug, "Initializing SDL");
        auto _ { sdl::init(APP_NAME, APP_VERSION, "org.kei.guiterm").or_else(
        [&p_logger]( const std::string &p_msg ) -> sdl::void_or_msg
        {
            p_logger->log(error, "Failed to initialize SDL: {}", p_msg);
            exit(1);
        }) };

        std::string title { APP_NAME };
        if (utils::json_valid_member(p_config->get_json(),
                                    "terminal", "window", "title")) {
            title = (*p_config)["terminal"]["window"]["title"].asString();
        }
        sdl::Window window { p_logger, title, { 800, 600 } };

        window.set_bg(0x0E0E0EFF_rgba);

        if (window.run() == EXIT_FAILURE) return EXIT_FAILURE;

        p_logger->log(debug, "Quitting SDL");
        sdl::quit();

        return EXIT_SUCCESS;
    }
}


auto
main( int32_t p_argc, char **p_argv ) -> int32_t
{
    auto args { get_args(p_argc, p_argv) };

    if (*std::get_if<bool>(&args.at("help"))) {
        print_help(std::cout, *p_argv);
        exit(0);
    }

    if (*std::get_if<bool>(&args.at("version"))) {
        print_version(std::cout);
        exit(0);
    }

#define GET_ARG( p_name ) \
    *std::get_if<std::string>(&args.at(p_name))

    std::string logger_arg { GET_ARG("log"   ) };
    std::string config_arg { GET_ARG("config") };

    auto logger { std::make_shared<Logger>(logger_arg)         };
    auto config { std::make_shared<Config>(logger, config_arg) };

    return start_app(logger, config);
}