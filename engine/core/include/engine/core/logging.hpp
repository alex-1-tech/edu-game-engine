#pragma once

/**
 * @file    logger.hpp
 * @brief   Logging system for the engine
 * @author  alex-1-tech
 * @date    2026
 */

#include "base.hpp"
#include "types.hpp"
#include <fmt/core.h>
#include <fmt/format.h>
#include <fstream>
#include <mutex>

EGE_NAMESPACE_BEGIN

/// @brief Log severity levels
enum class LogLevel : u8 {
  Trace = 0, ///< Detailed tracing information
  Debug,     ///< Debug information
  Info,      ///< General information
  Warning,   ///< Warning messages
  Error,     ///< Error messages
  Critical   ///< Critical errors
};

/// @brief Thread-safe logging system
class Logger {
public:
  static void
  init(const String &log_file = "engine.log"); ///< Initialize the logger
  static void shutdown();                      ///< Shutdown the logger

  /// @brief Log a formatted message
  template <typename... Args>
  static void log(LogLevel level, const char *format, Args &&...args) {
    std::scoped_lock lock(get_mutex());
    auto message = fmt::format(format, std::forward<Args>(args)...);
    log_internal(level, message);
  }

  static void set_level(LogLevel level); ///< Set minimum log level
  static void
  enable_file_output(bool enable); ///< Enable or disable file output

private:
  static void
  log_internal(LogLevel level,
               const String &message); //< Internal logging implementation

  static std::mutex &get_mutex(); ///< Get the mutex instance

  static LogLevel s_level;            ///< Current log level
  static std::ofstream s_file_stream; ///< Log file stream
  static bool s_file_enabled;         ///< File output enabled flag
  static std::mutex s_mutex;          ///< Thread safety mutex
};

// Convenience logging macros
#define EGE_TRACE(...)                                                         \
  engine::Logger::log(engine::LogLevel::Trace, __VA_ARGS__) ///< Trace-level log
#define EGE_DEBUG(...)                                                         \
  engine::Logger::log(engine::LogLevel::Debug, __VA_ARGS__) ///< Debug-level log
#define EGE_INFO(...)                                                          \
  engine::Logger::log(engine::LogLevel::Info, __VA_ARGS__) ///< Info-level log
#define EGE_WARN(...)                                                          \
  engine::Logger::log(engine::LogLevel::Warning,                               \
                      __VA_ARGS__) ///< Warning-level log
#define EGE_ERROR(...)                                                         \
  engine::Logger::log(engine::LogLevel::Error, __VA_ARGS__) ///< Error-level log
#define EGE_CRITICAL(...)                                                      \
  engine::Logger::log(engine::LogLevel::Critical,                              \
                      __VA_ARGS__) ///< Critical-level log

// Educational logging macros (for learning runtime)
#define EGE_EXPLAIN(...)                                                       \
  engine::Logger::log(engine::LogLevel::Info,                                  \
                      "[EXPLAIN] " __VA_ARGS__) ///< Explanation messages
#define EGE_HINT(...)                                                          \
  engine::Logger::log(engine::LogLevel::Info,                                  \
                      "[HINT] " __VA_ARGS__) ///< Hint messages

EGE_NAMESPACE_END