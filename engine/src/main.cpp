#include "engine/core/logging.hpp"
#include "engine/core/math.hpp"
#include "engine/core/time.hpp"
#include <SDL.h>
#include <iostream>

int main(int argc, char *argv[]) {
  using namespace engine;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
    std::cerr << "SDL initialization failed: " << SDL_GetError() << '\n';
    return 1;
  }

  Logger::init();
  Logger::set_level(LogLevel::Debug);
  EGE_INFO("Starting EduGame Engine v0.1.0");

  const char *platform =
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

  SDL_Window *window = SDL_CreateWindow(
      "EduGame Engine Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (window == nullptr) {
    EGE_ERROR("Failed to create window: {}", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  Vec3 a(1.0F, 0.0F, 0.0F);
  Vec3 b(0.0F, 1.0F, 0.0F);
  edu_math::explain_dot_product(a, b);
  edu_math::explain_cross_product(a, b);

  EGE_INFO("Educational game engine initialized successfully");
  EGE_INFO("Press ESC to exit");

  bool running = true;
  while (running) {
    Timer frame_timer("Frame");

    Time::update();

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        running = false;
      } else if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
          running = false;
        } else if (event.key.keysym.sym == SDLK_SPACE) {
          EGE_EXPLAIN("Space pressed! Delta time: {:.6f}s", Time::delta_time());
        }
      }
    }

    {
      Timer update_timer("Update");
    }

    {
      Timer render_timer("Render");
    }

    static f64 stats_timer = 0.0;
    static u32 frames = 0;
    stats_timer += Time::delta_time();
    frames++;

    if (stats_timer >= 1.0) {
      EGE_DEBUG("FPS: {:.1f}, Frame time: {:.3f}ms", frames / stats_timer,
                stats_timer * 1000.0 / frames);
      stats_timer = 0.0;
      frames = 0;
    }

    SDL_Delay(1);
  }

  EGE_INFO("Shutting down...");
  SDL_DestroyWindow(window);
  SDL_Quit();
  Logger::shutdown();

  return 0;
}