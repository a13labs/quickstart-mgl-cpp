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
#include "appgl/appgl.hpp"
#include "appwindow/event.hpp"
#include "appwindow/input.hpp"
#include "appwindow/window.hpp"

#include "SDL2/SDL.h"

namespace AppWindow
{
  class Window : public BaseWindow
  {

public:
    Window(const WindowConfig& config = WindowConfig())
        : BaseWindow(config)
    { }
    virtual ~Window() = default;

public:
    virtual bool create_window() override;
    virtual void destroy_window() override;
    virtual void swap_buffers() override;
    virtual void process_events() override;

    virtual bool on_window_resize(WindowResizeEvent& event) override;
    AppCore::Ref<AppGL::Context> context();

    virtual void initialize_event_handler() override;
    virtual void toggle_full_screen() override;

    virtual void set_title(const AppCore::String& value) override;
    virtual const AppCore::String& title() const override;

private:
    SDL_GLContext m_context;
    AppCore::Ref<AppGL::Context> m_shared_context;
    AppCore::String m_title;
    SDL_Window* native_window = nullptr;
  };

  inline AppCore::Ref<AppGL::Context> Window::context()
  {
    return m_shared_context;
  }
} // namespace AppWindow
