#include "engine/core/logging.hpp"

#include <array>
#include <boost/range/algorithm/find_if.hpp>
#include <chrono>
#include <ctime>
#include <iostream>
#include <optional>

EGE_NAMESPACE_BEGIN

constexpr usize TIME_STRING_BUFFER_SIZE = 64;
constexpr u8 LOG_LEVEL_COUNT = 6;

LogLevel Logger::s_level = LogLevel::INFO;
Unique<std::ofstream> Logger::s_file_stream = nullptr;
bool Logger::s_file_enabled = false;
std::mutex Logger::s_mutex;

void Logger::init(const String& log_file)
{
  std::lock_guard<std::mutex> lock(s_mutex);

  try {
    s_file_stream = std::make_unique<std::ofstream>(log_file, std::ios::out | std::ios::trunc);
    if (!s_file_stream->is_open()) {
      std::cerr << "Logger: failed to open file '" << log_file << "', falling back to console\n";
      s_file_stream.reset();
      s_file_enabled = false;
    } else {
      s_file_enabled = true;
      *s_file_stream << "=== Logger started ===\n";
      s_file_stream->flush();
    }
  } catch (const std::exception& e) {
    std::cerr << "Logger exception: " << e.what() << '\n';
    s_file_stream.reset();
    s_file_enabled = false;
  }
}

void Logger::shutdown()
{
  std::lock_guard<std::mutex> lock(s_mutex);

  if (s_file_stream && s_file_stream->is_open()) {
    s_file_stream->close();
  }
  s_file_stream.reset();
}

void Logger::setLevel(LogLevel level)
{
  std::lock_guard<std::mutex> lock(s_mutex);
  s_level = level;
}

void Logger::enableFileOutput(bool enable)
{
  std::lock_guard<std::mutex> lock(s_mutex);
  s_file_enabled = enable;
}

void Logger::logInternal(LogLevel level, const String& message)
{
  if (level < s_level) {
    return;
  }

  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  std::array<char, TIME_STRING_BUFFER_SIZE> time_str{};

  std::tm local_time{};
#ifdef _WIN32
  (void)localtime_s(&local_time, &time);
#else
  (void)localtime_r(&time, &local_time);
#endif

  const usize chars_written = std::strftime(time_str.data(), time_str.size(), "%H:%M:%S", &local_time);

  if (chars_written == 0) {
    time_str[0] = '\0';
  }

  constexpr std::array<const char*, LOG_LEVEL_COUNT> LEVEL_STRINGS = {"TRACE", "DEBUG", "INFO",
                                                                      "WARN",  "ERROR", "CRITICAL"};

  auto formatted =
      fmt::format("[{}] [{}] {}", time_str.data(), LEVEL_STRINGS[static_cast<int>(level)], message);

  std::ostream& stream = (level >= LogLevel::WARNING) ? std::cerr : std::cout;
  stream << formatted << '\n';
  stream.flush();

  if (s_file_enabled && s_file_stream && s_file_stream->is_open()) {
    *s_file_stream << formatted << '\n';
    s_file_stream->flush();
  }
}

auto Logger::getMutex() -> std::mutex&
{
  return s_mutex;
}

auto Logger::getStringToLevel() -> const std::unordered_map<String, LogLevel>& 
{
  static const auto STRING_TO_LEVEL = std::unordered_map<String, LogLevel> {
        {"TRACE",    LogLevel::TRACE},
        {"DEBUG",    LogLevel::DEBUG},
        {"INFO",     LogLevel::INFO},
        {"WARNING",  LogLevel::WARNING},
        {"ERROR",    LogLevel::ERROR},
        {"CRITICAL", LogLevel::CRITICAL}
    };
  return STRING_TO_LEVEL;
}


auto Logger::stringToLogLevel(const String& level) -> std::optional<LogLevel>{
    const auto& map = Logger::getStringToLevel();
    auto map_it = map.find(level);
    if (map_it != map.end())
    {
        return map_it->second;
    }
    return std::nullopt;
}

auto Logger::logLevelToString(const LogLevel& level) -> std::optional<String>{
    const auto& map = Logger::getStringToLevel();

    auto map_it = boost::range::find_if(map, 
    [level](const auto& pair){ return pair.second == level; });
    if (map_it != map.end()){
      return map_it->first;
    }
    return std::nullopt;
    
}

EGE_NAMESPACE_END