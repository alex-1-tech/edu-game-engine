#pragma once

#include <unordered_map>
#include <variant>

#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"


EGE_NAMESPACE_BEGIN

class IConfigLoader;
class JSONConfigLoader;

struct Validate {
public:
  static auto range(u32 value, u32 min, u32 max) -> bool { return (min <= value && value <= max); }
};

class EngineConfig final
{
public:
  using ConfigValue = std::variant<u32, String, LogLevel>;
  enum class Property : u8 { WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, TARGET_FPS, LOGLEVEL };

  ~EngineConfig() = default;

  EngineConfig() = default;
  EngineConfig(EngineConfig&) = delete;    ///< Copy constructor deleted
  EngineConfig(EngineConfig&&) = delete;   ///< Move constructor deleted
  auto operator=(EngineConfig&) = delete;  ///< Copy assignment deleted
  auto operator=(EngineConfig&&) = delete; ///< Move assignment deleted

  static auto getInstance() -> EngineConfig&
  {
    static EngineConfig instance;
    return instance;
  }
  auto loadFromFile(const IConfigLoader& loader, const String& path) -> bool;

  [[nodiscard]] auto saveToFile(const IConfigLoader& loader, const String& path) const -> bool;

  template<typename T> [[nodiscard]] auto getProperty(Property configProperty) const -> T;
  template<typename T> void setProperty(Property configProperty, T value);

private:
  std::unordered_map<Property, ConfigValue> m_values;
};

class IConfigLoader
{
public:
  virtual ~IConfigLoader() = default;

  IConfigLoader() = default;
  IConfigLoader(const IConfigLoader&) = delete;
  auto operator=(const IConfigLoader&) -> IConfigLoader& = delete;
  IConfigLoader(IConfigLoader&&) = delete;
  auto operator=(IConfigLoader&&) -> IConfigLoader& = delete;

  [[nodiscard]] virtual auto save(const EngineConfig& config, const String& path) const -> bool = 0;
  virtual auto load(EngineConfig& config, const String& path) const -> bool = 0;
};


class JSONConfigLoader : public IConfigLoader
{
public:
  JSONConfigLoader() { EGE_INFO("Loader INIT"); }

  [[nodiscard]] auto save(const EngineConfig& config, const String& path) const -> bool override;
  auto load(EngineConfig& config, const String& path) const -> bool override;
};

struct ConfigDefaults {
  // WindowSettings
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

  // GraphicsSettings
  static constexpr u32 MSAA = 4;
  static constexpr f32 ANISOTROPY = 8.0F;

  // LoggingSetting
  static constexpr LogLevel LOGLEVEL = LogLevel::INFO;

  // PerfomaceSettings
  static constexpr u32 TARGET_FPS = 120;
  static constexpr u32 MIN_TFPS = 30;
  static constexpr u32 MAX_TFPS = 400;

  static constexpr bool CAPFPS = true;

  static constexpr const char* PATH_TO_CONFIG = "settings.json"; // PROJECT_ROOT "/settings.json"

  struct PropertyInfo {
    EngineConfig::ConfigValue defaultValue;
    u32 min;
    u32 max;
  };

  static void setUpDefaultSettings(EngineConfig& config);
  static auto getDefaultSettingsMap() -> const std::unordered_map<EngineConfig::Property, PropertyInfo>&;
};


template<typename T> [[nodiscard]] auto EngineConfig::getProperty(Property configProperty) const -> T
{
  auto iterator = m_values.find(configProperty);
  if (iterator != m_values.end()) {
    if (std::holds_alternative<T>(iterator->second)) {
      return std::get<T>(iterator->second);
    }
    EGE_ERROR("EGE::Config: Type mismatch in getProperty");
    return T{};
  }
  EGE_ERROR("EGE::Config: Unknown Property in getProperty");
  return T{};
}


template<typename T> void EngineConfig::setProperty(Property configProperty, T value)
{
  const auto& schema = ConfigDefaults::getDefaultSettingsMap();
  auto defaultSettingsIterator = schema.find(configProperty);
  if (defaultSettingsIterator == schema.end()) {
    EGE_ERROR("EGE::Config: Unknown Property in setProperty");
    return;
  }

  const auto& defaultSettingsInfo = defaultSettingsIterator->second;

  if constexpr (std::is_same_v<T, u32>) {
    if (Validate::range(value, defaultSettingsInfo.min, defaultSettingsInfo.max)) {
      m_values[configProperty] = value;
      return;
    }
  } else if constexpr (std::is_same_v<T, String>) {
    if (Validate::range(static_cast<u32>(value.length()), defaultSettingsInfo.min, defaultSettingsInfo.max)) {
      m_values[configProperty] = std::move(value);
      return;
    }
  } else if constexpr (std::is_same_v<T, LogLevel>) {
    m_values[configProperty] = value;
    return;
  } else {
    EGE_WARN("EGE::Config: Type mismatch in setProperty");
    return;
  }

  EGE_WARN("EGE::Config: Validation failed using default");
  m_values[configProperty] = defaultSettingsInfo.defaultValue;
}

EGE_NAMESPACE_END