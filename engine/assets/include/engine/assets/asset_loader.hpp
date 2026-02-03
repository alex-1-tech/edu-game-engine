#pragma once

/**
 * @file    asset_loader.hpp
 * @brief   Resource Loader Interface
 * @author  alex-1-tech
 * @date    2026
 */

#include <typeindex>

#include "engine/core/resource_types.hpp"

EGE_NAMESPACE_BEGIN
class IAssetLoader
{
public:
  IAssetLoader() = default;
  virtual ~IAssetLoader() = default;

  IAssetLoader(const IAssetLoader&) = delete;
  auto operator=(const IAssetLoader&) -> IAssetLoader& = delete;
  IAssetLoader(IAssetLoader&&) = delete;
  auto operator=(IAssetLoader&&) -> IAssetLoader& = delete;

  [[nodiscard]] virtual auto supportsType(const std::type_index& type) const -> bool = 0;

  ///> Loads the resource synchronously (blocking the call)
  virtual auto loadSync(const Path& path) -> ResourceResult<std::shared_ptr<void>> = 0;

  virtual auto loadAsync(const Path& path, LoadPriority priority) -> ResourceFuture<void> = 0;

  virtual void unload(void* resource) = 0;

  [[nodiscard]] virtual auto getResourceType() const -> std::type_index = 0;

  [[nodiscard]] virtual auto getSupportedExtensions() const -> std::vector<std::string> = 0;
};

class AssetLoaderRegistry
{
public:
  template<typename T, typename Loader> void registerLoader(const std::string& extension);

  auto getLoaderForExtension(const std::string& extension) -> std::shared_ptr<IAssetLoader>;
  auto getLoaderForType(const std::type_index& type) -> std::shared_ptr<IAssetLoader>;

private:
  std::unordered_map<std::string, std::shared_ptr<IAssetLoader>> m_extensionToLoader;
  std::unordered_map<std::type_index, std::shared_ptr<IAssetLoader>> m_typeToLoader;
};
EGE_NAMESPACE_END