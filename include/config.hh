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


    template<typename... T_Keys>
    [[nodiscard]]
    auto get( T_Keys &&...p_keys ) -> Json::Value
    {
        Json::Value curr { m_config };

        bool all {
            (( curr = is_member(curr, p_keys)
             ? curr[p_keys] : Json::nullValue ) && ...)
        };

        return all ? curr : Json::nullValue;
    }


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


    static auto is_member( const Json::Value      &p_root,
                           const Json::ArrayIndex &p_idx ) -> bool;
    static auto is_member( const Json::Value      &p_root,
                           const std::string      &p_key ) -> bool;
};