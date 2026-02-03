#pragma once

/**
 * @file    types.hpp
 * @brief   Type definitions and aliases for the engine
 * @author  alex-1-tech
 * @date    2026
 */

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <variant>

#include "engine/core/base.hpp"

EGE_NAMESPACE_BEGIN

/// Integer types
/// @{
using i8 = int8_t;   ///< 8-bit signed integer
using i16 = int16_t; ///< 16-bit signed integer
using i32 = int32_t; ///< 32-bit signed integer
using i64 = int64_t; ///< 64-bit signed integer

using u8 = uint8_t;   ///< 8-bit unsigned integer
using u16 = uint16_t; ///< 16-bit unsigned integer
using u32 = uint32_t; ///< 32-bit unsigned integer
using u64 = uint64_t; ///< 64-bit unsigned integer

using f32 = float;  ///< 32-bit floating point
using f64 = double; ///< 64-bit floating point
/// }@

/// Size types
/// @{
using usize = size_t;    ///< Unsigned size type (for array sizes)
using ssize = ptrdiff_t; ///< Signed size type (for pointer differences)
/// }@

/// String types
/// @{
using String = std::string;          ///< Mutable string type
using StringView = std::string_view; ///< Immutable string view type
/// }@

/// Smart pointers
/// @{
template<typename T> using Ref = std::shared_ptr<T>;    ///< Shared ownership smart pointer
template<typename T> using WeakRef = std::weak_ptr<T>;  ///< Non-owning reference to shared_ptr
template<typename T> using Unique = std::unique_ptr<T>; ///< Exclusive ownership smart pointer
/// }@

/// Function types
/// @{
template<typename Signature> using Function = std::function<Signature>; ///< Generic function wrapper
/// }@

/// Result type for error handling
/// @{
template<typename T, typename E = String> using Result = std::variant<T, E>; ///< Result type for error handling (T = success, E = error)
/// }@

/// ID types
/// @{
struct EntityId { ///< Entity identifier type
  constexpr explicit EntityId(u32 t_value = 0)
      : value(t_value)
  {
  }
  constexpr explicit operator u32() const { return value; }
  constexpr explicit operator u64() const { return static_cast<u64>(value); }
  [[nodiscard]] constexpr auto get() const { return value; }

  constexpr auto operator==(EntityId other) const { return value == other.value; }
  constexpr auto operator!=(EntityId other) const { return value != other.value; }

  constexpr auto operator==(u32 other) const { return value == other; }
  constexpr auto operator!=(u32 other) const { return value != other; }

private:
  u32 value;
};

struct EntityGeneration { ///< Entity generation type
  constexpr explicit EntityGeneration(u32 t_value = 0)
      : value(t_value)
  {
  }
  constexpr explicit operator u32() const { return value; }
  constexpr explicit operator u64() const { return static_cast<u64>(value); }
  [[nodiscard]] constexpr auto get() const { return value; }

  constexpr auto operator==(EntityGeneration other) const { return value == other.value; }
  constexpr auto operator!=(EntityGeneration other) const { return value != other.value; }

  constexpr auto operator==(u32 other) const { return value == other; }
  constexpr auto operator!=(u32 other) const { return value != other; }

private:
  u32 value;
};
using ComponentTypeId = u32; ///< Component type identifier
using SystemId = u32;        ///< System identifier type
/// }@

/// Special values
/// @{
static constexpr f32 ZERO_F = 0.0F;           ///< Zero as 32-bit float
static constexpr f32 ONE_F = 1.0F;            ///< One as 32-bit float
static constexpr f32 HALF_F = 0.5F;           ///< Half as 32-bit float
static constexpr i32 DOUBLE = 2;              ///< Double as 32-bit signed integer
static constexpr u32 ONE = 1U;                ///< One as 32-bit unsigned integer
static constexpr u32 ZERO = 0U;               ///< Zero as 32-bit unsigned integer
constexpr f64 EPSILON = 0.0001;               ///< Tolerance for floating-point comparisons
constexpr f64 DEFAULT_FIXED_FPS = 120.0;      ///< Default fixed update rate (frames per second)
static constexpr u32 ENABLE_EXTRA_CHECKS = 1; ///< Flag to enable additional runtime validation
static constexpr i32 IVALID_INDEX = -1;       ///<
/// }@

/// Time
/// @{
///< Conversion factor: seconds to milliseconds
static constexpr f64 MILLISECONDS_PER_SECOND = 1000.0;

///< Maximum fixed updates per frame to prevent spiral of death
static constexpr u32 MAX_FIXED_UPDATES_PER_FRAME = 5;
/// }@

/// Path
using Path = std::filesystem::path;

EGE_NAMESPACE_END