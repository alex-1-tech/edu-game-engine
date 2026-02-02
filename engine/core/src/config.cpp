#include "engine/core/config.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <optional>
#include <unordered_map>

#include "engine/core/base.hpp"
#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"


EGE_NAMESPACE_BEGIN

auto ConfigDefaults::getDefaultSettingsMap() -> const std::unordered_map<EngineConfig::Property, PropertyInfo>&
{
  static const std::unordered_map<EngineConfig::Property, PropertyInfo> map = {
      {EngineConfig::Property::WINDOW_WIDTH, {ConfigDefaults::WIDTH, ConfigDefaults::MIN_WIDTH, ConfigDefaults::MAX_WIDTH}},
      {EngineConfig::Property::WINDOW_HEIGHT, {ConfigDefaults::HEIGHT, ConfigDefaults::MIN_HEIGHT, ConfigDefaults::MAX_HEIGHT}},
      {EngineConfig::Property::WINDOW_TITLE, {String(ConfigDefaults::TITLE), ConfigDefaults::MIN_TITLE_LEN, ConfigDefaults::MAX_TITLE_LEN}},
      {EngineConfig::Property::LOGLEVEL, {ConfigDefaults::LOGLEVEL}},
      {EngineConfig::Property::TARGET_FPS, {ConfigDefaults::TARGET_FPS, ConfigDefaults::MIN_TFPS, ConfigDefaults::MAX_TFPS}}};
  return map;
}

void ConfigDefaults::setUpDefaultSettings(EngineConfig& config)
{
  const auto& defaultSettingsMap = getDefaultSettingsMap();

  for (const auto& [mapProperty, mapInfo] : defaultSettingsMap) {
    std::visit(
        [&config, property = mapProperty](auto&& defaultValue) {
          config.setProperty<std::decay_t<decltype(defaultValue)>>(property, std::forward<decltype(defaultValue)>(defaultValue));
        },
        mapInfo.defaultValue);
  }
}

auto JSONConfigLoader::save(const EngineConfig& config, const String& path) const -> bool
{
  nlohmann::json jsonObj;

  jsonObj = {{"PLATFORM",
              {{"windowWidth", config.getProperty<u32>(EngineConfig::Property::WINDOW_WIDTH)},
               {"windowHeight", config.getProperty<u32>(EngineConfig::Property::WINDOW_HEIGHT)},
               {"title", config.getProperty<String>(EngineConfig::Property::WINDOW_TITLE)},
               {"targetFPS", config.getProperty<u32>(EngineConfig::Property::TARGET_FPS)},
               {"logLevel", Logger::logLevelToString(config.getProperty<LogLevel>(EngineConfig::Property::LOGLEVEL)).value()}}}};

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
    config.setProperty(EngineConfig::Property::WINDOW_WIDTH, platform["windowWidth"].get<u32>());
  }
  if (platform.contains("windowHeight")) {
    config.setProperty(EngineConfig::Property::WINDOW_HEIGHT, platform["windowHeight"].get<u32>());
  }
  if (platform.contains("targetFPS")) {
    config.setProperty(EngineConfig::Property::TARGET_FPS, platform["targetFPS"].get<u32>());
  }
  if (platform.contains("logLevel")) {
    std::optional<LogLevel> logLevelOpt = Logger::stringToLogLevel(platform["logLevel"].get<String>());
    if (logLevelOpt.has_value()) {
      config.setProperty(EngineConfig::Property::LOGLEVEL, logLevelOpt.value());
    }
  }
  if (platform.contains("title")) {
    config.setProperty(EngineConfig::Property::WINDOW_TITLE, platform["title"].get<String>());
  }
  return true;
}

auto EngineConfig::loadFromFile(const IConfigLoader& loader, const String& path) -> bool
{
  return loader.load(*this, path);
}

auto EngineConfig::saveToFile(const IConfigLoader& loader, const String& path) const -> bool
{
  return loader.save(*this, path);
}


EGE_NAMESPACE_END