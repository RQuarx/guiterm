#include <iostream>
#include <cstdint>

#include <SDL3_ttf/SDL_ttf.h>
#include <harfbuzz/hb.h>
#include <json/json.h>

#include "sdl/window.hh"
#include "arg_parser.hh"
#include "log.hh"
#include "sdl.hh"


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
            "  -c --class      <str>       Define window class/app_id.          (guiterm)\n"
            "  -d --directory  <dir>       Set custom starting directory.       ($HOME)  \n"
            "  -e --command    <cmd>       Command and args to execute.                  \n"
            "  -l --log        <int,dir>   Set log-level (0-3) and or log file. (warn/2) \n"
            "  -t --title      <str>       Set a custom window title.           (guiterm)\n"
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
}


int32_t
main( int32_t p_argc, char **p_argv )
{
    ArgParser arg_parser({ p_argv, p_argv + p_argc });

    arg_parser.add_flag({ "-V", "--version" });
    arg_parser.add_flag({ "-h", "--help"    });

    arg_parser.add_option<std::string>({ "-l", "--log"       }, "warn");
    arg_parser.add_option<std::string>({ "-c", "--class"     }, "guiterm");
    arg_parser.add_option<std::string>({ "-t", "--title"     }, "guiterm");
    arg_parser.add_option<std::string>({ "-e", "--command"   }, "");
    arg_parser.add_option<std::string>({ "-d", "--directory" }, "");

    auto args { arg_parser.get() };

    if (*std::get_if<bool>(&args.at("help"))) {
        print_help(std::cout, *p_argv);
        exit(0);
    }

    if (*std::get_if<bool>(&args.at("version"))) {
        print_version(std::cout);
        exit(0);
    }

    std::string *logger_arg { std::get_if<std::string>(&args.at("log")) };

    std::shared_ptr<Logger> logger { std::make_shared<Logger>(*logger_arg) };

    logger->log(debug, "Initializing SDL");
    auto _ { sdl::init(APP_NAME, APP_VERSION, "org.kei.guiterm").or_else(
    [&logger]( const std::string &p_msg ) -> sdl::void_or_msg
    {
        logger->log(error, "Failed to initialize SDL: {}", p_msg);
        exit(1);
    }) };


    sdl::Window window { logger, APP_NAME, { 800, 600 } };

    window.set_bg(0x0E0E0E_rgb);


    logger->log(debug, "Quitting SDL");
    sdl::quit();
}