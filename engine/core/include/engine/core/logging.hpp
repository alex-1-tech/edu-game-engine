#pragma once

/**
 * @file    logging.hpp
 * @brief   Logging system for the engine
 * @author  alex-1-tech
 * @date    2026
 */

#include <fmt/core.h>
#include <fmt/format.h>

#include <cassert>
#include <fstream>
#include <mutex>
#include <optional>

#include "base.hpp"
#include "types.hpp"

EGE_NAMESPACE_BEGIN

/// @brief Log severity levels
enum class LogLevel : u8 {
  TRACE = 0, ///< Detailed tracing information
  DEBUG,     ///< Debug information
  INFO,      ///< General information
  WARNING,   ///< Warning messages
  ERROR,     ///< Error messages
  CRITICAL   ///< Critical errors
};

/// @brief Thread-safe logging system
class Logger
{
public:
  /**
   * @brief Initialize the logger
   * @param log_file Path to the log file (default: "engine.log")
   */
  static void init(const String& log_file = "engine.log");

  static void shutdown(); ///< Shutdown the logger and close file streams

  /**
   * @brief Log a formatted message
   * @tparam Args Format arguments type
   * @param level Log severity level
   * @param format Format string
   * @param args Format arguments
   */
  template<typename... Args> static void log(LogLevel level, const char* format, Args&&... args)
  {
    std::scoped_lock lock(getMutex());
    auto message = fmt::format(format, std::forward<Args>(args)...);
    logInternal(level, message);
  }

  /**
   * @brief Set minimum log level
   * @param level Minimum level to log
   */
  static void setLevel(LogLevel level);

  /**
   * @brief Enable or disable file output
   * @param enable True to enable file output, false to disable
   */
  static void enableFileOutput(bool enable);

  static auto getStringToLevel() -> const std::unordered_map<String, LogLevel>&;

  static auto stringToLogLevel(const String&) -> std::optional<LogLevel>;

  static auto logLevelToString(const LogLevel&) -> std::optional<String>;

private:
  /**
   * @brief Get the file stream instance
   * @return Reference to the file stream
   */
  static auto getFileStream() -> std::ofstream&;

  /**
   * @brief Internal logging implementation
   * @param level Log severity level
   * @param message Formatted message
   */
  static void logInternal(LogLevel level, const String& message);

  /**
   * @brief Get the mutex instance for thread safety
   * @return Reference to the mutex
   */
  static auto getMutex() -> std::mutex&;

  static LogLevel s_level;                    ///< Current log level
  static Unique<std::ofstream> s_file_stream; ///< Log file stream
  static bool s_file_enabled;                 ///< File output enabled flag
  static std::mutex s_mutex;                  ///< Thread safety mutex
};

// Convenience logging macros
#define EGE_TRACE(...) engine::Logger::log(engine::LogLevel::TRACE, __VA_ARGS__)       ///< Trace-level log
#define EGE_DEBUG(...) engine::Logger::log(engine::LogLevel::DEBUG, __VA_ARGS__)       ///< Debug-level log
#define EGE_INFO(...) engine::Logger::log(engine::LogLevel::INFO, __VA_ARGS__)         ///< Info-level log
#define EGE_WARN(...) engine::Logger::log(engine::LogLevel::WARNING, __VA_ARGS__)      ///< Warning-level log
#define EGE_ERROR(...) engine::Logger::log(engine::LogLevel::ERROR, __VA_ARGS__)       ///< Error-level log
#define EGE_CRITICAL(...) engine::Logger::log(engine::LogLevel::CRITICAL, __VA_ARGS__) ///< Critical-level log

// Educational logging macros (for learning runtime)
#define EGE_EXPLAIN(...) engine::Logger::log(engine::LogLevel::INFO, "[EXPLAIN] " __VA_ARGS__) ///< Explanation messages
#define EGE_HINT(...) engine::Logger::log(engine::LogLevel::INFO, "[HINT] " __VA_ARGS__)       ///< Hint messages


// Debug macros
#if !defined(NDEBUG) || defined(EGE_DEBUG)
#define EGE_DEBUG_MODE 1 ///< Debug mode is active

/// @brief Assertion macro with logging
#define EGE_ASSERT(expr, ...)                                                                                                              \
  do {                                                                                                                                     \
    if (!(expr)) {                                                                                                                         \
      EGE_CRITICAL("Assertion failed: {} ({})", #expr, __VA_ARGS__);                                                                       \
      assert(expr);                                                                                                                        \
    }                                                                                                                                      \
  } while (0)
#else
#define EGE_DEBUG_MODE 0                ///< Debug mode is inactive
#define EGE_ASSERT(expr, ...) ((void)0) ///< No-op in release builds
#endif

EGE_NAMESPACE_END