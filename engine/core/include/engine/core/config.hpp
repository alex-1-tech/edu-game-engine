#pragma once

#include "engine/core/types.hpp"

EGE_NAMESPACE_BEGIN

struct EngineConfig {
  // Window
  String windowTitle = "EduGame Engine";
  u32 windowWidth = 1280;
  u32 windowHeight = 720;
  bool windowFullscreen = false;
  bool windowVSync = true;

  // Graphics
  u32 msaaSamples = 4;
  f32 maxAnisotropy = 8.0F;

  // Performance
  u32 targetFPS = 120;
  bool capFPS = true;

  // Debug
  bool showDebugInfo = false;
  bool enableValidationLayers = true;

  // Time
  f64 MAX_DELTA = 0.1;
  f64 MILLISECONDS_PER_SECOND = 1000.0;
  u32 MAX_FIXED_UPDATES_PER_FRAME = 5;

  // Static instance
  static auto getDefault() -> const EngineConfig&
  {
    static EngineConfig instance;
    return instance;
  }
};

EGE_NAMESPACE_END