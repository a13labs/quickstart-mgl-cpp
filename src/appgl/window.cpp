
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
#include "window.hpp"
#include "appcore/log.hpp"
#include "context.hpp"

#include "SDL2/SDL_syswm.h"
// #include "glad/glad.h"

namespace AppGL
{

  bool Window::create_window()
  {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    if(m_state.current_config.samples > 1)
    {
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
      SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, m_state.current_config.samples);
    }

    auto flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;

    m_state.fullscreen = m_state.current_config.fullscreen;
    if(m_state.current_config.fullscreen)
    {
      flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    else
    {
      if(m_state.current_config.resizable)
      {
        flags |= SDL_WINDOW_RESIZABLE;
      }
    }

    APPCORE_INFO(
        "BaseWindow: Creating window {0},{1} with OpenGL support", m_state.current_config.width, m_state.current_config.height);
    m_state.native_window = SDL_CreateWindow(m_state.current_config.title.c_str(),
                                             SDL_WINDOWPOS_CENTERED,
                                             SDL_WINDOWPOS_CENTERED,
                                             m_state.current_config.width,
                                             m_state.current_config.height,
                                             flags);

    if(!m_state.native_window)
    {
      auto error = SDL_GetError();
      APPCORE_TRACE("BaseWindow: Error creating window, '{0}'.", error);
      return false;
    }

    m_context = SDL_GL_CreateContext(m_state.native_window);
    if(!m_context)
    {
      auto error = SDL_GetError();
      APPCORE_TRACE("BaseWindow: Error creating OpenGL context, '{0}'.", error);
      SDL_DestroyWindow(m_state.native_window);
      m_state.native_window = nullptr;
      return false;
    }

    SDL_GL_SetSwapInterval(m_state.current_config.VSync ? 1 : 0);

    m_shared_context = Context::create_context(ContextMode::SHARE, 330);

    if(!m_shared_context)
    {
      APPCORE_TRACE("Window: Error initializing GL shared context.");
      SDL_GL_DeleteContext(m_context);
      SDL_DestroyWindow(m_state.native_window);
      m_shared_context = nullptr;
      m_context = nullptr;
      m_state.native_window = nullptr;
      return false;
    }

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);

    if(!SDL_GetWindowWMInfo(m_state.native_window, &wmi))
    {
      auto error = SDL_GetError();
      APPCORE_TRACE("BaseWindow: Error retrieving window information: {0}.", error);
      m_shared_context->release();
      SDL_GL_DeleteContext(m_context);
      SDL_DestroyWindow(m_state.native_window);
      m_shared_context = nullptr;
      m_context = nullptr;
      m_state.native_window = nullptr;
      return false;
    }

    m_state.width = m_state.current_config.width;
    m_state.height = m_state.current_config.height;
    m_state.aspect_ratio = m_state.width / m_state.height;

    return true;
  }

  void Window::destroy_window()
  {
    m_shared_context->release();
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_state.native_window);
    m_shared_context = nullptr;
    m_context = nullptr;
    m_state.native_window = nullptr;
  }

  void Window::swap_buffers()
  {
    SDL_GL_SwapWindow(m_state.native_window);
  }

  bool Window::on_window_resize(AppCore::Events::WindowResizeEvent& event)
  {
    return BaseWindow::on_window_resize(event);
  }

} // namespace AppGL
