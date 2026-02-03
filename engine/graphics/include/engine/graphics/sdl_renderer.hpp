#pragma once

#include <SDL2/SDL.h>

#include "engine/graphics/renderer.hpp"
#include "engine/platform/window_sdl.hpp"

EGE_NAMESPACE_BEGIN

class SDLRenderer : public Renderer
{
public:
  explicit SDLRenderer(SDLWindow* window);
  ~SDLRenderer() override;

  SDLRenderer(const SDLRenderer&) = delete;
  auto operator=(const SDLRenderer&) -> SDLRenderer& = delete;
  SDLRenderer(SDLRenderer&& other) noexcept;
  auto operator=(SDLRenderer&& other) noexcept -> SDLRenderer&;

  void clear(const Color& color = Color::black()) override;
  void present() override;

  void drawRect(const Rect& rect, const Color& color) override;
  void drawRectOutline(const Rect& rect, const Color& color, f32 thickness = ONE_F) override;

  void drawCircle(const Circle& circle, const Color& color) override;
  void drawCircleOutline(const Circle& circle, const Color& color, f32 thickness = ONE_F) override;

  void drawLine(const Line& line, const Color& color) override;
  void drawLine(const Vec2& start, const Vec2& end, const Color& color, f32 thickness = ONE_F) override;

  void drawPoint(const Vec2& point, const Color& color, f32 size = ONE_F) override;

  auto loadTexture(const String& path) -> TextureId override;
  void drawTexture(TextureId texture, const Rect& dest, const Color& tint = Color::white()) override;
  void drawTexture(TextureId texture, const Rect& src, const Rect& dest, const Color& tint = Color::white()) override;
  void unloadTexture(TextureId texture) override;

  void drawText(const String& text, const Vec2& position, const Color& color) override;

  [[nodiscard]] auto getViewportSize() const -> Vec2 override;
  void setViewport(const Rect& viewport) override;
  void resetViewport() override;

  void pushState() override;
  void popState() override;

  [[nodiscard]] auto getSDLRenderer() const -> SDL_Renderer* { return m_renderer; }

  [[nodiscard]] auto isValid() const -> bool { return m_renderer != nullptr; }

private:
  SDL_Renderer* m_renderer = nullptr;
  SDLWindow* m_window = nullptr;

  struct RenderState {
    Color drawColor;
    Rect viewport;
    SDL_RendererFlip flip;
    f32 rotation{};
  };
  std::vector<RenderState> m_stateStack;

  void setSDLDrawColor(const Color& color);
  static auto toSDLRect(const Rect& rect) -> SDL_Rect;
  static auto toSDLColor(const Color& color) -> SDL_Color;
};

EGE_NAMESPACE_END