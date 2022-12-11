/*
   Copyright 2020 Alexandre Pires (c.alexandre.pires@gmail.com)

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
#include "base.hpp"
#include "event.hpp"
#include "input.hpp"

#include "SDL2/SDL.h"

namespace AppCore
{
  typedef struct
  {
    std::string Title = "Application";
    uint32_t Width = 800;
    uint32_t Height = 600;
    uint32_t GLMajor = 3;
    uint32_t GLMinor = 3;
    bool Fullscreen = false;
    bool Resizable = true;
    bool VSync = true;
    uint32_t Samples = 0;
    bool Cursor = true;
    Input::Key::Enum ExitKey = Input::Key::Esc;
    Input::Key::Enum FullScreenKey = Input::Key::F11;
  } ApplicationConfig;

  typedef struct
  {
    EventHandler Handler;
    ApplicationConfig Config = ApplicationConfig();
    bool Fullscreen = false;
  } ApplicationStateData;

  class Application
  {

public:
    Application(const ApplicationConfig& config = ApplicationConfig());
    virtual ~Application() = default;

public:
    void Run();
    void Quit();
    void ToggleFullScreen();

public:
    static Application& Current() { return *sInstance; }

    // Events
    void OnEvent(Events::Event& event);

    // Windows Events
    virtual bool OnWindowClose(Events::WindowCloseEvent& event);
    virtual bool OnWindowResize(Events::WindowResizeEvent& event);

    // Keys Events
    virtual bool OnKeyPressed(Events::KeyPressedEvent& event) { return true; }
    virtual bool OnKeyReleased(Events::KeyReleasedEvent& event) { return true; }

    // Mouse Events
    virtual bool OnMouseMoved(Events::MouseMovedEvent& event) { return true; }
    virtual bool OnMouseScrolled(Events::MouseScrolledEvent& event) { return true; }
    virtual bool OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) { return true; }
    virtual bool OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) { return true; }

private:
    static Application* sInstance;

    bool mRunning;
    SDL_Window* mWindow;
    SDL_GLContext mGLContext;
    ApplicationStateData mData;
  };
} // namespace AppCore
