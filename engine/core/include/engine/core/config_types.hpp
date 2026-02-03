#pragma once

#include "engine/core/base.hpp"
#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"


EGE_NAMESPACE_BEGIN

enum class Property : u8 { WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, TARGET_FPS, LOGLEVEL };
using ConfigValue = std::variant<u32, String, LogLevel>;
struct PropertyInfo {
  ConfigValue defaultValue;
  u32 min;
  u32 max;
};

EGE_NAMESPACE_END