#pragma once

/**
 * @file    time.hpp
 * @brief   Time management and profiling utilities
 * @author  alex-1-tech
 * @date    2026
 */

#include <chrono>

#include "engine/core/types.hpp"

EGE_NAMESPACE_BEGIN

/// @brief Time management system for game/program timing
class Time
{
public:
  using Clock = std::chrono::high_resolution_clock;           ///< High resolution clock type
  using Duration = std::chrono::duration<f64>;                ///< Duration in seconds with double precision
  using TimePoint = std::chrono::time_point<Clock, Duration>; ///< Time point type

  static void init();   ///< Initialize time system
  static void update(); ///< Update time values each frame

  /**
   * @brief Get frame delta time (scaled)
   * @return Scaled delta time in seconds
   */
  static auto deltaTime() -> f64 { return s_delta_time; }

  /**
   * @brief Get fixed update delta time
   * @return Fixed delta time in seconds
   */
  static auto fixedDeltaTime() -> f64 { return s_fixed_delta_time; }

  /**
   * @brief Get current scaled time
   * @return Current scaled time in seconds
   */
  static auto time() -> f64 { return s_time; }

  /**
   * @brief Get time scaling factor
   * @return Time scaling factor
   */
  static auto timeScale() -> f64 { return s_time_scale; }

  /**
   * @brief Get current frame count
   * @return Total frames processed
   */
  static auto frameCount() -> u64 { return s_frame_count; }

  /**
   * @brief Set time scaling factor
   * @param scale Time scaling factor (1.0 = normal)
   */
  static void setTimeScale(f64 scale) { s_time_scale = scale; }

  /**
   * @brief Set fixed delta time
   * @param delta Fixed delta time in seconds
   */
  static void setFixedDeltaTime(f64 delta) { s_fixed_delta_time = delta; }

  /**
   * @brief Get time since initialization in seconds
   * @return Time elapsed since initialization
   */
  static auto sinceStart() -> f64
  {
    auto now = Clock::now();
    return std::chrono::duration<f64>(now - s_start_time).count();
  }

private:
  /**
   * @brief Get the start time instance
   * @return Reference to the start time
   */
  static auto getStartTime() -> TimePoint&;

  /**
   * @brief Get the last frame time instance
   * @return Reference to the last frame time
   */
  static auto getLastFrameTime() -> TimePoint&;

  /**
   * @brief Get the current frame time instance
   * @return Reference to the current frame time
   */
  static auto getCurrentFrameTime() -> TimePoint&;

  static TimePoint s_start_time;         ///< Time at initialization
  static TimePoint s_last_frame_time;    ///< Time of previous frame
  static TimePoint s_current_frame_time; ///< Time of current frame

  static f64 s_delta_time;       ///< Scaled delta time between frames
  static f64 s_fixed_delta_time; ///< Fixed timestep for physics
  static f64 s_time;             ///< Current scaled time
  static f64 s_time_scale;       ///< Time scaling factor (1.0 = normal)
  static u64 s_frame_count;      ///< Total frames processed
};

/// @brief Scoped timer for profiling and educational purposes
class Timer
{
public:
  /**
   * @brief Create timer with optional name
   * @param name Timer identifier for logging
   */
  Timer(String name = "")
      : m_name(std::move(name))
  {
    m_start = Time::Clock::now();
  }

  Timer(const Timer&) = delete;
  auto operator=(const Timer&) -> Timer& = delete;

  /**
   * @brief Move constructor
   * @param other Timer to move from
   */
  Timer(Timer&& other) noexcept
      : m_name(std::move(other.m_name))
      , m_start(other.m_start)
  {
  }

  /**
   * @brief Move assignment operator
   * @param other Timer to move from
   * @return Reference to this timer
   */
  auto operator=(Timer&& other) noexcept -> Timer&
  {
    if (this != &other) {
      m_name = std::move(other.m_name);
      m_start = other.m_start;
    }
    return *this;
  }

  ~Timer() ///< Timer destructor logs elapsed time if name provided
  {
    if (!m_name.empty()) {
      auto end = Time::Clock::now();
      f64 duration = std::chrono::duration<f64>(end - m_start).count();
      // EGE_DEBUG("Timer '{}': {:.6f}s", m_name, duration);
    }
  }

  /**
   * @brief Get elapsed time in seconds
   * @return Elapsed time in seconds
   */
  [[nodiscard]] auto elapsed() const -> f64
  {
    auto now = Time::Clock::now();
    return std::chrono::duration<f64>(now - m_start).count();
  }

  /**
   * @brief Reset timer to current time
   */
  void reset() { m_start = Time::Clock::now(); }

private:
  String m_name;           ///< Timer identifier for logging
  Time::TimePoint m_start; ///< Start time point
};

EGE_NAMESPACE_END