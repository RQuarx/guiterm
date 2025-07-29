#include <filesystem>
#include <algorithm>

#include "utils.hh"

namespace fs = std::filesystem;


namespace utils
{
    auto
    getenv( const std::string &p_env ) -> std::string_view
    {
        const char *val { ::getenv(p_env.c_str()) };
        return val == nullptr ? "" : val;
    }


    auto
    path_is_valid_json( const fs::path &p_path ) -> bool
    {
        if (!fs::exists(p_path)) return false;
        if (!fs::is_regular_file(p_path)) return false;

        std::string ext { p_path.extension() };
        std::ranges::transform(ext, ext.begin(), ::tolower);

        return ext == ".json" || ext == ".jsonc";
    }
}