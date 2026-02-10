#pragma once

/**
 * @file    components/shape.hpp
 * @brief   Shape component for primitive rendering
 * @author  alex-1-tech
 * @date    2026
 */

#include <fmt/format.h>

#include "engine/ecs/component.hpp"
#include "engine/graphics/types.hpp"

EGE_NAMESPACE_BEGIN
namespace BASE_SHAPE_VALUES
{
constexpr f32 OUTLINE_THICKNESS = 1.0F;
constexpr f32 SIZE_X = 50.0F;
constexpr f32 SIZE_Y = 50.0F;
constexpr f32 RADIUS = 25.0F;
constexpr f32 END_POINT_X = 100.0F;
constexpr f32 END_POINT_Y = 0.0F;
} // namespace BASE_SHAPE_VALUES

struct ShapeData {
  enum class Type : u8 { RECTANGLE, CIRCLE, LINE };

  Type type = Type::RECTANGLE;
  Color color = Color::white();
  bool filled = true;
  f32 outlineThickness = BASE_SHAPE_VALUES::OUTLINE_THICKNESS;

  // Rectangle data
  Vec2 size{BASE_SHAPE_VALUES::SIZE_X, BASE_SHAPE_VALUES::SIZE_Y};

  // Circle data
  f32 radius = BASE_SHAPE_VALUES::RADIUS;

  // Line data
  Vec2 endPoint{BASE_SHAPE_VALUES::END_POINT_X, BASE_SHAPE_VALUES::END_POINT_Y};
};

class Shape final : public Component<Shape>
{
public:
  Shape() = default;

  explicit Shape(ShapeData::Type type, const Color& color = Color::white(), bool filled = true)
      : m_data{.type = type, .color = color, .filled = filled}
  {
  }

  // Rectangle constructor
  static auto createRectangle(const Vec2& size, const Color& color = Color::white(), bool filled = true) -> Shape
  {
    Shape shape(ShapeData::Type::RECTANGLE, color, filled);
    shape.m_data.size = size;
    return shape;
  }

  // Circle constructor
  static auto createCircle(f32 radius, const Color& color = Color::white(), bool filled = true) -> Shape
  {
    Shape shape(ShapeData::Type::CIRCLE, color, filled);
    shape.m_data.radius = radius;
    return shape;
  }

  // Line constructor
  static auto createLine(const Vec2& endPoint, const Color& color = Color::white(), f32 thickness = ONE_F) -> Shape
  {
    Shape shape(ShapeData::Type::LINE, color, false);
    shape.m_data.endPoint = endPoint;
    shape.m_data.outlineThickness = thickness;
    return shape;
  }

  // Accessors
  [[nodiscard]] auto type() const -> ShapeData::Type { return m_data.type; }
  [[nodiscard]] auto color() const -> const Color& { return m_data.color; }
  [[nodiscard]] auto filled() const -> bool { return m_data.filled; }
  [[nodiscard]] auto outlineThickness() const -> f32 { return m_data.outlineThickness; }
  [[nodiscard]] auto size() const -> const Vec2& { return m_data.size; }
  [[nodiscard]] auto radius() const -> f32 { return m_data.radius; }
  [[nodiscard]] auto endPoint() const -> const Vec2& { return m_data.endPoint; }
  [[nodiscard]] auto data() const -> const ShapeData& { return m_data; }

  void setType(ShapeData::Type type) { m_data.type = type; }
  void setColor(const Color& color) { m_data.color = color; }
  void setFilled(bool filled) { m_data.filled = filled; }
  void setOutlineThickness(f32 thickness) { m_data.outlineThickness = thickness; }
  void setSize(const Vec2& size) { m_data.size = size; }
  void setRadius(f32 radius) { m_data.radius = radius; }
  void setEndPoint(const Vec2& endPoint) { m_data.endPoint = endPoint; }

  [[nodiscard]] auto toString() const -> String override
  {
    const char* typeStr = "Unknown";
    switch (m_data.type) {
      case ShapeData::Type::RECTANGLE: typeStr = "Rectangle"; break;
      case ShapeData::Type::CIRCLE: typeStr = "Circle"; break;
      case ShapeData::Type::LINE: typeStr = "Line"; break;
    }

    return fmt::format("Shape(type={}, color=RGBA({},{},{},{}))", typeStr, m_data.color.r(), m_data.color.g(), m_data.color.b(),
                       m_data.color.a());
  }

  [[nodiscard]] auto explain() const -> String override
  {
    String typeStr;
    String details;

    switch (m_data.type) {
      case ShapeData::Type::RECTANGLE:
        typeStr = "Rectangle";
        details = fmt::format("├─ Size: ({:.1f}, {:.1f})\n", m_data.size.x, m_data.size.y);
        break;
      case ShapeData::Type::CIRCLE:
        typeStr = "Circle";
        details = fmt::format("├─ Radius: {:.1f}\n", m_data.radius);
        break;
      case ShapeData::Type::LINE:
        typeStr = "Line";
        details = fmt::format("├─ End Point: ({:.1f}, {:.1f})\n", m_data.endPoint.x, m_data.endPoint.y);
        break;
    }

    return fmt::format("Shape Component:\n"
                       "├─ Type: {}\n"
                       "{}"
                       "├─ Filled: {}\n"
                       "├─ Outline Thickness: {:.1f}\n"
                       "└─ Color: RGBA({}, {}, {}, {})",
                       typeStr, details, m_data.filled ? "Yes" : "No", m_data.outlineThickness, m_data.color.r(), m_data.color.g(),
                       m_data.color.b(), m_data.color.a());
  }

private:
  ShapeData m_data;
};

EGE_NAMESPACE_END