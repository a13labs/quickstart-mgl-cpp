
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
#include "window.hpp"
#include "appcore/log.hpp"

#include "SDL2/SDL_syswm.h"
// #include "glad/glad.h"

namespace AppGL
{

  bool Window::CreateWindow()
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    if(mState.CurrentConfig.Samples > 1)
    {
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, mState.CurrentConfig.Samples);
    }

    auto flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;

    mState.Fullscreen = mState.CurrentConfig.Fullscreen;
    if(mState.CurrentConfig.Fullscreen)
    {
      flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    else
    {
      if(mState.CurrentConfig.Resizable)
      {
        flags |= SDL_WINDOW_RESIZABLE;
      }
    }

    APPCORE_INFO("BaseWindow: Creating window {0},{1} with OpenGL support",
                 mState.CurrentConfig.Width,
                 mState.CurrentConfig.Height);
    mState.NativeWindow = SDL_CreateWindow(mState.CurrentConfig.Title.c_str(),
                                           SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED,
                                           mState.CurrentConfig.Width,
                                           mState.CurrentConfig.Height,
                                           flags);

    if(!mState.NativeWindow)
    {
      auto error = SDL_GetError();
      APPCORE_TRACE("BaseWindow: Error creating window, '{0}'.", error);
      return false;
    }

    mContext = SDL_GL_CreateContext(mState.NativeWindow);
    if(!mContext)
    {
      auto error = SDL_GetError();
      APPCORE_TRACE("BaseWindow: Error creating OpenGL context, '{0}'.", error);
      SDL_DestroyWindow(mState.NativeWindow);
      mState.NativeWindow = nullptr;
      return false;
    }

    SDL_GL_SetSwapInterval(mState.CurrentConfig.VSync ? 1 : 0);

    mSharedContext = Context::Create(ContextMode::Share, 330);

    if(!mSharedContext)
    {
      APPCORE_TRACE("Window: Error initializing GL shared context.");
      SDL_GL_DeleteContext(mContext);
      SDL_DestroyWindow(mState.NativeWindow);
      mSharedContext = nullptr;
      mContext = nullptr;
      mState.NativeWindow = nullptr;
      return false;
    }

    // if(!gladLoadGL())
    // {
    //   APPCORE_TRACE("BaseWindow: Error initializing GL extensions.");
    //   SDL_GL_DeleteContext(mContext);
    //   SDL_DestroyWindow(mState.NativeWindow);
    //   mContext = nullptr;
    //   mState.NativeWindow = nullptr;
    //   return false;
    // }

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);

    if(!SDL_GetWindowWMInfo(mState.NativeWindow, &wmi))
    {
      auto error = SDL_GetError();
      APPCORE_TRACE("BaseWindow: Error retrieving window information: {0}.", error);
      mSharedContext->Release();
      SDL_GL_DeleteContext(mContext);
      SDL_DestroyWindow(mState.NativeWindow);
      mSharedContext = nullptr;
      mContext = nullptr;
      mState.NativeWindow = nullptr;
      return false;
    }

    return true;
  }

  void Window::DestroyWindow()
  {
    mSharedContext->Release();
    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mState.NativeWindow);
    mSharedContext = nullptr;
    mContext = nullptr;
    mState.NativeWindow = nullptr;
  }

  void Window::SwapBuffers()
  {
    SDL_GL_SwapWindow(mState.NativeWindow);
  }

  bool Window::OnWindowResize(AppCore::Events::WindowResizeEvent& event)
  {
    return true;
  }

} // namespace AppGL
