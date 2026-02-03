#pragma once

#include "engine/core//config_types.hpp"
#include "engine/core/base.hpp"
#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"


EGE_NAMESPACE_BEGIN

class EngineConfig;

struct ConfigDefaults {
  // WindowSettings
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

  // GraphicsSettings
  static constexpr u32 MSAA = 4;
  static constexpr f32 ANISOTROPY = 8.0F;

  // LoggingSetting
  static constexpr LogLevel LOGLEVEL = LogLevel::INFO;

  // PerfomaceSettings
  static constexpr u32 TARGET_FPS = 120;
  static constexpr u32 MIN_TFPS = 30;
  static constexpr u32 MAX_TFPS = 400;

  static constexpr bool CAPFPS = true;

  static constexpr const char* PATH_TO_CONFIG = "settings.json"; // PROJECT_ROOT "/settings.json"

  static void setUpDefaultSettings(EngineConfig& config);
  static auto getDefaultSettingsMap() -> const std::unordered_map<Property, PropertyInfo>&;
};
EGE_NAMESPACE_END