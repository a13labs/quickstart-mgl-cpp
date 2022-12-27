
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
#include "appwindow/context/window.hpp"
#include "appcore/log.hpp"
#include "appgl/context.hpp"
#include "appwindow/context/sdl/window.hpp"
#include "event.hpp"
#include "input.hpp"

namespace mgl_window
{

  Window* Window::s_instance = nullptr;

  Window::Window(const window_config& config)
  {
    MGL_CORE_ASSERT(!s_instance, "BaseWindow already running!");
    mgl_core::Log::init();

    m_native_window = mgl_core::create_scope<WindowSDL>(config);
    s_instance = this;
    m_running = false;
  }

  void Window::on_event(Event& event)
  {
    EventDispatcher dispatcher(event);

    // Dispatch Windows Events
    dispatcher.dispatch<WindowCloseEvent>(MGL_CORE_BIND_EVENT_FN(Window::on_window_close));
    dispatcher.dispatch<WindowResizeEvent>(MGL_CORE_BIND_EVENT_FN(Window::on_window_resize));

    // Dispatch key events to be handled by the application
    dispatcher.dispatch<KeyPressedEvent>(MGL_CORE_BIND_EVENT_FN(Window::on_key_pressed));
    dispatcher.dispatch<KeyReleasedEvent>(MGL_CORE_BIND_EVENT_FN(Window::on_key_released));

    // Dispatch mouse events to be handled by the application
    dispatcher.dispatch<MouseMovedEvent>(MGL_CORE_BIND_EVENT_FN(Window::on_mouse_moved));
    dispatcher.dispatch<MouseScrolledEvent>(MGL_CORE_BIND_EVENT_FN(Window::on_mouse_scrolled));
    dispatcher.dispatch<MouseButtonPressedEvent>(MGL_CORE_BIND_EVENT_FN(Window::on_mouse_button_pressed));
    dispatcher.dispatch<MouseButtonReleasedEvent>(MGL_CORE_BIND_EVENT_FN(Window::on_mouse_button_released));
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
      MGL_CORE_TRACE("BaseWindow: Error creating Window");
      return;
    }

    m_context = mgl::Context::create_context(mgl::ContextMode::SHARE, 330);

    if(!m_context)
    {
      MGL_CORE_TRACE("BaseWindow: Error initializing GL shared context.");
      m_native_window->destroy_window();
      return;
    }

    m_native_window->initialize_event_handler(MGL_CORE_BIND_EVENT_FN(Window::on_event));

    m_running = true;
    MGL_CORE_PROFILE_BEGIN_SESSION();

    on_load();
    m_timer.start();

    while(m_running)
    {
      m_native_window->process_events();
      auto frame_time = m_timer.next_frame();
      on_draw(frame_time.current, frame_time.delta);
      m_native_window->swap_buffers();
#if MGL_CORE_PROFILE
      // Since we are profiling we just render one frame
      m_running = false;
#endif
    }
    on_unload();

    MGL_CORE_PROFILE_END_SESSION();

    m_context->release();
    m_native_window->destroy_window();
  }

  window_config load_window_configuration(const mgl_core::string& filename)
  {
    // TODO: Implement load from JSON
    return window_config();
  }
} // namespace mgl_window
