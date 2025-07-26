#pragma once
#include <iostream>
#include <fstream>
#include <format>
#include <array>
#include <print>


enum log_level : unsigned char
{
    debug = 0,
    info  = 1,
    warn  = 2,
    error = 3,
    MAX   = 4
};


class Logger
#ifndef HEADER
{
    using label_pair = std::pair<std::string_view, std::string_view>;
public:
    struct fmt_data
    {
        std::string func;
        std::string file;
        int32_t line;
    };

    Logger( const std::string &p_arg_string );


    template<typename... T_Args>
    void _log( const log_level  p_level,
               const fmt_data  &p_data,
               std::string_view p_fmt,
               T_Args      &&...p_args )
    {
        std::string msg { std::vformat(p_fmt,
                                       std::make_format_args(p_args...)) };
        std::string label {
            std::format("{} {} at \033[1m{}\033[0m( \033[1;30m{}:{}\033[0;0m )",
                         get_time(),
                         m_LABELS.at(p_level).first,
                         p_data.func, p_data.file, p_data.line)
        };

        if (m_log_file.is_open()) {
            std::string file_label {
                std::format("{} at {}( {}:{} )",
                             get_time(),
                             m_LABELS.at(p_level).second,
                             p_data.func, p_data.file, p_data.line)
            };

            std::println(m_log_file, "[{}]: {}", file_label, msg);
            m_log_file.flush();
        }

        if (p_level >= m_threshold_level) {
            std::ostream &stream { p_level >= warn ? std::cerr : std::cout };

            std::println(stream, "[{}]: \033[1m{}\033[0m", label, msg);
            stream.flush();
        }
    }

private:
    static constexpr std::array<label_pair, 4> m_LABELS {{
        { "\033[1;36mdebug\033[0;0;0m", "debug" },
        { "\033[1;32minfo\033[0;0;0m ", "info " },
        { "\033[1;33mwarn\033[0;0;0m ", "warn " },
        { "\033[1;31merror\033[0;0;0m", "error" },
    }};

    std::ofstream m_log_file;
    log_level     m_threshold_level;


    [[nodiscard]]
    auto get_time( void ) const -> const std::string;
}
#endif
;


#define log( p_level, ... ) \
    _log(p_level, { __FUNCTION__, __FILE__, __LINE__ }, __VA_ARGS__)