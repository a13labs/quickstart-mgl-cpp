
/*
   Copyright 2022 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#include "window.hpp"
#include "appcore/log.hpp"
#include "input.hpp"

namespace AppWindow
{
  BaseWindow* BaseWindow::s_instance = nullptr;

  BaseWindow::BaseWindow(const WindowConfig& config)
  {
    APPCORE_ASSERT(!s_instance, "BaseWindow already running!");
    AppCore::Log::init();

    m_state.current_config = config;
    s_instance = this;
    m_running = false;
  }

  void BaseWindow::on_event(Event& event)
  {
    EventDispatcher dispatcher(event);

    // Dispatch Windows Events
    dispatcher.dispatch<WindowCloseEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::on_window_close));
    dispatcher.dispatch<WindowResizeEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::on_window_resize));

    // Dispatch key events to be handled by the application
    dispatcher.dispatch<KeyPressedEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::on_key_pressed));
    dispatcher.dispatch<KeyReleasedEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::on_key_released));

    // Dispatch mouse events to be handled by the application
    dispatcher.dispatch<MouseMovedEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::on_mouse_moved));
    dispatcher.dispatch<MouseScrolledEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::on_mouse_scrolled));
    dispatcher.dispatch<MouseButtonPressedEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::on_mouse_button_pressed));
    dispatcher.dispatch<MouseButtonReleasedEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::on_mouse_button_released));
  }

  bool BaseWindow::on_window_close(WindowCloseEvent& event)
  {
    m_running = false;
    return true;
  }

  void BaseWindow::run()
  {
    if(m_running)
      return;

    if(!create_window())
    {
      APPCORE_TRACE("BaseWindow: Error creating Window");
      return;
    }

    m_state.handler = APPCORE_BIND_EVENT_FN(BaseWindow::on_event);
    initialize_event_handler();

    m_running = true;
    APPCORE_PROFILE_BEGIN_SESSION();

    on_load();
    m_timer.start();

    while(m_running)
    {
      process_events();
      auto frame_time = m_timer.next_frame();
      on_draw(frame_time.current, frame_time.delta);
      swap_buffers();
#if APPCORE_PROFILE
      // Since we are profiling we just render one frame
      m_running = false;
#endif
    }
    on_unload();

    APPCORE_PROFILE_END_SESSION();
    destroy_window();
  }

  void BaseWindow::quit()
  {
    m_running = false;
  }

  bool BaseWindow::on_window_resize(WindowResizeEvent& event)
  {
    m_state.width = event.get_width();
    m_state.height = event.get_height();
    return true;
  }

  WindowConfig load_window_configuration(const AppCore::String& filename)
  {
    // TODO: Implement load from JSON
    return WindowConfig();
  }
} // namespace AppWindow
