#include "engine/core/config.hpp"

#include "engine/core/base.hpp"
#include "engine/core/config_loader.hpp"
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
  loadFromFile(JSONConfigLoader{}, String(ConfigDefaults::PATH_TO_CONFIG));
  EGE_INFO("Loading settings from JSON file completed...");
}


EGE_NAMESPACE_END