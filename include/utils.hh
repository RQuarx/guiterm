#pragma once
#include <filesystem>
#include <string>
#include <json/value.h>

namespace fs = std::filesystem;


namespace utils
{
    /**
     * @brief A wrapper for ::getenv().
     *
     * @param p_env The environment variable to get the value of.
     * @return A non empty string if the variable exist, else an empty string.
     */
    auto getenv( const std::string &p_env ) -> std::string_view;


    /**
     * @brief Checks whether @p p_path is a valid json file
     *
     * @return true if p_path points to a .json/.jsonc file
     *         else, returns false.
     */
    auto path_is_valid_json( const fs::path &p_path ) -> bool;


    template<typename... T_Args>
    auto format( std::string_view p_fmt, T_Args &&...p_args ) -> std::string
    { return std::vformat(p_fmt, std::make_format_args(p_args...)); }


    inline auto
    json_valid_member( const Json::Value & /* p_root */ ) -> bool
    { return true; }


    template<typename... T_Rest>
    auto json_valid_member( const Json::Value &p_root,
                            const std::string &p_key,
                            T_Rest        &&...p_rest ) -> bool
    {
        if (!p_root.isObject() || !p_root.isMember(p_key)) return false;
        return json_valid_member(p_root[p_key], std::forward<T_Rest>(p_rest)...);
    }


    template<typename... T_Rest>
    auto json_valid_member( const Json::Value      &p_root,
                            const Json::ArrayIndex &p_idx,
                            T_Rest             &&...p_rest ) -> bool
    {
        if (!p_root.isArray() || !p_root.isValidIndex(p_idx)) return false;
        return json_valid_member(p_root[p_idx], std::forward<T_Rest>(p_rest)...);
    }
}