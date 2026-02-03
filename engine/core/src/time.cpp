#include "engine/core/time.hpp"

#include <algorithm>

EGE_NAMESPACE_BEGIN


// Time system state
f64 Time::s_unscaled_delta_time = ZERO_F;
f64 Time::s_scaled_delta_time = ZERO_F;
f64 Time::s_fixed_delta_time = ONE_F / DEFAULT_FIXED_FPS;
f64 Time::s_time = ZERO_F;
f64 Time::s_time_scale = ONE_F; // 1.0 = realtime
u64 Time::s_frame_count = 0;
f64 Time::s_accumulator = 0;
const f64 MAX_DELTA = 0.25;

void Time::init()
{
  getStartTime() = Clock::now();
  getLastFrameTime() = getStartTime();
  getCurrentFrameTime() = getStartTime();
}


void Time::update()
{
  // Update current time and calculate delta time
  getCurrentFrameTime() = Clock::now();

  // Calculate delta time in seconds (duration cast to double)
  f64 realDeltaTime = std::chrono::duration<f64>(getCurrentFrameTime() - getLastFrameTime()).count();
  realDeltaTime = std::min(realDeltaTime, MAX_DELTA);

  // Accumulate unscaled time
  s_unscaled_delta_time = realDeltaTime;
  s_scaled_delta_time = realDeltaTime * s_time_scale;

  // Accumulate scaled time
  s_accumulator += s_scaled_delta_time;
  s_time += s_scaled_delta_time;


  // Update frame tracking
  getLastFrameTime() = getCurrentFrameTime();
}

auto Time::hasFixedStep() -> bool
{
  return s_accumulator >= (s_fixed_delta_time - EPSILON);
}
void Time::consumeFixedStep()
{
  s_accumulator -= s_fixed_delta_time;
}
void Time::onFrameRendered()
{
  ++s_frame_count;
}

auto Time::realSinceStart() -> f64
{
  auto now = Clock::now();
  return Duration(now - getStartTime()).count();
}


EGE_NAMESPACE_END