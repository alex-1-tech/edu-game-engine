#include "engine/platform/application_manager.hpp"

#include "engine/core/config.hpp"
#include "engine/core/logging.hpp"
#include "engine/core/time.hpp"
#include "engine/platform/window_sdl.hpp"

EGE_NAMESPACE_BEGIN

ApplicationManager::ApplicationManager()
    : m_config(Config::getInstance())
{
  m_config.loadFromFile(JSONConfigLoader{}, String(Config::ConfigDefaults::PATH_TO_CONFIG));
  EGE_DEBUG("ApplicationManager created: {} ({}x{})", m_config.getParametr<String>(Config::Param::WINDOW_TITLE),
  m_config.getParametr<u32>(Config::Param::WINDOW_WIDTH), m_config.getParametr<u32>(Config::Param::WINDOW_HEIGHT));}

  ApplicationManager::~ApplicationManager()
{
  shutdown();
}

auto ApplicationManager::initialize() -> bool
{
  
  Logger::setLevel(m_config.getParametr<LogLevel>(Config::Param::LOGLEVEL));

  EGE_INFO("Initializing application...");
  m_window =
      std::make_unique<SDLWindow>(m_config.getParametr<String>(Config::Param::WINDOW_TITLE),
   m_config.getParametr<u32>(Config::Param::WINDOW_WIDTH), m_config.getParametr<u32>(Config::Param::WINDOW_HEIGHT));

  Time::init();

  m_running = true;

  //Trying to save new confing [DEBUG PURPOSE ONLY]/////////////////////////////////
  m_config.setParametr(Config::Param::LOGLEVEL, LogLevel::WARNING);
  m_config.setParametr(Config::Param::WINDOW_TITLE, String("NEW TITLE"));
  u32 const new_size = 1234;
  m_config.setParametr(Config::Param::WINDOW_WIDTH, new_size);
  bool result = m_config.saveToFile(JSONConfigLoader{}, Config::ConfigDefaults::PATH_TO_CONFIG);
  ///////////////////////////////////////////////////////////////////////////////////

  return true;
}

void ApplicationManager::shutdown()
{
  if (!m_running) {
    return;
  }

  EGE_INFO("Shutting down application...");
  m_running = false;
  m_window.reset();
  EGE_INFO("Application shutdown complete");
}

void ApplicationManager::run()
{
  if (!m_running && !initialize()) {
    EGE_ERROR("Failed to initialize application");
    return;
  }

  EGE_INFO("Starting main application loop...");

  while (m_running && !m_window->shouldClose()) {
    runMainLoop();
  }

  EGE_INFO("Application loop ended");
  printStats();
}

constexpr f64 ONE_SECOND = 1.0;

void ApplicationManager::runMainLoop()
{
  auto start = Time::Clock::now();
  Time::update();
  m_window->update();

  // ===== FIXED UPDATE =====
  u32 fixedUpdateCount = 0;
  while (Time::hasFixedStep() && fixedUpdateCount < MAX_FIXED_UPDATES_PER_FRAME) {
    if (m_onUpdate) {
      m_onUpdate(Time::fixedDeltaTime());
    }

    Time::consumeFixedStep();
    ++fixedUpdateCount;
  }

  if (m_onRender) {
    m_onRender();
  }

  // ===== FRAME RATE LIMIT =====
  const f64 target = 1.0 / m_config.getParametr<u32>(Config::Param::TARGET_FPS);
  const f64 frameTime = Time::Duration(Time::Clock::now() - start).count();

  if (frameTime < target) {
    SDL_Delay(static_cast<u32>((target - frameTime) * MILLISECONDS_PER_SECOND));
  }
  auto end = Time::Clock::now();
  auto realFrameTime = Time::Duration(end - start).count();
  Time::onFrameRendered();

  // ===== STATISTICS =====
  m_stats.fixedUpdatesThisFrame = fixedUpdateCount;
  m_stats.totalFixedUpdates += fixedUpdateCount;
  m_stats.lastFrameTime = realFrameTime;
  m_stats.totalRunTime += realFrameTime;
  m_stats.avgFrameTime = m_stats.totalRunTime / static_cast<f64>(Time::frameCount());

  // ====== real FPS ======
  m_stats.fpsCounter++;
  m_stats.fpsTimer += realFrameTime;

  if (m_stats.fpsTimer >= ONE_SECOND) {
    m_stats.fps = m_stats.fpsCounter;
    m_stats.fpsCounter = 0;
    m_stats.fpsTimer -= ONE_SECOND;
  }
}


void ApplicationManager::runOneFrame()
{
  if (!m_running && !initialize()) {
    return;
  }

  runMainLoop();

  EGE_EXPLAIN("Single frame executed:");
  EGE_EXPLAIN("  Delta time: {:.3f}s", Time::deltaTime());
  EGE_EXPLAIN("  Scaled delta time: {:.3f}s", Time::scaledDeltaTime());
  EGE_EXPLAIN("  Frame time: {:.1f}ms", m_stats.lastFrameTime * MILLISECONDS_PER_SECOND);
  EGE_EXPLAIN("  Fixed updates this frame: {}", m_stats.fixedUpdatesThisFrame);
  EGE_EXPLAIN("  Total frames: {}", Time::frameCount());
}


void ApplicationManager::printStats() const
{
  String stats = fmt::format("📊 Application Statistics:\n"
                             "├─ Runtime: {:.2f}s\n"
                             "├─ Frames: {}\n"
                             "├─ FPS (last): {}\n"
                             "├─ Avg Frame Time: {:.2f}ms\n"
                             "├─ Total Fixed Updates: {}\n"
                             "└─ Window: {} ({}x{})\n",
                             m_stats.totalRunTime, Time::frameCount(), m_stats.fps,
                             m_stats.avgFrameTime * MILLISECONDS_PER_SECOND, m_stats.totalFixedUpdates,
                             m_config.getParametr<String>(Config::Param::WINDOW_TITLE),
                             m_config.getParametr<u32>(Config::Param::WINDOW_WIDTH),
                             m_config.getParametr<u32>(Config::Param::WINDOW_HEIGHT));

  EGE_INFO("{}", stats);
}

void ApplicationManager::stop()
{
  m_running = false;
}

void ApplicationManager::processFrame(f64 deltaTime)
{
  if (m_onUpdate) {
    m_onUpdate(deltaTime);
  }
  if (m_onRender) {
    m_onRender();
  }
}


EGE_NAMESPACE_END