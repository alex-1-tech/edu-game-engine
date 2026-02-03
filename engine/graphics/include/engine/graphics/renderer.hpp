#pragma once

/**
 * @file    render.hpp
 * @brief   Abstract renderer interface
 * @author  alex-1-tech
 * @date    2026
 */


#include "engine/graphics/types.hpp"

EGE_NAMESPACE_BEGIN
class Renderer
{
public:
  Renderer() = default;
  Renderer(const Renderer&) = default;
  auto operator=(const Renderer&) -> Renderer& = default;
  Renderer(Renderer&&) = default;
  auto operator=(Renderer&&) -> Renderer& = default;

  virtual ~Renderer() = default;

  virtual void clear(const Color& color = Color::black()) = 0;                                         ///< Clear screen
  virtual void present() = 0;                                                                          ///< Display rendered frame
  virtual void drawRect(const Rect& rect, const Color& color) = 0;                                     ///< Draw filled rectangle
  virtual void drawRectOutline(const Rect& rect, const Color& color, f32 thickness = ONE_F) = 0;       ///< Draw rectangle outline
  virtual void drawCircle(const Circle& circle, const Color& color) = 0;                               ///< Draw filled circle
  virtual void drawCircleOutline(const Circle& circle, const Color& color, f32 thickness = ONE_F) = 0; ///< Draw circle outline
  virtual void drawLine(const Line& line, const Color& color) = 0;                                     ///< Draw line
  virtual void drawLine(const Vec2& start, const Vec2& end, const Color& color,
                        f32 thickness = ONE_F) = 0;                                    ///< Draw line (convenient overload)
  virtual void drawPoint(const Vec2& point, const Color& color, f32 size = ONE_F) = 0; ///< Draw point
  virtual auto loadTexture(const String& path) -> TextureId = 0;                       ///< Load texture from file
  virtual void drawTexture(TextureId texture, const Rect& dest, const Color& tint = Color::white()) = 0; ///< Draw texture
  virtual void drawTexture(TextureId texture, const Rect& src, const Rect& dest,
                           const Color& tint = Color::white()) = 0;                        ///< Draw texture region (sprite sheet)
  virtual void unloadTexture(TextureId texture) = 0;                                       ///< Unload texture
  virtual void drawText(const String& text, const Vec2& position, const Color& color) = 0; ///< Draw text (basic)
  [[nodiscard]] virtual auto getViewportSize() const -> Vec2 = 0;                          ///< Get viewport size
  virtual void setViewport(const Rect& viewport) = 0;                                      ///< Set viewport region
  virtual void resetViewport() = 0;                                                        ///< Reset viewport to full screen
  virtual void pushState() = 0;                                                            ///< Save current renderer state
  virtual void popState() = 0;                                                             ///< Restore previous renderer state
};
EGE_NAMESPACE_END