#include <SDL.h>

#include "engine/core/logging.hpp"
#include "engine/platform/application_manager.hpp"


auto main(int /*argc*/, char** /*argv*/) -> int
{
  using namespace engine;

  Logger::init();
  Logger::setLevel(LogLevel::DEBUG);

  EGE_INFO("Starting EduGame Engine v0.1.0");


#if defined(EGE_PLATFORM_WINDOWS)
  EGE_INFO("Platform: Windows");
#elif defined(EGE_PLATFORM_LINUX)
  EGE_INFO("Platform: Linux");
#elif defined(EGE_PLATFORM_MACOS)
  EGE_INFO("Platform: macOS");
#else
  EGE_INFO("Platform: Unknown");
#endif

  ApplicationManager app;

  app.setUpdateCallback([](f64 delta) {});

  app.setRenderCallback([]() {});

  app.setEventCallback([](Event& event) { EGE_INFO("Event: {}", event.toString()); });

  app.run();

  Logger::shutdown();
  return 0;
}