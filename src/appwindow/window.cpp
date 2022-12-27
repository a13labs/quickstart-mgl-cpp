
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
#include "appgl/context.hpp"
#include "input.hpp"
#include "sdl/window.hpp"

namespace AppWindow
{

  Window* Window::s_instance = nullptr;

  Window::Window(const WindowConfig& config)
  {
    APPCORE_ASSERT(!s_instance, "BaseWindow already running!");
    AppCore::Log::init();

    m_native_window = AppCore::create_scope<WindowSDL>(config);
    s_instance = this;
    m_running = false;
  }

  void Window::on_event(Event& event)
  {
    EventDispatcher dispatcher(event);

    // Dispatch Windows Events
    dispatcher.dispatch<WindowCloseEvent>(APPCORE_BIND_EVENT_FN(Window::on_window_close));
    dispatcher.dispatch<WindowResizeEvent>(APPCORE_BIND_EVENT_FN(Window::on_window_resize));

    // Dispatch key events to be handled by the application
    dispatcher.dispatch<KeyPressedEvent>(APPCORE_BIND_EVENT_FN(Window::on_key_pressed));
    dispatcher.dispatch<KeyReleasedEvent>(APPCORE_BIND_EVENT_FN(Window::on_key_released));

    // Dispatch mouse events to be handled by the application
    dispatcher.dispatch<MouseMovedEvent>(APPCORE_BIND_EVENT_FN(Window::on_mouse_moved));
    dispatcher.dispatch<MouseScrolledEvent>(APPCORE_BIND_EVENT_FN(Window::on_mouse_scrolled));
    dispatcher.dispatch<MouseButtonPressedEvent>(APPCORE_BIND_EVENT_FN(Window::on_mouse_button_pressed));
    dispatcher.dispatch<MouseButtonReleasedEvent>(APPCORE_BIND_EVENT_FN(Window::on_mouse_button_released));
  }

  bool Window::on_window_close(WindowCloseEvent& event)
  {
    m_running = false;
    return true;
  }

  void Window::run()
  {
    if(m_running)
      return;

    if(!m_native_window->create_window())
    {
      APPCORE_TRACE("BaseWindow: Error creating Window");
      return;
    }

    m_context = AppGL::Context::create_context(AppGL::ContextMode::SHARE, 330);

    if(!m_context)
    {
      APPCORE_TRACE("BaseWindow: Error initializing GL shared context.");
      m_native_window->destroy_window();
      return;
    }

    m_native_window->initialize_event_handler(APPCORE_BIND_EVENT_FN(Window::on_event));

    m_running = true;
    APPCORE_PROFILE_BEGIN_SESSION();

    on_load();
    m_timer.start();

    while(m_running)
    {
      m_native_window->process_events();
      auto frame_time = m_timer.next_frame();
      on_draw(frame_time.current, frame_time.delta);
      m_native_window->swap_buffers();
#if APPCORE_PROFILE
      // Since we are profiling we just render one frame
      m_running = false;
#endif
    }
    on_unload();

    APPCORE_PROFILE_END_SESSION();

    m_context->release();
    m_native_window->destroy_window();
  }

  WindowConfig load_window_configuration(const AppCore::String& filename)
  {
    // TODO: Implement load from JSON
    return WindowConfig();
  }
} // namespace AppWindow
