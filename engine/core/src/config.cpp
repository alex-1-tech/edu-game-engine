#include "engine/core/base.hpp"
#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"
#include "engine/core/config.hpp"

#include <nlohmann/json.hpp>
#include <fstream>


EGE_NAMESPACE_BEGIN

auto Config::stringToLogLevel(String& level) -> LogLevel{
    auto map_iterator = stringToLevel.find(level);
    if (map_iterator != stringToLevel.end())
    {
        return map_iterator->second;
    }
    EGE_ERROR("You entered an incorrect logging level in settings.json.");
    return ConfigDefaults::LOGLEVEL;
}

void Config::loadConfig() {
    std::ifstream file(PROJECT_ROOT "/settings.json");
    if (!file.is_open()) {
        EGE_ERROR("settings.json not found, using defaults.");
        return;
    }

    try {
        auto json = nlohmann::json::parse(file);
        
        if (json.contains("PLATFORM")) {
            const auto& platform = json["PLATFORM"];
            
            if (platform.contains("windowWidth")) {
                setParametr(Param::WINDOW_WIDTH, platform["windowWidth"].get<u32>());}
                
            if (platform.contains("windowHeight")){
                setParametr(Param::WINDOW_HEIGHT, platform["windowHeight"].get<u32>());}

            if (platform.contains("targetFPS")){
                setParametr(Param::TARGET_FPS, platform["targetFPS"].get<u32>());}

            if (platform.contains("logLevel")) {
                String levelStr = platform["logLevel"].get<String>();
                setParametr(Param::LOGLEVEL, stringToLogLevel(levelStr));
            }
            if (platform.contains("title")) {
                setParametr(Param::WINDOW_TITLE, platform["title"].get<String>());
            }

        }
    } catch (const nlohmann::json::parse_error& e) {
        EGE_ERROR("JSON Parse Error: {}", e.what());
    }
}

Config::Config() {
    loadConfig();
}

EGE_NAMESPACE_END