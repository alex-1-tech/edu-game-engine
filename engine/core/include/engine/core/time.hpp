#pragma once

/**
 * @file    time.hpp
 * @brief   Time management and profiling utilities
 * @author  alex-1-tech
 * @date    2026
 */

#include "engine/core/types.hpp"
// #include "engine/core/logging.hpp"

#include <chrono>

EGE_NAMESPACE_BEGIN

/// @brief Time management system for game/program timing
class Time {
public:
  using Clock =
      std::chrono::high_resolution_clock; ///< High resolution clock type
  using Duration =
      std::chrono::duration<f64>; ///< Duration in seconds with double precision
  using TimePoint =
      std::chrono::time_point<Clock, Duration>; ///< Time point type

  static void init();   ///< Initialize time system
  static void update(); ///< Update time values each frame

  // Getters
  static f64 delta_time() {
    return s_delta_time;
  } ///< Get frame delta time (scaled)
  static f64 fixed_delta_time() {
    return s_fixed_delta_time;
  } ///< Get fixed update delta time
  static f64 time() { return s_time; }             ///< Get current scaled time
  static f64 time_scale() { return s_time_scale; } ///< Get time scaling factor
  static u64 frame_count() {
    return s_frame_count;
  } ///< Get current frame count

  // Setters
  static void set_time_scale(f64 scale) {
    s_time_scale = scale;
  } ///< Set time scaling factor
  static void set_fixed_delta_time(f64 dt) {
    s_fixed_delta_time = dt;
  } ///< Set fixed delta time

  /// @brief Get time since initialization in seconds
  static f64 since_start() {
    auto now = Clock::now();
    return std::chrono::duration<f64>(now - s_start_time).count();
  }

private:
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
class Timer {
public:
  /// @brief Create timer with optional name
  Timer(String name = "") : m_name(std::move(name)) {
    m_start = Time::Clock::now();
  }

  /// @brief Timer destructor logs elapsed time if name provided
  ~Timer() {
    if (!m_name.empty()) {
      auto end = Time::Clock::now();
      f64 duration = std::chrono::duration<f64>(end - m_start).count();
      // EGE_DEBUG("Timer '{}': {:.6f}s", m_name, duration);
    }
  }

  /// @brief Get elapsed time in seconds
  [[nodiscard]] f64 elapsed() const {
    auto now = Time::Clock::now();
    return std::chrono::duration<f64>(now - m_start).count();
  }

  /// @brief Reset timer to current time
  void reset() { m_start = Time::Clock::now(); }

private:
  String m_name;           ///< Timer identifier for logging
  Time::TimePoint m_start; ///< Start time point
};

EGE_NAMESPACE_END