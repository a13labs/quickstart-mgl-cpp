
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
#include "application.hpp"
#include "input.hpp"
#include "log.hpp"

#include "SDL2/SDL_syswm.h"

namespace AppCore
{
  namespace Application
  {
    BaseWindow* BaseWindow::s_instance = nullptr;

    BaseWindow::BaseWindow(const WindowConfig& config)
    {
      APPCORE_ASSERT(!s_instance, "BaseWindow already running!");
      Log::init();

      m_state.current_config = config;
      m_state.native_window = nullptr;
      s_instance = this;
      m_running = false;
    }

    void BaseWindow::on_event(Events::Event& event)
    {
      Events::EventDispatcher dispatcher(event);

      // Dispatch Windows Events
      dispatcher.dispatch<Events::WindowCloseEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::on_window_close));
      dispatcher.dispatch<Events::WindowResizeEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::on_window_resize));

      // Dispatch key events to be handled by the application
      dispatcher.dispatch<Events::KeyPressedEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::on_key_pressed));
      dispatcher.dispatch<Events::KeyReleasedEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::on_key_released));

      // Dispatch mouse events to be handled by the application
      dispatcher.dispatch<Events::MouseMovedEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::on_mouse_moved));
      dispatcher.dispatch<Events::MouseScrolledEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::on_mouse_scrolled));
      dispatcher.dispatch<Events::MouseButtonPressedEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::on_mouse_button_pressed));
      dispatcher.dispatch<Events::MouseButtonReleasedEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::on_mouse_button_released));
    }

    bool BaseWindow::on_window_close(Events::WindowCloseEvent& event)
    {
      m_running = false;
      return true;
    }

    void BaseWindow::run()
    {
      if(m_running)
        return;

      if(SDL_Init(SDL_INIT_VIDEO) < 0)
      {
        APPCORE_TRACE("BaseWindow: Error initializing SDL");
        return;
      }

      if(!create_window())
      {
        APPCORE_TRACE("BaseWindow: Error creating Window");
        SDL_Quit();
        return;
      }

      Input::init();
      m_state.handler = APPCORE_BIND_EVENT_FN(BaseWindow::on_event);

      SDL_AddEventWatch(
          [](void* userdata, SDL_Event* currentEvent) -> int {
            WindowState& m_state = *(WindowState*)userdata;
            switch(currentEvent->type)
            {
              case SDL_WINDOWEVENT:
                switch(currentEvent->window.event)
                {
                  case SDL_WINDOWEVENT_RESIZED: {
                    auto w = currentEvent->window.data1;
                    auto h = currentEvent->window.data2;
                    Events::WindowResizeEvent event(w, h);
                    m_state.handler(event);
                  }
                  break;
                  case SDL_WINDOWEVENT_CLOSE: {
                    Events::WindowCloseEvent event;
                    m_state.handler(event);
                  }
                  break;
                  default: break;
                }
                break;
              case SDL_MOUSEMOTION:
              case SDL_MOUSEBUTTONDOWN:
              case SDL_MOUSEBUTTONUP:
              case SDL_MOUSEWHEEL:
              case SDL_KEYDOWN:
              case SDL_KEYUP: {
                Input::update_state(currentEvent, m_state.handler);

                if(Input::is_key_pressed(m_state.current_config.exit_key))
                {
                  BaseWindow::current().quit();
                }

                if(Input::is_key_pressed(m_state.current_config.fullscreen_key))
                {
                  BaseWindow::current().toggle_full_screen();
                }
              }
              break;
              default: break;
            }

            return 0;
          },
          &m_state);

      m_running = true;

      APPCORE_PROFILE_BEGIN_SESSION();

      on_load();
      m_timer.start();

      while(m_running)
      {
        SDL_Event e;
        SDL_PollEvent(&e);
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
      SDL_Quit();
    }

    void BaseWindow::quit()
    {
      m_running = false;
    }

    void BaseWindow::toggle_full_screen()
    {
      m_state.fullscreen = !m_state.fullscreen;
      if(m_state.fullscreen)
      {
        SDL_SetWindowFullscreen(m_state.native_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        return;
      }

      SDL_RestoreWindow(m_state.native_window);
      SDL_SetWindowSize(m_state.native_window, m_state.current_config.width, m_state.current_config.height);
      SDL_SetWindowPosition(m_state.native_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
      SDL_SetWindowFullscreen(m_state.native_window, 0);
    }

    bool BaseWindow::on_window_resize(AppCore::Events::WindowResizeEvent& event)
    {
      m_state.width = event.get_width();
      m_state.height = event.get_height();
      m_state.aspect_ratio = m_state.width / m_state.height;
      return true;
    }

    WindowConfig load_window_configuration(const String& filename)
    {
      // TODO: Implement load from JSON
      return WindowConfig();
    }
  } // namespace Application
} // namespace AppCore
