#pragma once

/**
 * @file    window.hpp
 * @brief   Abstract window interface for platform-independent window management
 * @author  alex-1-tech
 * @date    2026
 */

#include <functional>

#include "engine/platform/events.hpp"

EGE_NAMESPACE_BEGIN

// Window configuration constants
constexpr u32 DEFAULT_WINDOW_WIDTH = 1280;                 ///< Default window width in pixels
constexpr u32 DEFAULT_WINDOW_HEIGHT = 720;                 ///< Default window height in pixels
constexpr const char* DEFAULT_WINDOW_TITLE = "Edu Engine"; ///< Default window title

/**
 * @brief Event callback function type
 *
 * Type for functions that handle engine events.
 * Used to forward events from the window system to the application.
 */
using EventCallbackFn = std::function<void(Event&)>;

/**
 * @brief Abstract window interface
 *
 * Defines the interface for platform-specific window implementations.
 * Provides methods for window management, event handling, and queries.
 */
class Window
{
public:
  Window() = default;
  Window(const Window&) = delete;
  auto operator=(const Window&) -> Window& = delete;
  Window(Window&&) = delete;
  auto operator=(Window&&) -> Window& = delete;
  virtual ~Window() = default;

  virtual void update() = 0;
  [[nodiscard]] virtual auto getWidth() const -> u32 = 0;
  [[nodiscard]] virtual auto getHeight() const -> u32 = 0;
  [[nodiscard]] virtual auto shouldClose() const -> bool = 0;

  /**
   * @brief Set the event callback function
   * @param callback Function to call when events occur
   */
  virtual void setEventCallback(const EventCallbackFn& callback) = 0;

  /**
   * @brief Create a platform-specific window
   *
   * Factory method that creates a window implementation based on the
   * current platform (SDL on desktop platforms).
   *
   * @param title Window title (default: "Edu Engine")
   * @param width Window width in pixels (default: 1280)
   * @param height Window height in pixels (default: 720)
   * @return Unique<Window> Unique pointer to the created window
   */
  static auto create(const String& title = DEFAULT_WINDOW_TITLE, u32 width = DEFAULT_WINDOW_WIDTH,
                     u32 height = DEFAULT_WINDOW_HEIGHT) -> Unique<Window>;
};

EGE_NAMESPACE_END