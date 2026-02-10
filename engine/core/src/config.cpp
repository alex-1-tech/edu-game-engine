#include "engine/core/config.hpp"

#include "engine/core/base.hpp"
#include "engine/core/config/config_loader.hpp"
#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"


EGE_NAMESPACE_BEGIN

auto EngineConfig::loadFromFile(const IConfigLoader& loader, const String& path) -> bool
{
  return loader.load(*this, path);
}

auto EngineConfig::saveToFile(const IConfigLoader& loader, const String& path) const -> bool
{
  return loader.save(*this, path);
}

void EngineConfig::intialize()
{
  setUpDefaultSettings();
  loadFromFile(JSONConfigLoader{}, String(ConfigDefaults::PATH_TO_CONFIG));
  EGE_INFO("Loading settings from JSON file completed...");
}

void EngineConfig::setUpDefaultSettings()
{
  const auto& defaultSettingsMap = ConfigDefaults::getDefaultSettingsMap();

  for (const auto& [mapProperty, mapInfo] : defaultSettingsMap) {
    std::visit(
        [this, property = mapProperty](auto&& defaultValue) {
          this->setProperty<std::decay_t<decltype(defaultValue)>>(property, std::forward<decltype(defaultValue)>(defaultValue));
        },
        mapInfo.defaultValue);
  }
}


EGE_NAMESPACE_END