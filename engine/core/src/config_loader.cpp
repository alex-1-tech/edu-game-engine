#include "engine/core/config_loader.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <optional>

#include "engine/core/base.hpp"
#include "engine/core/config.hpp"
#include "engine/core/config_default.hpp"
#include "engine/core/config_types.hpp"
#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"

EGE_NAMESPACE_BEGIN
auto JSONConfigLoader::save(const EngineConfig& config, const String& path) const -> bool
{
  nlohmann::json jsonObj;

  jsonObj = {{"PLATFORM",
              {{"windowWidth", config.getProperty<u32>(Property::WINDOW_WIDTH)},
               {"windowHeight", config.getProperty<u32>(Property::WINDOW_HEIGHT)},
               {"title", config.getProperty<String>(Property::WINDOW_TITLE)},
               {"targetFPS", config.getProperty<u32>(Property::TARGET_FPS)},
               {"logLevel", Logger::logLevelToString(config.getProperty<LogLevel>(Property::LOGLEVEL)).value()}}}};

  std::ofstream file(path, std::ios::out | std::ios::trunc);

  if (!file.is_open()) {
    EGE_ERROR("Failed to create or open file: {}", path);
    return false;
  }

  file << jsonObj.dump();
  EGE_INFO("Write config in json");
  return true;
}

auto JSONConfigLoader::load(EngineConfig& config, const String& path) const -> bool
{
  ConfigDefaults::setUpDefaultSettings(config);

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
    config.setProperty(Property::WINDOW_WIDTH, platform["windowWidth"].get<u32>());
  }
  if (platform.contains("windowHeight")) {
    config.setProperty(Property::WINDOW_HEIGHT, platform["windowHeight"].get<u32>());
  }
  if (platform.contains("targetFPS")) {
    config.setProperty(Property::TARGET_FPS, platform["targetFPS"].get<u32>());
  }
  if (platform.contains("logLevel")) {
    std::optional<LogLevel> logLevelOpt = Logger::stringToLogLevel(platform["logLevel"].get<String>());
    if (logLevelOpt.has_value()) {
      config.setProperty(Property::LOGLEVEL, logLevelOpt.value());
    }
  }
  if (platform.contains("title")) {
    config.setProperty(Property::WINDOW_TITLE, platform["title"].get<String>());
  }
  return true;
}
EGE_NAMESPACE_END