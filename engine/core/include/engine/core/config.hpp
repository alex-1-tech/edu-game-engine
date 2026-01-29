#pragma once

#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"

#include <string>
#include <unordered_map>


EGE_NAMESPACE_BEGIN

class Config final{

  public:
    enum class Param : u8 {
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      WINDOW_TITLE,
      TARGET_FPS,
      LOGLEVEL
    };
    
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

    template<typename T>
    [[nodiscard]] auto getParametr(Param confParam) const -> T {
        switch (confParam) {
            case Param::WINDOW_WIDTH:
                if constexpr (std::is_same_v<T, u32>){
                    return window.width;
                }
                break;
            case Param::WINDOW_HEIGHT:
                if constexpr (std::is_same_v<T, u32>){
                    return window.height;
                }
                break;
            case Param::WINDOW_TITLE:
                if constexpr (std::is_same_v<T, String>){
                    return window.title;
                }
                break;
            case Param::TARGET_FPS:
                if constexpr (std::is_same_v<T, u32>){
                    return perfomance.targetFPS;
                }
                break;
            case Param::LOGLEVEL:
                if constexpr (std::is_same_v<T, LogLevel>){
                    return logging.level;
                }
                break;
        }
        throw std::runtime_error("EGE::Config: Type mismatch or unknown Param");
    } 

    template <typename T>
    void setParametr(Param confParam, const T& value){
        switch (confParam) {
            case Param::WINDOW_HEIGHT:
                if constexpr (std::is_same_v<T, u32>) {
                    applyIfValid(this->window.height, value, 
                        ConfigDefaults::MIN_HEIGHT,ConfigDefaults::MAX_HEIGHT, ConfigDefaults::HEIGHT);}
                break;
            case Param::WINDOW_WIDTH:
                if constexpr (std::is_same_v<T, u32>) {
                    EGE_DEBUG("applyIfValid will call", this->window.width);
                    applyIfValid(this->window.width, value,
                        ConfigDefaults::MIN_WIDTH,ConfigDefaults::MAX_WIDTH, ConfigDefaults::WIDTH);}
                break;
            case Param::WINDOW_TITLE:
                if constexpr (std::is_same_v<T, String>) {
                    applyIfValidString(this->window.title, value, ConfigDefaults::MIN_TITLE_LEN,
                        ConfigDefaults::MAX_TITLE_LEN, String("EduGame Engine"));
                }
                break;
            case Param::TARGET_FPS:
                if constexpr (std::is_same_v<T, u32>) {
                    applyIfValid(this->perfomance.targetFPS, value, ConfigDefaults::MIN_TFPS,
                        ConfigDefaults::MAX_TFPS, ConfigDefaults::TARGETFPS);}
                break;
            case Param::LOGLEVEL: 
                if constexpr (std::is_same_v<T, LogLevel>) {
                    this->logging.level = value;}
                break;
        }
      //  saveConfig();
    }
    
  private:

    Config();
    void loadConfig();
    //void saveConfig();
    auto stringToLogLevel(String& level) -> LogLevel;

    template<typename T>
    static auto applyIfValid(T& field, const T& value, T min, T max, const T defaultVal) -> bool{
        if (min < value && value < max){
            field = value;
            return true;
        }
        EGE_WARN("Validation failed: value out of bounds, setting up default value");
        field = defaultVal;
        return false;
    }

    template<typename T, typename P>
    static auto applyIfValidString(T& field, const T& value, P min, P max, T defaultVal) -> bool{
        u16 len = value.length();
        if (min < len && len < max){
            field = value;
            return true;
        }
        EGE_WARN("Validation failed: value out of bounds, setting up default value");
        field = defaultVal;
        return false;
    }

    struct WindowSettings {
      String title = ConfigDefaults::TITLE;
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

    struct ConfigDefaults {
    //WindowSettings
    static constexpr u32 WIDTH = 1280;
    static constexpr u32 MIN_WIDTH = 700;
    static constexpr u32 MAX_WIDTH = 1920;

    static constexpr u32 HEIGHT = 720;
    static constexpr u32 MIN_HEIGHT = 480;
    static constexpr u32 MAX_HEIGHT = 1080;

    static constexpr bool FULLSCREEN = false;
    static constexpr bool VSYNC = true;

    static constexpr const char* TITLE = "EduGame Engine";
    static constexpr u32 MIN_TITLE_LEN = 4;
    static constexpr u32 MAX_TITLE_LEN = 30;

    //GraphicsSettings
    static constexpr u32 MSAA = 4;
    static constexpr f32 ANISOTROPY = 8.0F;

    //LoggingSetting
    static constexpr LogLevel LOGLEVEL = LogLevel::INFO;

    //PerfomaceSettings
    static constexpr u32 TARGETFPS = 120;
    static constexpr u32 MIN_TFPS = 30;
    static constexpr u32 MAX_TFPS = 400;

    static constexpr bool CAPFPS = true;
  };

};

EGE_NAMESPACE_END