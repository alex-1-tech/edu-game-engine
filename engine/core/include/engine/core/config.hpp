#pragma once

#include <unordered_map>
#include <variant>

#include "engine/core/config_default.hpp"
#include "engine/core/config_loader.hpp"
#include "engine/core/config_types.hpp"
#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"


EGE_NAMESPACE_BEGIN

class IConfigLoader;
class JSONConfigLoader;
class ConfigDefaults;

struct Validate {
public:
  static auto range(u32 value, u32 min, u32 max) -> bool { return (min <= value && value <= max); }
  static auto range(const String& value, u32 minLength, u32 maxLength) -> bool
  {
    u32 length = static_cast<u32>(value.length());
    return (minLength <= length && length <= maxLength);
  }
  static auto range(LogLevel /*unused*/, u32 /*unused*/, u32 /*unused*/) -> bool { return true; }
};

class EngineConfig final
{
public:
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

  void intialize();


  [[nodiscard]] auto saveToFile(const IConfigLoader& loader, const String& path) const -> bool;

  template<typename T> [[nodiscard]] auto getProperty(Property configProperty) const -> T;
  template<typename T> void setProperty(Property configProperty, T value);

private:
  std::unordered_map<Property, ConfigValue> m_values;
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
  const auto& defaultSettingsMap = ConfigDefaults::getDefaultSettingsMap();
  auto defaultSettingsIterator = defaultSettingsMap.find(configProperty);

  if (defaultSettingsIterator == defaultSettingsMap.end()) {
    EGE_ERROR("EGE::Config: Unknown Property");
    return;
  }

  const auto& info = defaultSettingsIterator->second;

  if (Validate::range(value, info.min, info.max)) {
    m_values[configProperty] = std::move(value);
  } else {
    EGE_WARN("EGE::Config: Value out of range or validation failed");
  }
}

EGE_NAMESPACE_END