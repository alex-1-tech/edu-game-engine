#include "engine/core/time.hpp"

EGE_NAMESPACE_BEGIN

namespace
{
constexpr f64 ONE_VALUE = 1.0;
constexpr f64 SIXTY_VALUE = 60.0;
constexpr f64 ZERO_VALUE = 0.0;
constexpr f64 DEFAULT_FIXED_FPS = 60.0;
} // namespace


// Time system state
f64 Time::s_delta_time = ZERO_VALUE;
f64 Time::s_fixed_delta_time = ONE_VALUE / DEFAULT_FIXED_FPS; // 60 FPS default
f64 Time::s_time = ZERO_VALUE;
f64 Time::s_time_scale = ONE_VALUE; // 1.0 = realtime
u64 Time::s_frame_count = 0;

void Time::init()
{
  getStartTime() = Clock::now();
  getLastFrameTime() = getStartTime();
  getCurrentFrameTime() = getStartTime();
}

auto Time::getStartTime() -> TimePoint&
{
  static TimePoint start_time;
  return start_time;
}

auto Time::getLastFrameTime() -> TimePoint&
{
  static TimePoint last_frame_time;
  return last_frame_time;
}

auto Time::getCurrentFrameTime() -> TimePoint&
{
  static TimePoint current_frame_time;
  return current_frame_time;
}

void Time::update()
{
  // Update current time and calculate delta time
  getCurrentFrameTime() = Clock::now();

  // Calculate delta time in seconds (duration cast to double)
  s_delta_time = std::chrono::duration<f64>(getCurrentFrameTime() - getLastFrameTime()).count();

  // Accumulate scaled time (affected by time_scale for slow-motion/fast-forward
  // effects)
  s_time += s_delta_time * s_time_scale;

  // Update frame tracking
  getLastFrameTime() = getCurrentFrameTime();
  s_frame_count++;
}

EGE_NAMESPACE_END