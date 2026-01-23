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

  // Static instance
  static auto getDefault() -> const EngineConfig&
  {
    static EngineConfig instance;
    return instance;
  }
};

EGE_NAMESPACE_END