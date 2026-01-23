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
  using Clock = std::chrono::steady_clock;                    ///< Steady_clock clock type
  using Duration = std::chrono::duration<f64>;                ///< Duration in seconds with double precision
  using TimePoint = std::chrono::time_point<Clock, Duration>; ///< Time point type

  static void init();   ///< Initialize time system
  static void update(); ///< Update time values each frame

  /// Getters
  /// @{
  static auto scaledDeltaTime() -> f64 { return s_scaled_delta_time; }
  static auto deltaTime() -> f64 { return s_unscaled_delta_time; }
  static auto fixedDeltaTime() -> f64 { return s_fixed_delta_time; }
  static auto time() -> f64 { return s_time; }
  static auto timeScale() -> f64 { return s_time_scale; }
  static auto frameCount() -> u64 { return s_frame_count; }
  /// @}

  /// Setters
  /// @{
  static void setTimeScale(f64 scale) { s_time_scale = scale; }
  static void setFixedDeltaTime(f64 delta) { s_fixed_delta_time = delta; }
  /// @}

  /// Fixed-step simulation utilities
  /// @{
  static auto hasFixedStep() -> bool;  ///< Check if fixed step should execute
  static void consumeFixedStep();      ///< Consume one fixed step from accumulator
  static void onFrameRendered();       ///< Increment frame counter after rendering
  static auto realSinceStart() -> f64; ///< Get real (unscaled) time since initialization
  /// @}

private:
  // -- Geters ( Meyers' singleton ) --
  static auto getStartTime() -> TimePoint&
  {
    static TimePoint start_time;
    return start_time;
  }
  static auto getLastFrameTime() -> TimePoint&
  {
    static TimePoint last_frame_time;
    return last_frame_time;
  }
  static auto getCurrentFrameTime() -> TimePoint&
  {
    static TimePoint current_frame_time;
    return current_frame_time;
  }
  // -----------------------------------

  static f64 s_unscaled_delta_time; ///< Unscaled delta time between frames
  static f64 s_scaled_delta_time;   ///< Scaled delta time between frames
  static f64 s_fixed_delta_time;    ///< Fixed timestep for physics
  static f64 s_time;                ///< Current scaled time
  static f64 s_time_scale;          ///< Time scaling factor (1.0 = normal)
  static u64 s_frame_count;         ///< Total frames processed
  static f64 s_accumulator;         ///< Accumulated time for fixed timestep simulation.
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
      f64 duration = Time::Duration(end - m_start).count();
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
    return Time::Duration(now - m_start).count();
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