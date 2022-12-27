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
#include "appcore/appcore.hpp"
#include "appcore/timer.hpp"
#include "appgl/appgl.hpp"
#include "appwindow.hpp"
#include "event.hpp"
#include "input.hpp"

namespace AppWindow
{
  typedef struct
  {
    AppCore::String title = "BaseWindow";
    uint32_t width = 800;
    uint32_t height = 600;
    bool fullscreen = false;
    bool resizable = true;
    bool VSync = true;
    uint32_t samples = 0;
    bool cursor = true;
    Key::Enum exit_key = Key::Esc;
    Key::Enum fullscreen_key = Key::F11;
  } WindowConfig;

  class NativeWindow
  {
public:
    NativeWindow(const WindowConfig& config = WindowConfig()){};
    virtual ~NativeWindow() = default;

    virtual bool create_window() = 0;
    virtual void destroy_window() = 0;
    virtual void process_events() = 0;
    virtual void swap_buffers() = 0;
    virtual void initialize_event_handler(const EventHandler& handler) = 0;
    virtual void toggle_full_screen() = 0;

    virtual AppCore::Ref<AppGL::Context> context() = 0;
    virtual int width() = 0;
    virtual int height() = 0;
    virtual int aspect_ratio() = 0;

    virtual const AppCore::String& title() const = 0;
    virtual void set_title(const AppCore::String& value) = 0;
  };

  class BaseWindow
  {

public:
    BaseWindow(const WindowConfig& config = WindowConfig());
    virtual ~BaseWindow() = default;

public:
    void run();
    void quit();

    int width();
    int height();
    int aspect_ratio();

    const AppCore::String& title() const;
    void set_title(const AppCore::String& value);

    void toggle_full_screen();
    AppCore::Ref<AppGL::Context> context();

    inline static BaseWindow& current() { return *s_instance; }

    // Events
    void on_event(Event& event);

    // Windows Events
    virtual bool on_window_close(WindowCloseEvent& event);
    virtual bool on_window_resize(WindowResizeEvent& event) { return true; };

    // Keys Events
    virtual bool on_key_pressed(KeyPressedEvent& event) { return true; }
    virtual bool on_key_released(KeyReleasedEvent& event) { return true; }

    // Mouse Events
    virtual bool on_mouse_moved(MouseMovedEvent& event) { return true; }
    virtual bool on_mouse_scrolled(MouseScrolledEvent& event) { return true; }
    virtual bool on_mouse_button_pressed(MouseButtonPressedEvent& event) { return true; }
    virtual bool on_mouse_button_released(MouseButtonReleasedEvent& event) { return true; }

    virtual void on_draw(float time, float frame_time){};
    virtual void on_load(){};
    virtual void on_unload(){};

private:
    static BaseWindow* s_instance;
    bool m_running;
    AppCore::Timer m_timer;
    AppCore::Scope<NativeWindow> m_native_window;
  };

  inline int BaseWindow::width()
  {
    return m_native_window->width();
  }

  inline int BaseWindow::height()
  {
    return m_native_window->height();
  }

  inline int BaseWindow::aspect_ratio()
  {
    return m_native_window->aspect_ratio();
  }

  inline const AppCore::String& BaseWindow::title() const
  {
    return m_native_window->title();
  }

  inline void BaseWindow::set_title(const AppCore::String& value)
  {
    return m_native_window->set_title(value);
  }

  inline void BaseWindow::toggle_full_screen()
  {
    return m_native_window->toggle_full_screen();
  }

  inline void BaseWindow::quit()
  {
    m_running = false;
  }

  inline AppCore::Ref<AppGL::Context> BaseWindow::context()
  {
    return m_native_window->context();
  }

  WindowConfig load_window_configuration(const AppCore::String& filename);
} // namespace AppWindow
