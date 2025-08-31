#pragma once
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;
class Logger;


class Process
{
public:
    Process( const std::shared_ptr<Logger>  &p_logger,
             const fs::path                 &p_proc,
             const fs::path                 &p_cwd,
             const std::vector<std::string> &p_args );


    void set_buffer_line_amount( const size_t &p_len );

    void run_proc( void );

private:
    std::shared_ptr<Logger> m_logger;
    fs::path m_proc;

    auto run_process( const char *p_file,
                      const char *p_cwd,
                      char      **p_args ) -> int32_t;
};