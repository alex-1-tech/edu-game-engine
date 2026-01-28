#pragma once

#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"


#include <string>
#include <unordered_map>


EGE_NAMESPACE_BEGIN

struct ConfigDefaults {
  //WindowSettings
  static constexpr u32 WIDTH = 1280;
  static constexpr u32 HEIGHT = 720;
  static constexpr bool FULLSCREEN = false;
  static constexpr bool VSYNC = true;

  //GraphicsSettings
  static constexpr u32 MSAA = 4;
  static constexpr f32 ANISOTROPY = 8.0F;

  //LoggingSetting
  static constexpr LogLevel LOGLEVEL = LogLevel::INFO;

  //PerfomaceSettings
  static constexpr u32 TARGETFPS = 120;
  static constexpr bool CAPFPS = true;
};

class Config final{

  public:
    
    ~Config() = default;

    Config(Config&) = delete;           ///< Copy constructor deleted
    Config(Config&&) = delete;          ///< Move constructor deleted
    auto operator=(Config&) = delete;   ///< Copy assignment deleted
    auto operator=(Config&&) = delete;  ///< Move assignment deleted
 
    static auto getInstance() -> Config const& 
    {
      static Config instance;
      return instance;
    }     
    
    [[nodiscard]] auto getWindowWidth() const -> u32 {return window.width;}
    [[nodiscard]] auto getWindowHeight() const -> u32 {return window.height;}
    [[nodiscard]] auto getWindowTitle() const -> String {return window.title;}
    [[nodiscard]] auto getLoggingLevel() const -> LogLevel {return logging.level;}
    [[nodiscard]] auto getTargetFPS() const -> u32 {return perfomance.targetFPS;}
    
  
  private:

    Config();

    auto stringToLogLevel(String& level) -> LogLevel;

    struct WindowSettings {
      String title = "EduGame Engine";
      u32 width = ConfigDefaults::WIDTH;
      u32 height = ConfigDefaults::HEIGHT;
      bool fullscreen = ConfigDefaults::FULLSCREEN;
      bool vsync = ConfigDefaults::VSYNC;
    } window;

    struct GraphicsSettings {
      u32 msaaSamples = ConfigDefaults::MSAA;
      f32 maxAnisotropy = ConfigDefaults::ANISOTROPY;
    } graphics;

    struct LoggingSetting {
      LogLevel level = ConfigDefaults::LOGLEVEL;
    } logging;

    struct PerfomanceSettings{
      u32 targetFPS = ConfigDefaults::TARGETFPS;
      bool capFPS = ConfigDefaults::CAPFPS;
    } perfomance;

    const std::unordered_map<std::string, LogLevel> stringToLevel = {
        {"TRACE",    LogLevel::TRACE},
        {"DEBUG",    LogLevel::DEBUG},
        {"INFO",     LogLevel::INFO},
        {"WARNING",  LogLevel::WARNING},
        {"ERROR",    LogLevel::ERROR},
        {"CRITICAL", LogLevel::CRITICAL}
    };

};

EGE_NAMESPACE_END