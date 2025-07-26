#include <cstring>
#include <chrono>
#include "utils.hh"
#include "log.hh"


namespace
{
    auto
    string_to_loglevel( const std::string &p_str ) -> log_level
    {
        if (p_str.contains("debug")) return debug;
        if (p_str.contains("info" )) return info;
        if (p_str.contains("warn" )) return warn;
        if (p_str.contains("error")) return error;
        return MAX;
    }
}


Logger::Logger( const std::string &p_arg_string )
{
    /* The input to the ctor should be either 'n', 'm', 'n,m', or 'm,n',
       where n is a number in a range of 0 -> 3, or a log-level,
       and m is a file path for the log file.
    */

    const size_t comma_idx { p_arg_string.find(',') };
    auto [ num, name ] { utils::split_string(p_arg_string, comma_idx) };

    try {
        int32_t level { std::stoi(num) };
        if (level >= log_level::MAX) {
            log(warn, "Log level too large, using default 'warn' level.");
            throw std::exception(); /* Trigger the catch */
        } else {
            m_threshold_level = static_cast<log_level>(std::stoi(num));
        }
    } catch (...) {
        log_level level { string_to_loglevel(num) };

        if (level == MAX) {
            m_threshold_level = warn;
            std::swap(name, num);
        } else {
            m_threshold_level = level;
        }
    }

    if (!name.empty()) {
        m_log_file.open(name, std::ios_base::app);
        if (m_log_file.fail() && !m_log_file.eof()) {
            log(error, "Failed to open {}: {}", name, std::strerror(errno));
            exit(1);
        }
    }

    log(debug, "Logger instance successfully created with a log-level of {}",
        m_LABELS.at(m_threshold_level).second);

}


auto
Logger::get_time( void ) const -> const std::string
{
    using std::chrono::duration;
    using ms = std::chrono::milliseconds;
    using m  = std::chrono::minutes;
    using s  = std::chrono::seconds;

    duration now { std::chrono::system_clock::now().time_since_epoch() };
    ms millis    { std::chrono::duration_cast<ms>(now) % 1000 };
    m  minutes   { std::chrono::duration_cast<m >(now) % 60 };
    s  seconds   { std::chrono::duration_cast<s >(now) % 60 };

    return std::format("{:02}:{:02}.{:03}",
                        minutes.count(), seconds.count(), millis.count());
}