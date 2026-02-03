#include "engine/core/config_default.hpp"

#include "engine/core/base.hpp"
#include "engine/core/config.hpp"
#include "engine/core/types.hpp"

EGE_NAMESPACE_BEGIN

auto ConfigDefaults::getDefaultSettingsMap() -> const std::unordered_map<Property, PropertyInfo>&
{
  static const std::unordered_map<Property, PropertyInfo> map = {
      {Property::WINDOW_WIDTH, {ConfigDefaults::WIDTH, ConfigDefaults::MIN_WIDTH, ConfigDefaults::MAX_WIDTH}},
      {Property::WINDOW_HEIGHT, {ConfigDefaults::HEIGHT, ConfigDefaults::MIN_HEIGHT, ConfigDefaults::MAX_HEIGHT}},
      {Property::WINDOW_TITLE, {String(ConfigDefaults::TITLE), ConfigDefaults::MIN_TITLE_LEN, ConfigDefaults::MAX_TITLE_LEN}},
      {Property::LOGLEVEL, {ConfigDefaults::LOGLEVEL, ZERO, ZERO}},
      {Property::TARGET_FPS, {ConfigDefaults::TARGET_FPS, ConfigDefaults::MIN_TFPS, ConfigDefaults::MAX_TFPS}}};
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

EGE_NAMESPACE_END