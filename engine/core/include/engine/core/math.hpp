#pragma once

/**
 * @file    math.hpp
 * @brief   Math utilities and educational math functions
 * @author  alex-1-tech
 * @date    2026
 */

#include "engine/core/logging.hpp"
#include "engine/core/types.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <sstream>

EGE_NAMESPACE_BEGIN

// Using GLM as foundation, but with our own interface for educational purposes
using Vec2 = glm::vec2; ///< 2D vector
using Vec3 = glm::vec3; ///< 3D vector
using Vec4 = glm::vec4; ///< 4D vector
using Mat3 = glm::mat3; ///< 3x3 matrix
using Mat4 = glm::mat4; ///< 4x4 matrix
using Quat = glm::quat; ///< Quaternion

// Math constants
constexpr f32 PI = 3.14159265358979323846F;   ///< π constant
constexpr f32 PI_2 = 1.57079632679489661923F; ///< π/2 constant
constexpr f32 TAU = 6.28318530717958647692F;  ///< 2π constant
constexpr f32 DEG_TO_RAD =
    PI / 180.0F; ///< Degrees to radians conversion factor
constexpr f32 RAD_TO_DEG =
    180.0F / PI; ///< Radians to degrees conversion factor

/// @brief Basic math utility functions
namespace math {
/// @brief Clamp value between min and max
template <typename T> constexpr T clamp(T value, T min, T max) {
  if (value < min) {
    return min;
  }
  if (value > max) {
    return max;
  }
  return value;
}

/// @brief Linear interpolation between a and b
template <typename T> constexpr T lerp(T a, T b, f32 t) {
  return a + ((b - a) * t);
}

/// @brief Minimum of two values
template <typename T> constexpr T min(T a, T b) { return a < b ? a : b; }

/// @brief Maximum of two values
template <typename T> constexpr T max(T a, T b) { return a > b ? a : b; }

inline f32 radians(f32 degrees) {
  return degrees * DEG_TO_RAD;
} ///< Convert degrees to radians
inline f32 degrees(f32 radians) {
  return radians * RAD_TO_DEG;
} ///< Convert radians to degrees

/// @brief Smoothstep interpolation
inline f32 smoothstep(f32 edge0, f32 edge1, f32 x) {
  x = clamp((x - edge0) / (edge1 - edge0), 0.0F, 1.0F);
  return x * x * (3.0F - 2.0F * x);
}
} // namespace math

/// @brief Educational math functions with explanations
namespace edu_math {
/// @brief Convert vector to string representation
inline std::string vec_to_string(const Vec3 &v) {
  std::ostringstream os;
  os << '[' << v.x << ", " << v.y << ", " << v.z << ']';
  return os.str();
}

/// @brief Explain dot product with educational logging
inline void explain_dot_product(const Vec3 &a, const Vec3 &b) {
  f32 result = glm::dot(a, b);
  EGE_EXPLAIN("Dot product of {} and {} = {}", vec_to_string(a),
              vec_to_string(b), result);
  EGE_EXPLAIN("It measures how much two vectors point in the same direction");
}

/// @brief Explain cross product with educational logging
inline void explain_cross_product(const Vec3 &a, const Vec3 &b) {
  Vec3 result = glm::cross(a, b);
  EGE_EXPLAIN("Cross product of {} and {} = {}", vec_to_string(a),
              vec_to_string(b), vec_to_string(result));
  EGE_EXPLAIN("It produces a vector perpendicular to both input vectors");
}
} // namespace edu_math

EGE_NAMESPACE_END