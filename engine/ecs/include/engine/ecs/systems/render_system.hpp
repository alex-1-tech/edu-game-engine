#pragma once

/**
 * @file    systems/render_system.hpp
 * @brief   Render system for drawing entities with graphical components
 * @author  alex-1-tech
 * @date    2026
 */

#include "engine/core/logging.hpp"
#include "engine/ecs/components/transform.hpp"
#include "engine/ecs/graphics_components/shape.hpp"
#include "engine/ecs/graphics_components/sprite.hpp"
#include "engine/ecs/graphics_components/text.hpp"
#include "engine/ecs/system.hpp"
#include "engine/graphics/renderer.hpp"

EGE_NAMESPACE_BEGIN

class RenderSystem : public System<Transform>
{
public:
  explicit RenderSystem(Renderer* renderer)
      : m_renderer(renderer)
  {
  }

  [[nodiscard]] auto getName() const -> String override { return "RenderSystem"; }

  [[nodiscard]] auto explain() const -> String override
  {
    return "Render System:\n"
           "Processes entities with Transform and any graphical component.\n"
           "Draws shapes, sprites, and text to the screen.\n"
           "Educational: Shows how rendering integrates with ECS";
  }

protected:
  void process(Entity entity, World& world, Transform& transform, float deltaTime) override
  {
    (void)deltaTime; // Not used for rendering

    if (!m_renderer) {
      return;
    }

    // Get graphical components
    Shape* shape = world.getComponent<Shape>(entity);
    Sprite* sprite = world.getComponent<Sprite>(entity);
    Text* text = world.getComponent<Text>(entity);

    // Save current renderer state
    m_renderer->pushState();

    // Draw shape if present
    if (shape) {
      drawShape(transform, *shape);
    }

    // Draw sprite if present
    if (sprite && sprite->textureId) {
      drawSprite(transform, *sprite);
    }

    // Draw text if present
    if (text) {
      drawText(transform, *text);
    }

    // Restore renderer state
    m_renderer->popState();
  }

private:
  Renderer* m_renderer;

  void drawShape(const Transform& transform, const Shape& shape)
  {
    const Vec2 position = transform.position();
    const f32 rotation = transform.rotation();
    const Vec2 scale = transform.scale();

    // Note: This is a simplified version - in a real implementation,
    // we would apply rotation and scale transformations
    (void)rotation;
    (void)scale;

    switch (shape.type) {
      case Shape::Type::RECTANGLE: {
        Rect rect(position - (shape.size * 0.5F), shape.size);
        if (shape.filled) {
          m_renderer->drawRect(rect, shape.color);
        } else {
          m_renderer->drawRectOutline(rect, shape.color, shape.outlineThickness);
        }
        break;
      }

      case Shape::Type::CIRCLE: {
        Circle circle(position, shape.radius);
        if (shape.filled) {
          m_renderer->drawCircle(circle, shape.color);
        } else {
          m_renderer->drawCircleOutline(circle, shape.color, shape.outlineThickness);
        }
        break;
      }

      case Shape::Type::LINE: {
        const Vec2 end = position + shape.endPoint;
        m_renderer->drawLine(position, end, shape.color, shape.outlineThickness);
        break;
      }
    }
  }

  void drawSprite(const Transform& transform, const Sprite& sprite)
  {
    const Vec2 position = transform.position();

    // Draw sprite at entity position
    const Vec2 halfSize = sprite.size * 0.5F;
    const Rect destRect(position - halfSize, sprite.size);

    // Draw the texture with tint color
    m_renderer->drawTexture(sprite.textureId, sprite.uvRect, destRect, sprite.tint);
  }

  void drawText(const Transform& transform, const Text& text)
  {
    // Draw text at entity position with offset
    const Vec2 textPos = transform.position() + text.offset;

    // Note: SDLRenderer currently doesn't implement drawText
    // For now, we'll draw a simple placeholder
    m_renderer->drawPoint(textPos, text.color, 3.0F);

    // In a real implementation, we would use:
    // m_renderer->drawText(text.content, textPos, text.color);
  }
};

EGE_NAMESPACE_END