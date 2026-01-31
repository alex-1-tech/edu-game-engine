#include "engine/core/base.hpp"
#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"
#include "engine/core/config.hpp"

#include <nlohmann/json.hpp>
#include <fstream>


EGE_NAMESPACE_BEGIN

auto JSONConfigLoader::save(const Config& config, const String& path) const -> bool{
    nlohmann::json jsonObj;

    jsonObj = {
            {"PLATFORM", {
            {"windowWidth", config.getParametr<u32>(Config::Param::WINDOW_WIDTH)},
            {"windowHeight", config.getParametr<u32>(Config::Param::WINDOW_HEIGHT)},
            {"title", config.getParametr<String>(Config::Param::WINDOW_TITLE)},
            {"targetFPS", config.getParametr<u32>(Config::Param::TARGET_FPS)},
            {"logLevel", Logger::logLevelToString(config.getParametr<LogLevel>(Config::Param::LOGLEVEL)).value()}
        }}
    };

    std::ofstream file(path, std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        EGE_ERROR("Failed to create or open file: {}", path);
        return false;
    }

    file << jsonObj.dump();
    EGE_INFO("Write config in json");
    return true; 
}

auto JSONConfigLoader::load(Config& config, const String& path) const -> bool {
    std::ifstream file(path);
    if (!file.is_open()) {
        EGE_ERROR("Config file not found: {}", path);
        return false;
    }

    nlohmann::json json;
    try {
        json = nlohmann::json::parse(file);
    } catch (const nlohmann::json::parse_error& e) {
        EGE_ERROR("JSON Syntax Error: {}", e.what());
        return false;
    }

    if (!json.contains("PLATFORM")) {
        EGE_ERROR("Critical error: Section [PLATFORM] missing in config!");
        return false;
    }

    const auto& platform = json["PLATFORM"];
    if (platform.contains("windowWidth")) {
        config.setParametr(Config::Param::WINDOW_WIDTH, platform["windowWidth"].get<u32>());}
        
    if (platform.contains("windowHeight")){
        config.setParametr(Config::Param::WINDOW_HEIGHT, platform["windowHeight"].get<u32>());}

    if (platform.contains("targetFPS")){
        config.setParametr(Config::Param::TARGET_FPS, platform["targetFPS"].get<u32>());}

    if (platform.contains("logLevel")) {
        String levelStr = platform["logLevel"].get<String>();
        config.setParametr(Config::Param::LOGLEVEL, Logger::stringToLogLevel(levelStr));
    }
    if (platform.contains("title")) {
        config.setParametr(Config::Param::WINDOW_TITLE, platform["title"].get<String>());
    }
    return true;
}

auto Config::loadFromFile(const IConfigLoader& loader, const String& path) -> bool {
        return loader.load(*this, path);
    }

auto Config::saveToFile(const IConfigLoader& loader, const String& path) const -> bool{
    return loader.save(*this, path);
}


EGE_NAMESPACE_END