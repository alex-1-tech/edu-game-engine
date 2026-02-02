#pragma once

#include "engine/ecs/component.hpp"
#include "engine/graphics/types.hpp"

EGE_NAMESPACE_BEGIN


class Text : public Component<Text>
{
public:
  String content = "Entity";
  Color color = Color::white();
  f32 fontSize = 16.0F;
  Vec2 offset{0.0F, 0.0F}; // Смещение относительно позиции Transform

  [[nodiscard]] auto toString() const -> String override { return fmt::format("Text(\"{}\", size={:.1f})", content, fontSize); }
};
EGE_NAMESPACE_END