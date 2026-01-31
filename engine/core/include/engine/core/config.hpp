#pragma once

#include <optional>
#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"


EGE_NAMESPACE_BEGIN

class IConfigLoader;
class JSONConfigLoader;

class EngineConfig final{

  public:
    enum class Param : u8 {
      WINDOW_WIDTH,
      WINDOW_HEIGHT,
      WINDOW_TITLE,
      TARGET_FPS,
      LOGLEVEL
    };
    
    ~EngineConfig() = default;

    EngineConfig() = default;
    EngineConfig(EngineConfig&) = delete;           ///< Copy constructor deleted
    EngineConfig(EngineConfig&&) = delete;          ///< Move constructor deleted
    auto operator=(EngineConfig&) = delete;   ///< Copy assignment deleted
    auto operator=(EngineConfig&&) = delete;  ///< Move assignment deleted
 
    static auto getInstance() -> EngineConfig& 
    {
      static EngineConfig instance;
      return instance;
    }
    auto loadFromFile(const IConfigLoader& loader, const String& path) -> bool;
    
    [[nodiscard]] auto saveToFile(const IConfigLoader& loader, const String& path) const -> bool;

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
        return T{};
        EGE_ERROR("EGE::Config: Type mismatch or unknown Param");
    } 

    template <typename T>
    void setParametr(Param confParam, const T& value){
        switch (confParam) {
            case Param::WINDOW_HEIGHT:
                if constexpr (std::is_same_v<T, u32>) {
                    applyIfValid(this->window.height, value, 
                        ConfigDefaults::MIN_HEIGHT,ConfigDefaults::MAX_HEIGHT, ConfigDefaults::HEIGHT,
                    [](const auto& val){ return val; });}
                break;
            case Param::WINDOW_WIDTH:
                if constexpr (std::is_same_v<T, u32>) {
                    applyIfValid(this->window.width, value,
                        ConfigDefaults::MIN_WIDTH,ConfigDefaults::MAX_WIDTH, ConfigDefaults::WIDTH,
                        [](const auto& val){ return val; });}
                break;
            case Param::WINDOW_TITLE:
                if constexpr (std::is_same_v<T, String>) {
                    applyIfValid(this->window.title, value, ConfigDefaults::MIN_TITLE_LEN,
                        ConfigDefaults::MAX_TITLE_LEN, String("EduGame Engine"),
                        [](const auto& val){ return val.length(); });
                }
                break;
            case Param::TARGET_FPS:
                if constexpr (std::is_same_v<T, u32>) {
                    applyIfValid(this->perfomance.targetFPS, value, ConfigDefaults::MIN_TFPS,
                        ConfigDefaults::MAX_TFPS, ConfigDefaults::TARGETFPS, 
                        [](const auto& val){ return val; });}
                break;
            case Param::LOGLEVEL: 
                if constexpr (std::is_same_v<T, std::optional<LogLevel>>) {
                    applyIfValidLevel(this->logging.level, value, ConfigDefaults::LOGLEVEL);}
                break;
        }
    }

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

    static constexpr const char* PATH_TO_CONFIG = PROJECT_ROOT "/settings.json";
    };
    
  private:
    static auto applyIfValidLevel(LogLevel& field, const std::optional<LogLevel>& value, const LogLevel& defaultVal) -> bool {
        field = value.value_or(defaultVal);
        if (value.has_value()){
            return true;
        }
        EGE_WARN("Validation failed: undefined logLevel, setting up default value");
        return false;
    }

    template<typename T, typename P, typename F>
    static auto applyIfValid(T& field, const T& value, P min, P max, T defaultVal, F transform) -> bool{
        auto valToCompare = transform(value);
        if (min < valToCompare && valToCompare < max){
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
};


class IConfigLoader {
public:
    virtual ~IConfigLoader() = default;

    IConfigLoader()=default;
    IConfigLoader(const IConfigLoader&) = delete;
    auto operator=(const IConfigLoader&) -> IConfigLoader& = delete;
    IConfigLoader(IConfigLoader&&) = delete;
    auto operator=(IConfigLoader&&) -> IConfigLoader& = delete;

    [[nodiscard]] virtual auto save(const EngineConfig& config, const String& path) const -> bool = 0;
    virtual auto load(EngineConfig& config, const String& path) const -> bool = 0;
};


class JSONConfigLoader : public IConfigLoader {
public:
    JSONConfigLoader() { 
        EGE_INFO("Loader INIT");
    }

    [[nodiscard]] auto save(const EngineConfig& config, const String& path) const -> bool override;
    auto load(EngineConfig& config, const String& path) const -> bool override; 
};



EGE_NAMESPACE_END