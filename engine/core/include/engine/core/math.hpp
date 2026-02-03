#pragma once

/**
 * @file    math.hpp
 * @brief   Math utilities and educational math functions
 * @author  alex-1-tech
 * @date    2026
 */

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "engine/core/types.hpp"

EGE_NAMESPACE_BEGIN

// Using GLM as foundation, but with our own interface for educational purposes
using Vec2 = glm::vec2; ///< 2D vector
using Vec3 = glm::vec3; ///< 3D vector
using Vec4 = glm::vec4; ///< 4D vector
using Mat3 = glm::mat3; ///< 3x3 matrix
using Mat4 = glm::mat4; ///< 4x4 matrix
using Quat = glm::quat; ///< Quaternion

// Math constants
namespace constants
{
inline constexpr f32 PI_VALUE = 3.14159265358979323846F;  ///< π constant
inline constexpr f32 PI_HALF = 1.57079632679489661923F;   ///< π/2 constant
inline constexpr f32 TAU_VALUE = 6.28318530717958647692F; ///< 2π constant
inline constexpr f32 DEG_TO_RAD = PI_VALUE / 180.0F;      ///< Degrees to radians conversion factor
inline constexpr f32 RAD_TO_DEG = 180.0F / PI_VALUE;      ///< Radians to degrees conversion factor
} // namespace constants

/// @brief Basic math utility functions
namespace math
{
/**
 * @brief Clamp value between min and max
 * @tparam T Value type
 * @param value Value to clamp
 * @param minimum Minimum allowed value
 * @param maximum Maximum allowed value
 * @return Clamped value
 */
template<typename T> constexpr auto clamp(T value, T minimum, T maximum) -> T
{
  if (value < minimum) {
    return minimum;
  }
  if (value > maximum) {
    return maximum;
  }
  return value;
}

/**
 * @brief Linear interpolation between a and b
 * @tparam T Value type
 * @param value_a Start value
 * @param value_b End value
 * @param time Interpolation factor [0, 1]
 * @return Interpolated value
 */
template<typename T> constexpr auto lerp(T value_a, T value_b, f32 time) -> T
{
  return value_a + ((value_b - value_a) * time);
}

/**
 * @brief Minimum of two values
 * @tparam T Value type
 * @param value_a First value
 * @param value_b Second value
 * @return Minimum value
 */
template<typename T> constexpr auto min(T value_a, T value_b) -> T
{
  return value_a < value_b ? value_a : value_b;
}

/**
 * @brief Maximum of two values
 * @tparam T Value type
 * @param value_a First value
 * @param value_b Second value
 * @return Maximum value
 */
template<typename T> constexpr auto max(T value_a, T value_b) -> T
{
  return value_a > value_b ? value_a : value_b;
}

/**
 * @brief Convert degrees to radians
 * @param degrees Angle in degrees
 * @return Angle in radians
 */
inline auto radians(f32 degrees) -> f32
{
  return degrees * constants::DEG_TO_RAD;
}

/**
 * @brief Convert radians to degrees
 * @param radians Angle in radians
 * @return Angle in degrees
 */
inline auto degrees(f32 radians) -> f32
{
  return radians * constants::RAD_TO_DEG;
}

// Smoothstep constants
constexpr f32 SMOOTHSTEP_MULTIPLIER_1 = 3.0F; ///< First smoothstep constant
constexpr f32 SMOOTHSTEP_MULTIPLIER_2 = 2.0F; ///< Second smoothstep constant
constexpr f32 SMOOTHSTEP_CLAMP_MAX = 1.0F;    ///< Maximum clamp value for smoothstep

/**
 * @brief Smoothstep interpolation
 * @param edge0 Lower edge of the transition
 * @param edge1 Upper edge of the transition
 * @param x_value Input value
 * @return Interpolated value
 */
inline auto smoothstep(f32 edge0, f32 edge1, f32 x_value) -> f32
{
  x_value = clamp((x_value - edge0) / (edge1 - edge0), 0.0F, SMOOTHSTEP_CLAMP_MAX);
  return x_value * x_value * (SMOOTHSTEP_MULTIPLIER_1 - SMOOTHSTEP_MULTIPLIER_2 * x_value);
}
} // namespace math

EGE_NAMESPACE_END