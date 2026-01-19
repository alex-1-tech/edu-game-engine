#include "engine/core/time.hpp"

EGE_NAMESPACE_BEGIN

// Static member initialization
Time::TimePoint Time::s_start_time;
Time::TimePoint Time::s_last_frame_time;
Time::TimePoint Time::s_current_frame_time;

// Time system state
f64 Time::s_delta_time = 0.0;
f64 Time::s_fixed_delta_time = 1.0 / 60.0; // 60 FPS default
f64 Time::s_time = 0.0;
f64 Time::s_time_scale = 1.0; // 1.0 = realtime
u64 Time::s_frame_count = 0;

void Time::init() {
  s_start_time = Clock::now();
  s_last_frame_time = s_start_time;
  s_current_frame_time = s_start_time;
}

void Time::update() {
  // Update current time and calculate delta time
  s_current_frame_time = Clock::now();

  // Calculate delta time in seconds (duration cast to double)
  s_delta_time =
      std::chrono::duration<f64>(s_current_frame_time - s_last_frame_time)
          .count();

  // Accumulate scaled time (affected by time_scale for slow-motion/fast-forward
  // effects)
  s_time += s_delta_time * s_time_scale;

  // Update frame tracking
  s_last_frame_time = s_current_frame_time;
  s_frame_count++;
}

EGE_NAMESPACE_END