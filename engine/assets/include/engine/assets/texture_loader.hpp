#pragma once

#include "engine/assets/asset_loader.hpp"
#include "engine/graphics/renderer.hpp"

EGE_NAMESPACE_BEGIN

class TextureLoader : public IAssetLoader
{
public:
  explicit TextureLoader(Renderer* renderer);
  ~TextureLoader() override = default;

  TextureLoader(const TextureLoader&) = delete;
  auto operator=(const TextureLoader&) -> TextureLoader& = delete;
  TextureLoader(TextureLoader&&) = delete;
  auto operator=(TextureLoader&&) -> TextureLoader& = delete;

  [[nodiscard]] auto supportsType(const std::type_index& type) const -> bool override;
  [[nodiscard]] auto getResourceType() const -> std::type_index override;

  auto loadSync(const Path& path) -> ResourceResult<Ref<void>> override;
  auto loadAsync(const Path& path, LoadPriority priority) -> ResourceFuture<void> override;

  void unload(void* resource) override;

  [[nodiscard]] auto getSupportedExtensions() const -> std::vector<String> override;

private:
  Renderer* m_renderer;

  auto loadTextureInternal(const Path& path) -> ResourceResult<TextureId>;
};

EGE_NAMESPACE_END