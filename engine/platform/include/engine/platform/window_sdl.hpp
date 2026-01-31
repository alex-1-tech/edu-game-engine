#pragma once

/**
 * @file    window_sdl.hpp
 * @brief   SDL-based window implementation for the engine
 * @author  alex-1-tech
 * @date    2026
 */

#include <SDL2/SDL.h>

#include "engine/platform/window.hpp"

EGE_NAMESPACE_BEGIN

/**
 * @brief SDL-based window implementation
 *
 * Implements the Window interface using the SDL2 library.
 * Handles window creation, event processing, and rendering context.
 */
class SDLWindow : public Window
{
public:
  /**
   * @brief Construct a new SDLWindow object
   * @param title Window title
   * @param width Window width in pixels
   * @param height Window height in pixels
   */
  SDLWindow(const String& title, u32 width, u32 height);
  ~SDLWindow() override;

  SDLWindow(const SDLWindow&) = delete;
  auto operator=(const SDLWindow&) -> SDLWindow& = delete;
  SDLWindow(SDLWindow&& other) noexcept;
  auto operator=(SDLWindow&& other) noexcept -> SDLWindow&;

  void update() override;
  [[nodiscard]] auto getWidth() const -> u32 override { return m_width; }
  [[nodiscard]] auto getHeight() const -> u32 override { return m_height; }
  [[nodiscard]] auto shouldClose() const -> bool override { return m_should_close; }

  /**
   * @brief Set the event callback function
   * @param callback Function to call when events occur
   */
  void setEventCallback(const EventCallbackFn& callback) override { m_event_callback = callback; }

private:
  SDL_Window* m_window = nullptr;   ///< SDL window handle
  u32 m_width;                      ///< Current window width
  u32 m_height;                     ///< Current window height
  bool m_should_close;              ///< Window close flag
  EventCallbackFn m_event_callback; ///< Event callback function

  /**
   * @brief Handle SDL event and convert to engine event
   * @param sdl_event SDL event to process
   */
  void handleSdlEvent(const SDL_Event& sdl_event);
};

EGE_NAMESPACE_END