#include "engine/graphics/sdl_renderer.hpp"

#include <SDL_image.h>

#include "engine/core/logging.hpp"

EGE_NAMESPACE_BEGIN

SDLRenderer::SDLRenderer(SDLWindow* window)
    : m_window(window)
{
  if (window == nullptr) {
    EGE_ERROR("Cannot create SDLRenderer: window is null");
    return;
  }

  m_renderer = SDL_CreateRenderer(window->getSDLWindow(), IVALID_INDEX, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (m_renderer == nullptr) {
    EGE_ERROR("Failed to create SDL renderer: {}", SDL_GetError());
    return;
  }

  SDL_RenderSetLogicalSize(m_renderer, static_cast<int>(window->getWidth()), static_cast<int>(window->getHeight()));
  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
  EGE_INFO("SDLRenderer initialized successfully");
}

SDLRenderer::~SDLRenderer()
{
  if (m_renderer != nullptr) {
    SDL_DestroyRenderer(m_renderer);
    EGE_DEBUG("SDLRenderer destroyed");
  }
}

SDLRenderer::SDLRenderer(SDLRenderer&& other) noexcept
    : m_renderer(other.m_renderer)
    , m_window(other.m_window)
{
  other.m_renderer = nullptr;
  other.m_window = nullptr;
}

auto SDLRenderer::operator=(SDLRenderer&& other) noexcept -> SDLRenderer&
{
  if (this != &other) {
    if (m_renderer != nullptr) {
      SDL_DestroyRenderer(m_renderer);
    }
    m_renderer = other.m_renderer;
    m_window = other.m_window;
    m_stateStack = std::move(other.m_stateStack);

    other.m_renderer = nullptr;
    other.m_window = nullptr;
  }
  return *this;
}

void SDLRenderer::clear(const Color& color)
{
  if (m_renderer == nullptr) {
    return;
  }
  setSDLDrawColor(color);
  SDL_RenderClear(m_renderer);
}

void SDLRenderer::present()
{
  if (m_renderer == nullptr) {
    return;
  }
  SDL_RenderPresent(m_renderer);
}

void SDLRenderer::drawRect(const Rect& rect, const Color& color)
{
  if (m_renderer == nullptr) {
    return;
  }
  setSDLDrawColor(color);
  SDL_Rect sdlRect = toSDLRect(rect);
  SDL_RenderFillRect(m_renderer, &sdlRect);
}

void SDLRenderer::drawRectOutline(const Rect& rect, const Color& color, f32 thickness)
{
  if ((m_renderer == nullptr) || thickness <= 0.0F) {
    return;
  }
  setSDLDrawColor(color);

  const i32 thick = static_cast<i32>(thickness);
  SDL_Rect sdlRect = toSDLRect(rect);

  SDL_Rect top = {sdlRect.x, sdlRect.y, sdlRect.w, thick};
  SDL_Rect bottom = {sdlRect.x, sdlRect.y + sdlRect.h - thick, sdlRect.w, thick};
  SDL_Rect left = {sdlRect.x, sdlRect.y + thick, thick, sdlRect.h - (DOUBLE * thick)};
  SDL_Rect right = {sdlRect.x + sdlRect.w - thick, sdlRect.y + thick, thick, sdlRect.h - (DOUBLE * thick)};

  SDL_RenderFillRect(m_renderer, &top);
  SDL_RenderFillRect(m_renderer, &bottom);
  SDL_RenderFillRect(m_renderer, &left);
  SDL_RenderFillRect(m_renderer, &right);
}

void SDLRenderer::drawCircle(const Circle& circle, const Color& color)
{
  if (m_renderer == nullptr) {
    return;
  }
  setSDLDrawColor(color);

  const i32 radius = static_cast<i32>(circle.radius());
  const i32 centerX = static_cast<i32>(circle.center().x);
  const i32 centerY = static_cast<i32>(circle.center().y);

  for (i32 dy = -radius; dy <= radius; ++dy) {
    for (i32 dx = -radius; dx <= radius; ++dx) {
      if (dx * dx + dy * dy <= radius * radius) {
        SDL_RenderDrawPoint(m_renderer, centerX + dx, centerY + dy);
      }
    }
  }
}

void SDLRenderer::drawCircleOutline(const Circle& circle, const Color& color, f32 thickness)
{
  if ((m_renderer == nullptr) || thickness <= 0.0F) {
    return;
  }
  setSDLDrawColor(color);

  const i32 radius = static_cast<i32>(circle.radius());
  const i32 centerX = static_cast<i32>(circle.center().x);
  const i32 centerY = static_cast<i32>(circle.center().y);

  const i32 outerRadius = radius + static_cast<i32>(thickness / 2);
  const i32 innerRadius = std::max(0, radius - static_cast<i32>(thickness / 2));

  for (i32 dy = -outerRadius; dy <= outerRadius; ++dy) {
    for (i32 dx = -outerRadius; dx <= outerRadius; ++dx) {
      const i32 distSquared = (dx * dx) + (dy * dy);
      if (distSquared <= outerRadius * outerRadius && distSquared >= innerRadius * innerRadius) {
        SDL_RenderDrawPoint(m_renderer, centerX + dx, centerY + dy);
      }
    }
  }
}

void SDLRenderer::drawLine(const Line& line, const Color& color)
{
  drawLine(line.start(), line.end(), color, ONE_F);
}

void SDLRenderer::drawLine(const Vec2& start, const Vec2& end, const Color& color, f32 thickness)
{
  if ((m_renderer == nullptr) || thickness <= 0.0F) {
    return;
  }
  setSDLDrawColor(color);

  if (thickness == ONE_F) {
    SDL_RenderDrawLineF(m_renderer, start.x, start.y, end.x, end.y);
  } else {
    const Vec2 dir = glm::normalize(end - start);
    const Vec2 perp = Vec2(-dir.y, dir.x) * (thickness * 0.5F);

    SDL_RenderDrawLineF(m_renderer, start.x - perp.x, start.y - perp.y, end.x - perp.x, end.y - perp.y);
    SDL_RenderDrawLineF(m_renderer, start.x + perp.x, start.y + perp.y, end.x + perp.x, end.y + perp.y);
  }
}

void SDLRenderer::drawPoint(const Vec2& point, const Color& color, f32 size)
{
  if ((m_renderer == nullptr) || size <= 0.0F) {
    return;
  }
  setSDLDrawColor(color);

  if (size == ONE_F) {
    SDL_RenderDrawPointF(m_renderer, point.x, point.y);
  } else {
    const f32 halfSize = size * 0.5F;
    const Rect rect = {point - halfSize, Vec2(size, size)};
    drawRect(rect, color);
  }
}

auto SDLRenderer::loadTexture(const String& path) -> TextureId
{
  if (m_renderer == nullptr) {
    return nullptr;
  }

  SDL_Texture* texture = IMG_LoadTexture(m_renderer, path.c_str());
  if (texture == nullptr) {
    EGE_ERROR("Failed to load texture {}: {}", path, SDL_GetError());
  }
  return static_cast<TextureId>(texture);
}

void SDLRenderer::drawTexture(TextureId texture, const Rect& dest, const Color& tint)
{
  drawTexture(texture, Rect(), dest, tint);
}

void SDLRenderer::drawTexture(TextureId texture, const Rect& src, const Rect& dest, const Color& tint)
{
  if ((m_renderer == nullptr) || (texture == nullptr)) {
    return;
  }

  auto* sdlTexture = static_cast<SDL_Texture*>(texture);
  SDL_Rect sdlSrc = toSDLRect(src);
  SDL_Rect sdlDest = toSDLRect(dest);

  SDL_SetTextureColorMod(sdlTexture, tint.r(), tint.g(), tint.b());
  SDL_SetTextureAlphaMod(sdlTexture, tint.a());
  SDL_RenderCopy(m_renderer, sdlTexture, &sdlSrc, &sdlDest);
}

void SDLRenderer::unloadTexture(TextureId texture)
{
  if (texture != nullptr) {
    SDL_DestroyTexture(static_cast<SDL_Texture*>(texture));
  }
}

void SDLRenderer::drawText(const String& /*text*/, const Vec2& /*position*/, const Color& /*color*/)
{
  if (m_renderer == nullptr) {
    return;
  }
  EGE_WARN("Text drawing not implemented");
}

auto SDLRenderer::getViewportSize() const -> Vec2
{
  if (m_renderer == nullptr) {
    return {};
  }

  i32 width = 0;
  i32 height = 0;
  SDL_RenderGetLogicalSize(m_renderer, &width, &height);
  return {static_cast<f32>(width), static_cast<f32>(height)};
}

void SDLRenderer::setViewport(const Rect& viewport)
{
  if (m_renderer == nullptr) {
    return;
  }
  SDL_Rect sdlRect = toSDLRect(viewport);
  SDL_RenderSetViewport(m_renderer, &sdlRect);
}

void SDLRenderer::resetViewport()
{
  if (m_renderer == nullptr) {
    return;
  }
  SDL_RenderSetViewport(m_renderer, nullptr);
}

void SDLRenderer::pushState()
{
  if (m_renderer == nullptr) {
    return;
  }

  RenderState state;
  SDL_GetRenderDrawColor(m_renderer, &state.drawColor.rRef(), &state.drawColor.gRef(), &state.drawColor.bRef(), &state.drawColor.aRef());

  SDL_Rect viewport;
  SDL_RenderGetViewport(m_renderer, &viewport);
  state.viewport =
      Rect(static_cast<f32>(viewport.x), static_cast<f32>(viewport.y), static_cast<f32>(viewport.w), static_cast<f32>(viewport.h));

  m_stateStack.push_back(state);
}

void SDLRenderer::popState()
{
  if ((m_renderer == nullptr) || m_stateStack.empty()) {
    return;
  }

  const RenderState& state = m_stateStack.back();
  setSDLDrawColor(state.drawColor);
  setViewport(state.viewport);

  m_stateStack.pop_back();
}

auto SDLRenderer::toSDLRect(const Rect& rect) -> SDL_Rect
{
  return {static_cast<int>(rect.position().x), static_cast<int>(rect.position().y), static_cast<int>(rect.size().x),
          static_cast<int>(rect.size().y)};
}

auto SDLRenderer::toSDLColor(const Color& color) -> SDL_Color
{
  return {color.r(), color.g(), color.b(), color.a()};
}

void SDLRenderer::setSDLDrawColor(const Color& color)
{
  SDL_SetRenderDrawColor(m_renderer, color.r(), color.g(), color.b(), color.a());
}

EGE_NAMESPACE_END