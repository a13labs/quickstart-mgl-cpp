#pragma once
#include "builtins.hpp"

namespace AppCore
{

  class Timer
  {
    struct FrameTime
    {
      float current;
      float delta;
    };

public:
    Timer();
    ~Timer() = default;
    bool is_paused();
    bool is_running();
    float time();
    Timer::FrameTime next_frame();
    void start();
    void pause();
    float stop();
    void toogle_pause();

private:
    Timepoint m_start_time;
    Timepoint m_stop_time;
    Timepoint m_pause_time;
    float m_last_frame;
    std::chrono::nanoseconds m_offset;
    bool m_paused;
    bool m_running;
  };

  inline bool Timer::is_paused()
  {
    return m_paused;
  }

  inline bool Timer::is_running()
  {
    return m_running;
  }

  inline void Timer::toogle_pause()
  {
    if(m_paused)
    {
      start();
    }
    else
    {
      pause();
    }
  }

} // namespace AppCore