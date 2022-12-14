
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
#include "application.hpp"
#include "input.hpp"
#include "log.hpp"

#include "SDL2/SDL_syswm.h"

namespace AppCore
{
  namespace Application
  {
    BaseWindow* BaseWindow::sInstance = nullptr;

    BaseWindow::BaseWindow(const WindowConfig& config)
    {
      APPCORE_ASSERT(!sInstance, "BaseWindow already running!");
      Log::Init();

      mState.CurrentConfig = config;
      mState.NativeWindow = nullptr;
      sInstance = this;
      mRunning = false;
    }

    void BaseWindow::OnEvent(Events::Event& event)
    {
      Events::EventDispatcher dispatcher(event);

      // Dispatch Windows Events
      dispatcher.Dispatch<Events::WindowCloseEvent>(
          APPCORE_BIND_EVENT_FN(BaseWindow::OnWindowClose));
      dispatcher.Dispatch<Events::WindowResizeEvent>(
          APPCORE_BIND_EVENT_FN(BaseWindow::OnWindowResize));

      // Dispatch key events to be handled by the application
      dispatcher.Dispatch<Events::KeyPressedEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::OnKeyPressed));
      dispatcher.Dispatch<Events::KeyReleasedEvent>(
          APPCORE_BIND_EVENT_FN(BaseWindow::OnKeyReleased));

      // Dispatch mouse events to be handled by the application
      dispatcher.Dispatch<Events::MouseMovedEvent>(APPCORE_BIND_EVENT_FN(BaseWindow::OnMouseMoved));
      dispatcher.Dispatch<Events::MouseScrolledEvent>(
          APPCORE_BIND_EVENT_FN(BaseWindow::OnMouseScrolled));
      dispatcher.Dispatch<Events::MouseButtonPressedEvent>(
          APPCORE_BIND_EVENT_FN(BaseWindow::OnMouseButtonPressed));
      dispatcher.Dispatch<Events::MouseButtonReleasedEvent>(
          APPCORE_BIND_EVENT_FN(BaseWindow::OnMouseButtonReleased));
    }

    bool BaseWindow::OnWindowClose(Events::WindowCloseEvent& event)
    {
      mRunning = false;
      return true;
    }

    void BaseWindow::Run()
    {
      if(mRunning)
        return;

      if(SDL_Init(SDL_INIT_VIDEO) < 0)
      {
        APPCORE_TRACE("BaseWindow: Error initializing SDL");
        return;
      }

      if(!CreateWindow())
      {
        APPCORE_TRACE("BaseWindow: Error creating Window");
        SDL_Quit();
        return;
      }

      Input::Init();
      mState.Handler = APPCORE_BIND_EVENT_FN(BaseWindow::OnEvent);

      SDL_AddEventWatch(
          [](void* userdata, SDL_Event* currentEvent) -> int {
            WindowState& mState = *(WindowState*)userdata;
            switch(currentEvent->type)
            {
              case SDL_WINDOWEVENT:
                switch(currentEvent->window.event)
                {
                  case SDL_WINDOWEVENT_RESIZED: {
                    auto w = currentEvent->window.data1;
                    auto h = currentEvent->window.data2;
                    Events::WindowResizeEvent event(w, h);
                    mState.Handler(event);
                  }
                  break;
                  case SDL_WINDOWEVENT_CLOSE: {
                    Events::WindowCloseEvent event;
                    mState.Handler(event);
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
                Input::UpdateState(currentEvent, mState.Handler);

                if(Input::IsKeyPressed(mState.CurrentConfig.ExitKey))
                {
                  BaseWindow::Current().Quit();
                }

                if(Input::IsKeyPressed(mState.CurrentConfig.FullScreenKey))
                {
                  BaseWindow::Current().ToggleFullScreen();
                }
              }
              break;
              default: break;
            }

            return 0;
          },
          &mState);

      mRunning = true;

      APPCORE_PROFILE_BEGIN_SESSION();

      while(mRunning)
      {
        SDL_Event e;
        SDL_PollEvent(&e);
        Draw();
        SwapBuffers();
#if APPCORE_PROFILE
        // Since we are profiling we just render one frame
        mRunning = false;
#endif
      }

      APPCORE_PROFILE_END_SESSION();
      DestroyWindow();
      SDL_Quit();
    }

    void BaseWindow::Quit()
    {
      mRunning = false;
    }

    void BaseWindow::ToggleFullScreen()
    {
      mState.Fullscreen = !mState.Fullscreen;
      if(mState.Fullscreen)
      {
        SDL_SetWindowFullscreen(mState.NativeWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
        return;
      }

      SDL_RestoreWindow(mState.NativeWindow);
      SDL_SetWindowSize(
          mState.NativeWindow, mState.CurrentConfig.Width, mState.CurrentConfig.Height);
      SDL_SetWindowPosition(mState.NativeWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
      SDL_SetWindowFullscreen(mState.NativeWindow, 0);
    }

    WindowConfig LoadWindowConfiguration(const String& filename)
    {
      // TODO: Implement load from JSON
      return WindowConfig();
    }
  } // namespace Application
} // namespace AppCore
