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
#pragma once
#include "appcore.hpp"
#include "event.hpp"
#include "input.hpp"
#include "timer.hpp"

#include "SDL2/SDL.h"

namespace AppCore
{
  namespace Application
  {

    typedef struct
    {
      String title = "BaseWindow";
      uint32_t width = 800;
      uint32_t height = 600;
      bool fullscreen = false;
      bool resizable = true;
      bool VSync = true;
      uint32_t samples = 0;
      bool cursor = true;
      Input::Key::Enum exit_key = Input::Key::Esc;
      Input::Key::Enum fullscreen_key = Input::Key::F11;
    } WindowConfig;

    typedef struct
    {
      EventHandler handler;
      WindowConfig current_config = WindowConfig();
      SDL_Window* native_window = nullptr;
      bool fullscreen = false;
      int width;
      int height;
      float aspect_ratio;
    } WindowState;

    class BaseWindow
    {

  public:
      BaseWindow(const WindowConfig& config = WindowConfig());
      virtual ~BaseWindow() = default;

  public:
      virtual bool create_window() = 0;
      virtual void destroy_window() = 0;
      virtual void swap_buffers() = 0;
      virtual void set_title(const AppCore::String& value) = 0;

      virtual const AppCore::String& title() const;
      void run();
      void quit();
      void toggle_full_screen();

      float aspect_ratio();

  public:
      inline static BaseWindow& current() { return *s_instance; }

      // Events
      void on_event(Events::Event& event);

      // Windows Events
      virtual bool on_window_close(Events::WindowCloseEvent& event);
      virtual bool on_window_resize(Events::WindowResizeEvent& event);

      // Keys Events
      virtual bool on_key_pressed(Events::KeyPressedEvent& event) { return true; }
      virtual bool on_key_released(Events::KeyReleasedEvent& event) { return true; }

      // Mouse Events
      virtual bool on_mouse_moved(Events::MouseMovedEvent& event) { return true; }
      virtual bool on_mouse_scrolled(Events::MouseScrolledEvent& event) { return true; }
      virtual bool on_mouse_button_pressed(Events::MouseButtonPressedEvent& event) { return true; }
      virtual bool on_mouse_button_released(Events::MouseButtonReleasedEvent& event) { return true; }

      virtual void on_draw(float time, float frame_time){};
      virtual void on_load(){};
      virtual void on_unload(){};

  protected:
      WindowState m_state;

  private:
      static BaseWindow* s_instance;
      bool m_running;
      Timer m_timer;
    };

    WindowConfig load_window_configuration(const String& filename);

    inline float BaseWindow::aspect_ratio()
    {
      return m_state.aspect_ratio;
    }

    inline const AppCore::String& BaseWindow::title() const
    {
      return m_state.current_config.title;
    }

  } // namespace Application

} // namespace AppCore
