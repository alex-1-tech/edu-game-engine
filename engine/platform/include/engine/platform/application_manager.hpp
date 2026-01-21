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
  ApplicationManager();  ///<  constructor
  ~ApplicationManager(); ///<  deconstructor

  ApplicationManager(const ApplicationManager&) = delete;                    ///< Copy constructor deleted
  auto operator=(const ApplicationManager&) -> ApplicationManager& = delete; ///< Copy assignment deleted
  ApplicationManager(ApplicationManager&&) = delete;                         ///< Move constructor deleted
  auto operator=(ApplicationManager&&) -> ApplicationManager& = delete;      ///< Move assignment deleted

  // Lifecycle
  auto initialize() -> bool;
  void shutdown();

  // Main loop control
  void run();         ///< Run until window closes
  void runOneFrame(); ///< Run single frame (for debugging)
  void stop();        ///< Request graceful stop

  // Callback setters
  void setUpdateCallback(std::function<void(f64 deltaTime)> callback) { m_onUpdate = std::move(callback); }

  void setRenderCallback(std::function<void()> callback) { m_onRender = std::move(callback); }

  void setEventCallback(const EventCallbackFn& callback)
  {
    if (m_window) {
      m_window->setEventCallback(callback);
    }
  }

  // Accessors
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
  void runMainLoop();
  void processFrame(f64 deltaTime);

  std::unique_ptr<EngineConfig> m_config;
  std::unique_ptr<SDLWindow> m_window;
  bool m_running = false;

  // Callbacks
  std::function<void(f64 deltaTime)> m_onUpdate;
  std::function<void()> m_onRender;

  ApplicationStats m_stats;
  u32 m_fpsCounter = 0;
  f64 m_fpsTimer = 0.0;
};

EGE_NAMESPACE_END