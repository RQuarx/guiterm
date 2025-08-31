#include <expected>
#include <cstring>
#include <unistd.h>
#include <pwd.h>

#include "process.hh"
#include "log.hh"


namespace
{
    [[nodiscard]]
    auto get_user_shell( void ) -> std::expected<fs::path, std::string>
    {
        uid_t   uid { getuid() };
        passwd *pw  { getpwuid(uid) };

        if (pw == nullptr)
            return std::unexpected(std::strerror(errno));

        return pw->pw_shell;
    }
}


Process::Process( const std::shared_ptr<Logger>  &p_logger,
                  const fs::path                 &p_proc,
                  const fs::path                 &p_cwd,
                  const std::vector<std::string> &p_args ) :
    m_logger(p_logger)
{
    if (p_proc.empty()) {
        auto buff { get_user_shell() };

        if (!buff) {
            m_logger->log(log_level::error,
                         "Failed to get user shell: {}", buff.error());
            std::exit(EXIT_FAILURE);
        }

        m_proc = *buff;
    } else { m_proc = p_proc; }

    m_logger->log(info, "Process is set as {}.", m_proc.string());

    // int32_t retval { run_process() };
    // if (retval < 0) {
    //     m_logger->log(error, "Failed to run process {}: {}",
    //                   m_proc.string(), 1);
    // }
}


auto
Process::run_process( const char *p_file,
                      const char *p_cwd,
                      char      **p_args ) -> int32_t
{
    
}