#include "engine/core/base.hpp"
#include "engine/core/logging.hpp"
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

Config::Config() {
    std::ifstream settingsFile(PROJECT_ROOT "/settings.json");
    if(!settingsFile.is_open()){
        EGE_ERROR("settings.json file not found, default settings will be used.");
        return;
    }
    EGE_INFO("Reading settings from json.");
    try {
        nlohmann::json jsonData = nlohmann::json::parse(settingsFile);
        if (jsonData.contains("PLATFORM")){
            //WindowSettings
            u32 winH = jsonData["PLATFORM"].value("windowHeight", ConfigDefaults::HEIGHT);
            u32 winW = jsonData["PLATFORM"].value("windowWidth", ConfigDefaults::WIDTH);
            //String title = jsonData["PLATFORM"].value("windowTitle", ConfigDefaults::TITLE);

            //LoggingSetting
            String logLevel = jsonData["PLATFORM"].value("logLevel", "INFO");
            
            //PerfomaceSettings
            u32 tFPS = jsonData["PLATFORM"].value("targetFPS", ConfigDefaults::TARGETFPS);

            window.width = winW;
            window.height = winH;
            perfomance.targetFPS = tFPS;
            logging.level = stringToLogLevel(logLevel);

        }
    } catch (nlohmann::json::parse_error& exeption) {
        EGE_ERROR(exeption.what());
        EGE_ERROR("An error occurred while parsing; default settings will be used.");
    } 
    
}

EGE_NAMESPACE_END