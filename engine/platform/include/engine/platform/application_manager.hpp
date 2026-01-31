#pragma once

/**
 * @file    application_manager.hpp
 * @brief   Manages application lifecycle and main game loop
 * @author  alex-1-tech
 * @date    2026
 */

#include "engine/core/config.hpp"
#include "engine/core/types.hpp"
#include "engine/platform/window_sdl.hpp"

EGE_NAMESPACE_BEGIN

class ApplicationManager
{
public:
  ApplicationManager();
  ~ApplicationManager();

  ApplicationManager(const ApplicationManager&) = delete;
  auto operator=(const ApplicationManager&) -> ApplicationManager& = delete;
  ApplicationManager(ApplicationManager&&) = delete;
  auto operator=(ApplicationManager&&) -> ApplicationManager& = delete;

  /// Lifecycle
  /// @{
  auto initialize() -> bool;
  void shutdown();
  /// }@

  /// Main loop control
  /// @{
  void run();         ///< Run until window closes
  void runOneFrame(); ///< Run single frame (for debugging)
  void stop();        ///< Request graceful stop
  /// }@

  /// Callback setters
  /// @{
  void setUpdateCallback(std::function<void(f64 deltaTime)> callback) { m_onUpdate = std::move(callback); }

  void setRenderCallback(std::function<void()> callback) { m_onRender = std::move(callback); }

  void setEventCallback(const EventCallbackFn& callback)
  {
    if (m_window) {
      m_window->setEventCallback(callback);
    }
  }
  /// }@

  /// Accessors
  auto getWindow() -> SDLWindow* { return m_window.get(); }
  [[nodiscard]] auto isRunning() const -> bool { return m_running; }

  struct ApplicationStats {
    // Real time
    f64 totalRunTime = 0.0;
    f64 lastFrameTime = 0.0;
    f64 avgFrameTime = 0.0;

    // FPS
    u32 fps = 0;
    f64 fpsTimer = 0.0;
    u32 fpsCounter = 0;

    // Update
    u32 fixedUpdatesThisFrame = 0;
    u64 totalFixedUpdates = 0;
  };

  [[nodiscard]] auto getStats() const -> const ApplicationStats& { return m_stats; }
  void printStats() const;

private:
  void runMainLoop();               ///< Internal main loop implementation
  void processFrame(f64 deltaTime); ///< Process a single frame (update, render)

  EngineConfig* m_config; ///< Engine configuration settings
  Unique<SDLWindow> m_window;    ///< Main application window
  bool m_running = false;        ///< Application running flag

  std::function<void(f64 deltaTime)> m_onUpdate; ///< User-defined update callback
  std::function<void()> m_onRender;              ///< User-defined render callback

  ApplicationStats m_stats; ///< Performance and runtime statistics
  u32 m_fpsCounter = 0;     ///< Frames counted in current FPS measurement interval
  f64 m_fpsTimer = 0.0;     ///< Timer for FPS calculation
};

EGE_NAMESPACE_END