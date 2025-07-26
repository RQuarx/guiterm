#pragma once
#include <filesystem>
#include <string>

namespace fs = std::filesystem;


namespace utils
{
    /**
     * @brief Splits a string at @p p_idx .
     *
     * @param p_str The string to split.
     * @param p_pos The index where the string will be split.
     * @return An std::pair<std::string, std::string> instance.
     *
     * The first string of the pair will contain the
     *   0 -> p_idx part of the string,
     * and the second string will contain
     *  p_idx + 1 -> str.length() part of the string.
     */
    auto split_string( const std::string &p_str,
                       const size_t      &p_pos
                     ) -> std::pair<std::string, std::string>;


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
}