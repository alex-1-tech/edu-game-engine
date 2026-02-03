#pragma once

/**
 * @file    engine_properties.hpp
 * @brief   Defines engine configuration properties, their types, and constraints 🛠️
 * @author  jhfawk
 * @date    2026
 */

#include "engine/core/base.hpp"
#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"


EGE_NAMESPACE_BEGIN

/**
 * @enum  Property
 * @brief Enumeration of all configurable engine settings
 */
enum class Property : u8 {
  WINDOW_WIDTH,  //!< Width of the application window in pixels
  WINDOW_HEIGHT, //!< Height of the application window in pixels
  WINDOW_TITLE,  //!< Display title of the application window
  TARGET_FPS,    //!< Desired frame rate limit
  LOGLEVEL       //!< Minimum level for logging output
};


/**
 * @typedef ConfigValue
 * @brief   A variant type that can hold any valid configuration value
 */
using ConfigValue = std::variant<u32, String, LogLevel>;


/**
 * @struct PropertyInfo
 * @brief  Metadata for a property, including its default value and valid range
 */
struct PropertyInfo {
  ConfigValue defaultValue; //!< The value used if no custom setting is provided
  u32 min;                  //!< Minimum allowed value
  u32 max;                  //!< Maximum allowed value
};

EGE_NAMESPACE_END