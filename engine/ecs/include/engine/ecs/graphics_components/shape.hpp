#pragma once

#include "engine/ecs/component.hpp"
#include "engine/graphics/types.hpp"

EGE_NAMESPACE_BEGIN

class Shape : public Component<Shape>
{
public:
  enum class Type : u8 { RECTANGLE, CIRCLE, LINE };

  Type type = Type::RECTANGLE;
  Color color = Color::white();
  bool filled = true;
  f32 outlineThickness = ONE_F;

  Vec2 size{50.0F, 50.0F};

  f32 radius = 25.0F;

  Vec2 endPoint{100.0F, 0.0F};

  [[nodiscard]] auto toString() const -> String override
  {
    const char* typeStr = "Unknown";
    switch (type) {
      case Type::RECTANGLE: typeStr = "Rectangle"; break;
      case Type::CIRCLE: typeStr = "Circle"; break;
      case Type::LINE: typeStr = "Line"; break;
    }

    return fmt::format("Shape(type={}, color=RGBA({},{},{},{}))", typeStr, color.r(), color.g(), color.b(), color.a());
  }
};

EGE_NAMESPACE_END