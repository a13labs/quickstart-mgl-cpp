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
#include "appcore/application.hpp"
#include "appcore/event.hpp"
#include "appcore/input.hpp"
#include "appgl.hpp"

namespace AppGL
{
  class Window : public AppCore::Application::BaseWindow
  {

public:
    Window(const AppCore::Application::WindowConfig& config = AppCore::Application::WindowConfig())
        : AppCore::Application::BaseWindow(config)
    { }
    virtual ~Window() = default;

public:
    virtual bool create_window() override;
    virtual void destroy_window() override;
    virtual void swap_buffers() override;

    virtual bool on_window_resize(AppCore::Events::WindowResizeEvent& event) override;
    AppCore::Ref<Context> context();

    virtual void initialize_event_handler() override;
    virtual void toggle_full_screen() override;

    virtual void set_title(const AppCore::String& value) override;
    virtual const AppCore::String& title() const override;

private:
    SDL_GLContext m_context;
    AppCore::Ref<Context> m_shared_context;
    AppCore::String m_title;
    SDL_Window* native_window = nullptr;
  };

  inline AppCore::Ref<Context> Window::context()
  {
    return m_shared_context;
  }
} // namespace AppGL
