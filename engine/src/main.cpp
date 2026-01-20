#include <SDL.h>

#include "engine/core/logging.hpp"
#include "engine/core/time.hpp"
#include "engine/platform/window.hpp"

namespace
{
constexpr engine::i32 WINDOW_WIDTH = 800;
constexpr engine::i32 WINDOW_HEIGHT = 600;
constexpr engine::f64 STATS_UPDATE_INTERVAL = 1.0;
constexpr engine::f64 MILLISECONDS_PER_SECOND = 1000.0;
constexpr engine::i32 MINIMAL_SDL_DELAY = 1;
constexpr engine::i32 EXIT_CODE_SUCCESS = 0;
constexpr engine::i32 EXIT_CODE_ERROR = 1;
} // namespace

auto main(int /*argc*/, char** /*argv*/) -> int
{
  using namespace engine;

  Logger::init();
  Logger::setLevel(LogLevel::DEBUG);

  EGE_INFO("Starting EduGame Engine v0.1.0");

  const char* platform =
#if defined(EGE_PLATFORM_WINDOWS)
      "Windows";
#elif defined(EGE_PLATFORM_LINUX)
      "Linux";
#elif defined(EGE_PLATFORM_MACOS)
      "macOS";
#else
      "Unknown";
#endif

  EGE_INFO("Platform: {}", platform);

  Time::init();

  auto window = Window::create("Hello", WINDOW_WIDTH, WINDOW_HEIGHT);

  if (window == nullptr) {
    EGE_ERROR("Failed to create window: {}", SDL_GetError());
    SDL_Quit();
    return EXIT_CODE_ERROR;
  }

  window->setEventCallback([](Event& event) { EGE_INFO("Event: {}", event.toString()); });

  EGE_INFO("Educational game engine initialized successfully");
  EGE_INFO("Press ESC to exit");

  f64 stats_timer = 0.0;
  u32 frames = 0;

  while (!window->shouldClose()) {
    Timer frame_timer("Frame");

    Time::update();
    window->update();

    {
      Timer update_timer("Update");
    }

    {
      Timer render_timer("Render");
    }

    stats_timer += Time::deltaTime();
    frames++;

    if (stats_timer >= STATS_UPDATE_INTERVAL) {
      const f64 fps = static_cast<double>(frames) / stats_timer;
      const f64 frame_time_ms = stats_timer * MILLISECONDS_PER_SECOND / static_cast<double>(frames);

      EGE_DEBUG("FPS: {:.1f}, Frame time: {:.3f}ms", fps, frame_time_ms);
      stats_timer = 0.0;
      frames = 0;
    }

    SDL_Delay(MINIMAL_SDL_DELAY);
  }

  EGE_INFO("Shutting down...");
  Logger::shutdown();

  return EXIT_CODE_SUCCESS;
}