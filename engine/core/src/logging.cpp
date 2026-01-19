#include "engine/core/logging.hpp"

#include <array>
#include <chrono>
#include <ctime>
#include <iostream>

EGE_NAMESPACE_BEGIN

// Initialize static members
LogLevel Logger::s_level = LogLevel::Info;
std::ofstream Logger::s_file_stream;
bool Logger::s_file_enabled = false;
std::mutex Logger::s_mutex;

void Logger::init(const String &log_file) {
  if (s_file_enabled) {
    s_file_stream.open(log_file, std::ios::out | std::ios::trunc);
    if (!s_file_stream.is_open()) {
      std::cerr << "Failed to open log file: " << log_file << '\n';
      s_file_enabled = false;
    }
  }
}

void Logger::shutdown() {
  if (s_file_stream.is_open()) {
    s_file_stream.close();
  }
}

void Logger::set_level(LogLevel level) { s_level = level; }

void Logger::enable_file_output(bool enable) { s_file_enabled = enable; }

void Logger::log_internal(LogLevel level, const String &message) {
  if (level < s_level) {
    return;
  }

  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  std::array<char, 64> time_str{};
  std::strftime(time_str.data(), sizeof(time_str), "%H:%M:%S",
                std::localtime(&time));

  constexpr std::array<const char *, 6> level_str = {
      "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "CRITICAL"};

  auto formatted = fmt::format("[{}] [{}] {}", time_str.data(),
                               level_str[static_cast<int>(level)], message);

  std::ostream &stream = (level >= LogLevel::Warning) ? std::cerr : std::cout;
  stream << formatted << '\n';

  if (s_file_enabled && s_file_stream.is_open()) {
    s_file_stream << formatted << '\n';
    s_file_stream.flush();
  }
}

std::mutex &Logger::get_mutex() { return s_mutex; }

EGE_NAMESPACE_END