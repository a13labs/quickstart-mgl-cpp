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
#include "appwindow/context/event.hpp"
#include "appwindow/context/input.hpp"
#include "appwindow/context/window.hpp"

#include "SDL2/SDL.h"

namespace mgl_window
{
  typedef struct
  {
    EventHandler handler;
    WindowConfig current_config = WindowConfig();
    bool fullscreen = false;
    int width;
    int height;
  } WindowState;

  class WindowSDL : public NativeWindow
  {

public:
    WindowSDL(const WindowConfig& config = WindowConfig());
    virtual ~WindowSDL() = default;

public:
    virtual bool create_window() override;
    virtual void destroy_window() override;
    virtual void swap_buffers() override;
    virtual void process_events() override;

    virtual int width() override;
    virtual int height() override;
    virtual int aspect_ratio() override;

    virtual void initialize_event_handler(const EventHandler& handler) override;
    virtual void toggle_full_screen() override;

    virtual void set_title(const mgl_core::string& value) override;
    virtual const mgl_core::string& title() const override;

private:
    SDL_GLContext m_context;
    mgl_core::string m_title;
    SDL_Window* native_window = nullptr;
    WindowState m_state;
  };
} // namespace mgl_window
