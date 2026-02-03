#include "engine/assets/texture_loader.hpp"

#include "engine/core/logging.hpp"

EGE_NAMESPACE_BEGIN

TextureLoader::TextureLoader(Renderer* renderer)
    : m_renderer(renderer)
{
  EGE_ASSERT(m_renderer != nullptr, "Renderer must be valid");
}

auto TextureLoader::supportsType(const std::type_index& type) const -> bool
{
  return type == typeid(TextureId);
}

auto TextureLoader::getResourceType() const -> std::type_index
{
  return typeid(TextureId);
}

auto TextureLoader::loadSync(const Path& path) -> ResourceResult<std::shared_ptr<void>>
{
  try {
    auto result = loadTextureInternal(path);
    if (!std::holds_alternative<TextureId>(result)) {
      return String(std::get<String>(result));
    }

    TextureId texture = std::get<TextureId>(result);
    return std::shared_ptr<void>(texture, [this](void* tex) {
      if (tex) {
        m_renderer->unloadTexture(static_cast<TextureId>(tex));
      }
    });
  } catch (const std::exception& e) {
    return String(e.what());
  }
}

auto TextureLoader::loadAsync(const Path& path, LoadPriority /*priority*/) -> ResourceFuture<void>
{
  return std::async(std::launch::async, [this, path]() -> ResourceResult<Ref<void>> { return loadSync(path); });
}


void TextureLoader::unload(void* resource)
{
  if ((resource != nullptr) && (m_renderer != nullptr)) {
    m_renderer->unloadTexture(static_cast<TextureId>(resource));
  }
}

auto TextureLoader::getSupportedExtensions() const -> std::vector<String>
{
  return {".png", ".jpg", ".jpeg", ".bmp", ".tga", ".dds"};
}


auto TextureLoader::loadTextureInternal(const Path& path) -> ResourceResult<TextureId>
{
  if (m_renderer == nullptr) {
    return String("Renderer is not initialized");
  }

  if (!std::filesystem::exists(path)) {
    return String("File does not exist: ") + path.string();
  }

  TextureId texture = m_renderer->loadTexture(path.string());
  if (texture == nullptr) {
    return String("Failed to load texture: ") + path.string();
  }

  EGE_INFO("Loaded texture: {}", path.string());
  return texture;
}
EGE_NAMESPACE_END