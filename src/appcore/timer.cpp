#include "timer.hpp"

namespace AppCore
{

  Timer::Timer()
  {
    m_running = false;
    m_paused = false;
    m_offset = std::chrono::nanoseconds(0);
  }

  void Timer::start()
  {
    if(m_paused)
    {
      return;
    }

    if(!m_running)
    {
      m_start_time = std::chrono::system_clock::now();
      m_last_frame = 0.0;
      m_offset = std::chrono::nanoseconds(0);
      m_running = true;
      m_paused = false;
    }
    else
    {
      m_offset += std::chrono::system_clock::now() - m_pause_time;
      m_paused = false;
    }
  }

  void Timer::pause()
  {
    if(!m_running)
    {
      return;
    }

    m_pause_time = std::chrono::system_clock::now();
    m_paused = true;
  }

  float Timer::time()
  {
    if(!m_running)
    {
      return 0.0;
    }

    if(m_paused)
    {
      auto elapsed = std::chrono::duration<float, std::chrono::seconds::period>(m_pause_time - m_start_time - m_offset);
      return elapsed.count();
    }

    auto elapsed =
        std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::system_clock::now() - m_start_time - m_offset);

    return elapsed.count();
  }

  Timer::FrameTime Timer::next_frame()
  {
    auto current = time();
    auto delta = current - m_last_frame;
    m_last_frame = current;

    return { current, delta };
  }

  float Timer::stop()
  {
    auto elapsed =
        std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::system_clock::now() - m_start_time - m_offset);

    m_running = false;
    m_paused = false;
    return elapsed.count();
  }

} // namespace AppCore