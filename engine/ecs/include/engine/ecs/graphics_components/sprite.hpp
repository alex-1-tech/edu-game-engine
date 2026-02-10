#pragma once

/**
 * @file    components/sprite.hpp
 * @brief   Sprite component for textured rendering
 * @author  alex-1-tech
 * @date    2026
 */

#include <fmt/format.h>

#include "engine/ecs/component.hpp"
#include "engine/graphics/types.hpp"

EGE_NAMESPACE_BEGIN

namespace BASE_SPRITE_VALUES
{
constexpr f32 SIZE_X = 50.0F;
constexpr f32 SIZE_Y = 50.0F;
} // namespace BASE_SPRITE_VALUES

struct SpriteData {
  TextureId textureId = nullptr;
  Color tint = Color::white();
  Vec2 size{BASE_SPRITE_VALUES::SIZE_X, BASE_SPRITE_VALUES::SIZE_Y};
  Rect uvRect{ZERO_F, ZERO_F, ONE_F, ONE_F};
};

class Sprite final : public Component<Sprite>
{
public:
  Sprite() = default;

  explicit Sprite(TextureId texture, const Vec2& size = {BASE_SPRITE_VALUES::SIZE_X, BASE_SPRITE_VALUES::SIZE_Y},
                  const Color& tint = Color::white())
      : m_data{.textureId = texture, .tint = tint, .size = size}
  {
  }

  // Accessors
  [[nodiscard]] auto textureId() const -> TextureId { return m_data.textureId; }
  [[nodiscard]] auto tint() const -> const Color& { return m_data.tint; }
  [[nodiscard]] auto size() const -> const Vec2& { return m_data.size; }
  [[nodiscard]] auto uvRect() const -> const Rect& { return m_data.uvRect; }
  [[nodiscard]] auto data() const -> const SpriteData& { return m_data; }

  void setTextureId(TextureId texture) { m_data.textureId = texture; }
  void setTint(const Color& tint) { m_data.tint = tint; }
  void setSize(const Vec2& size) { m_data.size = size; }
  void setUvRect(const Rect& rect) { m_data.uvRect = rect; }

  [[nodiscard]] auto toString() const -> String override
  {
    return fmt::format("Sprite(texture={}, size=({:.1f}, {:.1f}))", (m_data.textureId != nullptr) ? "valid" : "null", m_data.size.x,
                       m_data.size.y);
  }

  [[nodiscard]] auto explain() const -> String override
  {
    return fmt::format("Sprite Component:\n"
                       "├─ Texture: {}\n"
                       "├─ Size: ({:.1f}, {:.1f})\n"
                       "├─ UV Rect: ({:.2f}, {:.2f}, {:.2f}, {:.2f})\n"
                       "└─ Tint: RGBA({}, {}, {}, {})",
                       (m_data.textureId != nullptr) ? "loaded" : "not loaded", m_data.size.x, m_data.size.y, m_data.uvRect.left(),
                       m_data.uvRect.right(), m_data.uvRect.width(), m_data.uvRect.height(), m_data.tint.r(), m_data.tint.g(),
                       m_data.tint.b(), m_data.tint.a());
  }

private:
  SpriteData m_data;
};

EGE_NAMESPACE_END