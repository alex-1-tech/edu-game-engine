#pragma once

/**
 * @file    components/text.hpp
 * @brief   Text component for rendering text
 * @author  alex-1-tech
 * @date    2026
 */

#include <fmt/format.h>

#include "engine/ecs/component.hpp"
#include "engine/graphics/types.hpp"

EGE_NAMESPACE_BEGIN

namespace BASE_SHAPE_VALUES
{
constexpr f32 FONT_SIZE = 16.0F;
}

struct TextData {
  String content = "Entity";
  Color color = Color::white();
  f32 fontSize = BASE_SHAPE_VALUES::FONT_SIZE;
  Vec2 offset{0.0F, 0.0F}; // Offset relative to the Transform position
};

class Text final : public Component<Text>
{
public:
  Text() = default;

  explicit Text(const String& content, f32 fontSize = BASE_SHAPE_VALUES::FONT_SIZE, const Color& color = Color::white())
      : m_data{.content = content, .color = color, .fontSize = fontSize}
  {
  }

  // Accessors
  [[nodiscard]] auto content() const -> const String& { return m_data.content; }
  [[nodiscard]] auto color() const -> const Color& { return m_data.color; }
  [[nodiscard]] auto fontSize() const -> f32 { return m_data.fontSize; }
  [[nodiscard]] auto offset() const -> const Vec2& { return m_data.offset; }
  [[nodiscard]] auto data() const -> const TextData& { return m_data; }

  void setContent(const String& content) { m_data.content = content; }
  void setColor(const Color& color) { m_data.color = color; }
  void setFontSize(f32 size) { m_data.fontSize = size; }
  void setOffset(const Vec2& offset) { m_data.offset = offset; }

  [[nodiscard]] auto toString() const -> String override
  {
    return fmt::format("Text(\"{}\", size={:.1f})", m_data.content, m_data.fontSize);
  }

  [[nodiscard]] auto explain() const -> String override
  {
    return fmt::format("Text Component:\n"
                       "├─ Content: \"{}\"\n"
                       "├─ Font Size: {:.1f}\n"
                       "├─ Color: RGBA({}, {}, {}, {})\n"
                       "└─ Offset: ({:.1f}, {:.1f})",
                       m_data.content, m_data.fontSize, m_data.color.r(), m_data.color.g(), m_data.color.b(), m_data.color.a(),
                       m_data.offset.x, m_data.offset.y);
  }

private:
  TextData m_data;
};

EGE_NAMESPACE_END