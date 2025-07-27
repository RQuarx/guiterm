#include <expected>
#include <fstream>
#include <json/reader.h>
#include "config.hh"
#include "log.hh"
#include "utils.hh"


namespace
{
    auto
    parse_config_file( const fs::path &p_path
                     ) -> std::expected<Json::Value, std::string>
    {
        Json::Value   root;
        std::ifstream file;

        try {
            file.open(p_path);
        } catch (const std::exception &e) {
            return std::unexpected(e.what());
        }

        if (file.fail() && !file.eof())
            return std::unexpected(std::strerror(errno));

        Json::CharReaderBuilder builder;
        std::string err;

        if (!Json::parseFromStream(builder, file, &root, &err))
            return std::unexpected(err);

        return root;
    }
}


Config::Config( const std::shared_ptr<Logger> &p_logger,
                fs::path                       p_config_path ) :
    m_logger(p_logger)
{
    if (p_config_path.empty()) {
        const std::string HOME { utils::getenv("HOME") };

        for (const auto &path : m_CONFIG_DIRS) {
            fs::path dir { utils::format(path, HOME) };

            if (!fs::exists(dir)) continue;

            p_config_path = dir;
            break;
        }
    } else {
        if (!fs::exists(p_config_path)) {
            m_logger->log(error, "Config file {} doesn't exist!",
                          p_config_path.string());
            exit(1);
        }

        if (fs::is_regular_file(p_config_path)) goto end;
    }

    for (const auto &entry : fs::directory_iterator(p_config_path)) {
        if (!entry.is_regular_file()) continue;
        fs::path file { entry.path() };

        if (file.stem() != "guiterm") continue;
        if (!utils::path_is_valid_json(file)) continue;

        p_config_path = file;
        goto end;
    }

    m_logger->log(error, "Unable to find config.");
    exit(1);

end:
    m_logger->log(info, "Parsing config file at {}", p_config_path.string());

    auto buff { parse_config_file(p_config_path) };
    if (!buff) {
        m_logger->log(error, "Failed to parse {}: {}",
                      p_config_path.string(), buff.error());
        exit(1);
    }
    m_config = *buff;
    return;
}