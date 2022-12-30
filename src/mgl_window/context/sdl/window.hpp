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
#include "mgl_window/context/event.hpp"
#include "mgl_window/context/input.hpp"
#include "mgl_window/context/window.hpp"

#include "SDL2/SDL.h"

namespace mgl_window
{
  struct window_state
  {
    event_handler handler;
    window_config current_config = window_config();
    bool fullscreen = false;
    int width;
    int height;
  };

  class sdl_window : public native_window
  {

public:
    sdl_window(const window_config& config = window_config());
    virtual ~sdl_window() = default;

public:
    virtual bool create_window() override;
    virtual void destroy_window() override;
    virtual void swap_buffers() override;
    virtual void process_events() override;

    virtual int width() override;
    virtual int height() override;
    virtual float aspect_ratio() override;

    virtual void initialize_event_handler(const event_handler& handler) override;
    virtual void toggle_full_screen() override;
    virtual mgl_core::size get_drawable_size() override;

    virtual void set_title(const mgl_core::string& value) override;
    virtual const mgl_core::string& title() const override;

private:
    SDL_GLContext m_context;
    mgl_core::string m_title;
    SDL_Window* m_native_window = nullptr;
    window_state m_state;
  };
} // namespace mgl_window
