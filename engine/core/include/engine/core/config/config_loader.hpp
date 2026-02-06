#pragma once

/**
 * @file    config/config_loader.hpp
 * @brief   Interface and implementation of the engine configuration loader in JSON format.
 * @author  jhfawk
 * @date    2026
 */

#include "engine/core/base.hpp"
#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"

EGE_NAMESPACE_BEGIN

class EngineConfig;

/**
 * @class IConfigLoader
 * @brief Abstract interface for handling engine configuration saving
 */
class IConfigLoader
{
public:
  virtual ~IConfigLoader() = default;

  IConfigLoader() = default;
  IConfigLoader(const IConfigLoader&) = delete;
  auto operator=(const IConfigLoader&) -> IConfigLoader& = delete;
  IConfigLoader(IConfigLoader&&) = delete;
  auto operator=(IConfigLoader&&) -> IConfigLoader& = delete;

  /**
   * @brief Saves the configuration to a file at the specified path.
   * @return true if the operation was successful
   */
  [[nodiscard]] virtual auto save(const EngineConfig& config, const String& path) const -> bool = 0;

  /**
   * @brief Loads configuration data from a file into the co  nfig object
   * @return true if the file was successfully read and parsed
   */
  virtual auto load(EngineConfig& config, const String& path) const -> bool = 0;
};


/**
 * @class JSONConfigLoader
 * @brief JSON-based implementation of the configuration loader
 */
class JSONConfigLoader : public IConfigLoader
{
public:
  JSONConfigLoader() { EGE_INFO("Loader INIT"); }


  /**
   * @brief Serializes EngineConfig to JSON format and writes it to disk
   */
  [[nodiscard]] auto save(const EngineConfig& config, const String& path) const -> bool override;


  /**
   * @brief Parses a JSON file and populates the EngineConfig
   */
  auto load(EngineConfig& config, const String& path) const -> bool override;
};

EGE_NAMESPACE_END