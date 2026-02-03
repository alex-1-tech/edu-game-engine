#pragma once

#include "engine/ecs/component.hpp"
#include "engine/graphics/types.hpp"

EGE_NAMESPACE_BEGIN

class Sprite : public Component<Sprite>
{
public:
  TextureId textureId = nullptr;
  Color tint = Color::white();
  Vec2 size{50.0F, 50.0F};

  Rect uvRect{0.0F, 0.0F, 1.0F, 1.0F};

  [[nodiscard]] auto toString() const -> String override
  {
    return fmt::format("Sprite(texture={}, size=({:.1f}, {:.1f}))", textureId ? "valid" : "null", size.x, size.y);
  }

  [[nodiscard]] auto explain() const -> String override
  {
    return fmt::format("Sprite Component:\n"
                       "├─ Texture: {}\n"
                       "├─ Size: ({:.1f}, {:.1f})\n"
                       "└─ Tint: RGBA({}, {}, {}, {})",
                       textureId ? "loaded" : "not loaded", size.x, size.y, tint.r(), tint.g(), tint.b(), tint.a());
  }
};

EGE_NAMESPACE_END