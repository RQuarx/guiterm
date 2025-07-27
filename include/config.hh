#pragma once
#include <filesystem>
#include <json/value.h>

namespace fs = std::filesystem;
class Logger;


class Config
{
public:
    Config( const std::shared_ptr<Logger> &p_logger,
            fs::path                       p_config_path = "" );


    [[nodiscard]]
    auto get_json( void ) const -> Json::Value
    { return m_config; }


    template<typename T_Param>
    [[nodiscard]]
    auto operator[]( const T_Param &p_idx ) const -> Json::Value
    { return m_config[p_idx]; }

private:
    static constexpr std::array<std::string_view, 5> m_CONFIG_DIRS {{
        "{}/.config/guiterm/",
        "{}/.config/",
        "{}/",
        "/etc/guiterm/",
    }};

    std::shared_ptr<Logger> m_logger;
    Json::Value             m_config;
};