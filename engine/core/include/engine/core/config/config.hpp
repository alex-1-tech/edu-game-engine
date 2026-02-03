#pragma once

/**
 * @file    engine_config.hpp
 * @brief   Core configuration management system (Singleton)
 * @author  jhfawk
 * @date    2026
 */

#include <unordered_map>
#include <variant>

#include "engine/core/config/config_loader.hpp"
#include "engine/core/config/config_types.hpp"
#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"

EGE_NAMESPACE_BEGIN

class IConfigLoader;
class JSONConfigLoader;
class ConfigDefaults;

/**
 * @struct Validate
 * @brief  Helper utility for value range and length verification
 */
struct Validate {
public:
  /** @brief Checks if a numeric value is within [min, max] range */
  static auto range(u32 value, u32 min, u32 max) -> bool { return (min <= value && value <= max); }

  /** @brief Checks if string length is within [min, max] range */
  static auto range(const String& value, u32 minLength, u32 maxLength) -> bool
  {
    u32 length = static_cast<u32>(value.length());
    return (minLength <= length && length <= maxLength);
  }

  /** @brief Validation for LogLevel (always valid) */
  static auto range(LogLevel /*unused*/, u32 /*unused*/, u32 /*unused*/) -> bool { return true; }
};

/**
 * @class EngineConfig
 * @brief Thread-safe Singleton managing all engine settings and their data storage
 */
class EngineConfig final
{
public:
  ~EngineConfig() = default;

  EngineConfig(EngineConfig&) = delete;
  EngineConfig(EngineConfig&&) = delete;
  auto operator=(EngineConfig&) = delete;
  auto operator=(EngineConfig&&) = delete;

  /** @brief Returns the global instance of the configuration manager*/
  static auto getInstance() -> EngineConfig&
  {
    static EngineConfig instance;
    return instance;
  }

  /** @brief Loads configuration from file using the provided loader*/
  auto loadFromFile(const IConfigLoader& loader, const String& path) -> bool;

  /** @brief Core initialization of the config system */
  void intialize();

  /** @brief Reverts all settings to their predefined default values */
  void setUpDefaultSettings();

  /** @brief Serializes and saves current settings to a file with provided loader*/
  [[nodiscard]] auto saveToFile(const IConfigLoader& loader, const String& path) const -> bool;

  /** @brief Retrieves a property value by its key (with type safety) */
  template<typename T> [[nodiscard]] auto getProperty(Property configProperty) const -> T;

  /** @brief Validates and updates a property value */
  template<typename T> void setProperty(Property configProperty, T value);

private:
  EngineConfig() = default;
  std::unordered_map<Property, ConfigValue> m_values;
};

/**
 * @struct ConfigDefaults
 * @brief  Hardcoded default values and metadata for engine properties
 */
struct ConfigDefaults {
  // Window Settings
  static constexpr u32 WIDTH = 1280;
  static constexpr u32 MIN_WIDTH = 700;
  static constexpr u32 MAX_WIDTH = 1920;

  static constexpr u32 HEIGHT = 720;
  static constexpr u32 MIN_HEIGHT = 480;
  static constexpr u32 MAX_HEIGHT = 1080;

  static constexpr bool FULLSCREEN = false;
  static constexpr bool VSYNC = true;

  static constexpr const char* TITLE = "EduGame Engine";
  static constexpr u32 MIN_TITLE_LEN = 4;
  static constexpr u32 MAX_TITLE_LEN = 30;

  // Graphics Settings
  static constexpr u32 MSAA = 4;
  static constexpr f32 ANISOTROPY = 8.0F;

  // Logging Settings
  static constexpr LogLevel LOGLEVEL = LogLevel::INFO;

  // Performance Settings
  static constexpr u32 TARGET_FPS = 120;
  static constexpr u32 MIN_TFPS = 30;
  static constexpr u32 MAX_TFPS = 400;

  static constexpr bool CAPFPS = true;

  static constexpr const char* PATH_TO_CONFIG = PROJECT_ROOT "/settings.json";

  /** @brief Returns a static map containing default values and validation rules */
  static auto getDefaultSettingsMap() -> const std::unordered_map<Property, PropertyInfo>&
  {
    constexpr int UNUSED_VALUE = ZERO;
    static const std::unordered_map<Property, PropertyInfo> map = {
        {Property::WINDOW_WIDTH, {ConfigDefaults::WIDTH, ConfigDefaults::MIN_WIDTH, ConfigDefaults::MAX_WIDTH}},
        {Property::WINDOW_HEIGHT, {ConfigDefaults::HEIGHT, ConfigDefaults::MIN_HEIGHT, ConfigDefaults::MAX_HEIGHT}},
        {Property::WINDOW_TITLE, {String(ConfigDefaults::TITLE), ConfigDefaults::MIN_TITLE_LEN, ConfigDefaults::MAX_TITLE_LEN}},
        {Property::LOGLEVEL, {ConfigDefaults::LOGLEVEL, UNUSED_VALUE, UNUSED_VALUE}},
        {Property::TARGET_FPS, {ConfigDefaults::TARGET_FPS, ConfigDefaults::MIN_TFPS, ConfigDefaults::MAX_TFPS}}};
    return map;
  }
};

/**
 * @brief  Template implementation for safe property access
 * @throws Logs an error if property is missing or type is incorrect
 */
template<typename T> [[nodiscard]] auto EngineConfig::getProperty(Property configProperty) const -> T
{
  auto iterator = m_values.find(configProperty);
  if (iterator != m_values.end()) {
    if (std::holds_alternative<T>(iterator->second)) {
      return std::get<T>(iterator->second);
    }
    EGE_ERROR("EGE::Config: Type mismatch for property requested!");
    return T{};
  }
  EGE_ERROR("EGE::Config: Attempted to get an unregistered property!");
  return T{};
}

/**
 * @brief  Template implementation for property modification with range validation
 * @note   Values failing validation will trigger a warning and won't be applied
 */
template<typename T> void EngineConfig::setProperty(Property configProperty, T value)
{
  const auto& defaultSettingsMap = ConfigDefaults::getDefaultSettingsMap();
  auto defaultSettingsIterator = defaultSettingsMap.find(configProperty);

  if (defaultSettingsIterator == defaultSettingsMap.end()) {
    EGE_ERROR("EGE::Config: Cannot set value for unknown property!");
    return;
  }

  const auto& info = defaultSettingsIterator->second;

  if (Validate::range(value, info.min, info.max)) {
    m_values[configProperty] = std::move(value);
  } else {
    EGE_WARN("EGE::Config: Validation failed - value is out of bounds!");
  }
}

EGE_NAMESPACE_END
