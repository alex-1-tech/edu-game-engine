#pragma once

#include "engine/core/base.hpp"
#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"

EGE_NAMESPACE_BEGIN

class EngineConfig;

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

EGE_NAMESPACE_END