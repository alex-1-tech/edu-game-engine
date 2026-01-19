#pragma once

/**
 * @file    base.hpp
 * @brief   Platform, compiler, and utility macros for the engine
 * @author  alex-1-tech
 * @date    2026
 */

// Platform detection
#if defined(_WIN32) || defined(_WIN64)
#define EGE_PLATFORM_WINDOWS ///< Windows platform identifier
#elif defined(__linux__)
#define EGE_PLATFORM_LINUX ///< Linux platform identifier
#else
#error "Unsupported platform"
#endif

// Compiler detection
#if defined(_MSC_VER)
#define EGE_COMPILER_MSVC ///< Microsoft Visual C++ compiler
#elif defined(__GNUC__)
#define EGE_COMPILER_GCC ///< GNU Compiler Collection
#elif defined(__clang__)
#define EGE_COMPILER_CLANG ///< Clang compiler
#else
#warning "Unknown compiler"
#endif

// Debug macros
#if !defined(NDEBUG) || defined(EGE_DEBUG)
#define EGE_DEBUG_MODE 1 ///< Debug mode is active
#include <cassert>
/// @brief Assertion macro with logging
#define EGE_ASSERT(expr, ...)                                                  \
  do {                                                                         \
    if (!(expr)) {                                                             \
      engine::log_fatal("Assertion failed: {} ({})", #expr, __VA_ARGS__);      \
      assert(expr);                                                            \
    }                                                                          \
  } while (0)
#else
#define EGE_DEBUG_MODE 0                ///< Debug mode is inactive
#define EGE_ASSERT(expr, ...) ((void)0) ///< No-op in release builds
#endif

// Inline macros
#if defined(EGE_COMPILER_MSVC)
#define EGE_FORCE_INLINE __forceinline ///< Force inlining (MSVC)
#elif defined(EGE_COMPILER_GCC) || defined(EGE_COMPILER_CLANG)
#define EGE_FORCE_INLINE                                                       \
  inline __attribute__((always_inline)) ///< Force inlining (GCC/Clang)
#else
#define EGE_FORCE_INLINE inline ///< Standard inline fallback
#endif

// Export macros
#if defined(EGE_PLATFORM_WINDOWS)
#ifdef ENGINE_CORE_EXPORTS
#define EGE_API __declspec(dllexport) ///< Export symbol from DLL
#else
#define EGE_API __declspec(dllimport) ///< Import symbol from DLL
#endif
#else
#define EGE_API                                                                \
  __attribute__((visibility("default"))) ///< Default visibility on non-Windows
#endif

// Namespace shortcuts
#define EGE_NAMESPACE_BEGIN namespace engine { ///< Begin engine namespace
#define EGE_NAMESPACE_END }                    ///< End engine namespace