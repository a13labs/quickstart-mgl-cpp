
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
#include "mgl_core/log.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"
// #include "glad/glad.h"

namespace mgl_window
{
  static uint8_t key_forward_map[256];
  static uint8_t key_reverse_map[256];

  inline void add_key_translation(uint16_t src, key::name tgt)
  {
    key_forward_map[src & 0xff] = (uint8_t)tgt;
    key_reverse_map[tgt & 0xff] = (uint8_t)src;
  }

  inline int from_key(key::name key)
  {
    return (int)key_reverse_map[key & 0xff];
  }

  inline key::name to_key(int key)
  {
    return (key::name)key_forward_map[key & 0xff];
  }

  inline void clear_key_map()
  {
    std::memset(key_forward_map, 0, sizeof(key_forward_map));
    std::memset(key_reverse_map, 0, sizeof(key_reverse_map));
    std::memset(s_input_state.pressed_keys, 0, sizeof(s_input_state.pressed_keys));
    std::memset(s_input_state.pressed_mouse_buttons, 0, sizeof(s_input_state.pressed_mouse_buttons));
  }

  inline void set_key_pressed_state(key::name key, bool value)
  {
    s_input_state.pressed_keys[key & 0xff] = value;
  }

  inline void set_mouse_button_pressed_state(mouse_button::name button, bool value)
  {
    s_input_state.pressed_mouse_buttons[button & 0xff] = value;
  }

  void init()
  {
    clear_key_map();
    add_key_translation(SDL_SCANCODE_ESCAPE, key::Esc);
    add_key_translation(SDL_SCANCODE_RETURN, key::Return);
    add_key_translation(SDL_SCANCODE_TAB, key::Tab);
    add_key_translation(SDL_SCANCODE_BACKSPACE, key::Backspace);
    add_key_translation(SDL_SCANCODE_SPACE, key::Space);
    add_key_translation(SDL_SCANCODE_UP, key::Up);
    add_key_translation(SDL_SCANCODE_DOWN, key::Down);
    add_key_translation(SDL_SCANCODE_LEFT, key::Left);
    add_key_translation(SDL_SCANCODE_RIGHT, key::Right);
    add_key_translation(SDL_SCANCODE_PAGEUP, key::PageUp);
    add_key_translation(SDL_SCANCODE_PAGEDOWN, key::PageDown);
    add_key_translation(SDL_SCANCODE_HOME, key::Home);
    add_key_translation(SDL_SCANCODE_END, key::End);
    add_key_translation(SDL_SCANCODE_PRINTSCREEN, key::Print);
    add_key_translation(SDL_SCANCODE_KP_PLUS, key::Plus);
    add_key_translation(SDL_SCANCODE_EQUALS, key::Plus);
    add_key_translation(SDL_SCANCODE_KP_MINUS, key::Minus);
    add_key_translation(SDL_SCANCODE_MINUS, key::Minus);
    add_key_translation(SDL_SCANCODE_GRAVE, key::Tilde);
    add_key_translation(SDL_SCANCODE_KP_COMMA, key::Comma);
    add_key_translation(SDL_SCANCODE_COMMA, key::Comma);
    add_key_translation(SDL_SCANCODE_KP_PERIOD, key::Period);
    add_key_translation(SDL_SCANCODE_PERIOD, key::Period);
    add_key_translation(SDL_SCANCODE_SLASH, key::Slash);
    add_key_translation(SDL_SCANCODE_F1, key::F1);
    add_key_translation(SDL_SCANCODE_F2, key::F2);
    add_key_translation(SDL_SCANCODE_F3, key::F3);
    add_key_translation(SDL_SCANCODE_F4, key::F4);
    add_key_translation(SDL_SCANCODE_F5, key::F5);
    add_key_translation(SDL_SCANCODE_F6, key::F6);
    add_key_translation(SDL_SCANCODE_F7, key::F7);
    add_key_translation(SDL_SCANCODE_F8, key::F8);
    add_key_translation(SDL_SCANCODE_F9, key::F9);
    add_key_translation(SDL_SCANCODE_F10, key::F10);
    add_key_translation(SDL_SCANCODE_F11, key::F11);
    add_key_translation(SDL_SCANCODE_F12, key::F12);
    add_key_translation(SDL_SCANCODE_KP_0, key::NumPad0);
    add_key_translation(SDL_SCANCODE_KP_1, key::NumPad1);
    add_key_translation(SDL_SCANCODE_KP_2, key::NumPad2);
    add_key_translation(SDL_SCANCODE_KP_3, key::NumPad3);
    add_key_translation(SDL_SCANCODE_KP_4, key::NumPad4);
    add_key_translation(SDL_SCANCODE_KP_5, key::NumPad5);
    add_key_translation(SDL_SCANCODE_KP_6, key::NumPad6);
    add_key_translation(SDL_SCANCODE_KP_7, key::NumPad7);
    add_key_translation(SDL_SCANCODE_KP_8, key::NumPad8);
    add_key_translation(SDL_SCANCODE_KP_9, key::NumPad9);
    add_key_translation(SDL_SCANCODE_KP_ENTER, key::NumPadEnter);
    add_key_translation(SDL_SCANCODE_0, key::Key0);
    add_key_translation(SDL_SCANCODE_1, key::Key1);
    add_key_translation(SDL_SCANCODE_2, key::Key2);
    add_key_translation(SDL_SCANCODE_3, key::Key3);
    add_key_translation(SDL_SCANCODE_4, key::Key4);
    add_key_translation(SDL_SCANCODE_5, key::Key5);
    add_key_translation(SDL_SCANCODE_6, key::Key6);
    add_key_translation(SDL_SCANCODE_7, key::Key7);
    add_key_translation(SDL_SCANCODE_8, key::Key8);
    add_key_translation(SDL_SCANCODE_9, key::Key9);
    add_key_translation(SDL_SCANCODE_A, key::KeyA);
    add_key_translation(SDL_SCANCODE_B, key::KeyB);
    add_key_translation(SDL_SCANCODE_C, key::KeyC);
    add_key_translation(SDL_SCANCODE_D, key::KeyD);
    add_key_translation(SDL_SCANCODE_E, key::KeyE);
    add_key_translation(SDL_SCANCODE_F, key::KeyF);
    add_key_translation(SDL_SCANCODE_G, key::KeyG);
    add_key_translation(SDL_SCANCODE_H, key::KeyH);
    add_key_translation(SDL_SCANCODE_I, key::KeyI);
    add_key_translation(SDL_SCANCODE_J, key::KeyJ);
    add_key_translation(SDL_SCANCODE_K, key::KeyK);
    add_key_translation(SDL_SCANCODE_L, key::KeyL);
    add_key_translation(SDL_SCANCODE_M, key::KeyM);
    add_key_translation(SDL_SCANCODE_N, key::KeyN);
    add_key_translation(SDL_SCANCODE_O, key::KeyO);
    add_key_translation(SDL_SCANCODE_P, key::KeyP);
    add_key_translation(SDL_SCANCODE_Q, key::KeyQ);
    add_key_translation(SDL_SCANCODE_R, key::KeyR);
    add_key_translation(SDL_SCANCODE_S, key::KeyS);
    add_key_translation(SDL_SCANCODE_T, key::KeyT);
    add_key_translation(SDL_SCANCODE_U, key::KeyU);
    add_key_translation(SDL_SCANCODE_V, key::KeyV);
    add_key_translation(SDL_SCANCODE_W, key::KeyW);
    add_key_translation(SDL_SCANCODE_X, key::KeyX);
    add_key_translation(SDL_SCANCODE_Y, key::KeyY);
    add_key_translation(SDL_SCANCODE_Z, key::KeyZ);
    add_key_translation(SDL_SCANCODE_LALT, key::LeftAlt);
    add_key_translation(SDL_SCANCODE_RALT, key::RightAlt);
    add_key_translation(SDL_SCANCODE_LCTRL, key::LeftCtrl);
    add_key_translation(SDL_SCANCODE_RCTRL, key::RightCtrl);
    add_key_translation(SDL_SCANCODE_LSHIFT, key::LeftShift);
    add_key_translation(SDL_SCANCODE_RSHIFT, key::RightShift);
    add_key_translation(SDL_SCANCODE_LGUI, key::LeftMeta);
    add_key_translation(SDL_SCANCODE_RGUI, key::RightMeta);
  }

  void update_state(SDL_Event* event, const event_handler& handler)
  {
    switch(event->type)
    {
      case SDL_MOUSEMOTION: {
        const SDL_MouseMotionEvent& mev = event->motion;
        mouse_moved_event event(mev.x, mev.y);
        handler(event);
      }
      break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP: {
        const SDL_MouseButtonEvent& mev = event->button;
        mouse_button::name button;
        switch(mev.button)
        {
          default:
          case SDL_BUTTON_LEFT: button = mouse_button::Left; break;
          case SDL_BUTTON_MIDDLE: button = mouse_button::Middle; break;
          case SDL_BUTTON_RIGHT: button = mouse_button::Right; break;
        }

        switch(mev.state)
        {
          default:
          case SDL_PRESSED: {
            set_mouse_button_pressed_state(button, true);
            mouse_button_pressed_event event(button);
            handler(event);
          }
          break;
          case SDL_RELEASED: {
            set_mouse_button_pressed_state(button, false);
            mouse_button_released_event event(button);
            handler(event);
          }
          break;
        }
      }
      break;
      case SDL_MOUSEWHEEL: {
        const SDL_MouseWheelEvent& mev = event->wheel;
        mouse_scrolled_event event(mev.x, mev.y);
        handler(event);
      }
      break;
      case SDL_KEYDOWN: {
        const SDL_KeyboardEvent& kev = event->key;
        uint8_t modifiers = (uint8_t)(kev.keysym.mod);
        key::name key = to_key(kev.keysym.scancode);

        set_key_pressed_state(key, true);
        key_pressed_event event(key, modifiers, kev.repeat);
        handler(event);
      }
      break;
      case SDL_KEYUP: {
        const SDL_KeyboardEvent& kev = event->key;
        uint8_t modifiers = (uint8_t)(kev.keysym.mod);
        key::name key = to_key(kev.keysym.scancode);

        set_key_pressed_state(key, false);
        key_released_event event(key, modifiers);
        handler(event);
      }
      break;
      default: break;
    }
  }

  sdl_window::sdl_window(const window_config& config)
  {
    m_state.current_config = config;
    m_state.width = config.width;
    m_state.height = config.height;
  }

  bool sdl_window::create_window()
  {
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      MGL_CORE_TRACE("BaseWindow: Error initializing SDL");
      return false;
    }

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

    MGL_CORE_INFO(
        "BaseWindow: Creating window {0},{1} with OpenGL support", m_state.current_config.width, m_state.current_config.height);
    m_native_window = SDL_CreateWindow(m_state.current_config.title.c_str(),
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       m_state.current_config.width,
                                       m_state.current_config.height,
                                       flags);

    if(!m_native_window)
    {
      auto error = SDL_GetError();
      MGL_CORE_TRACE("BaseWindow: Error creating window, '{0}'.", error);
      SDL_Quit();
      return false;
    }

    m_context = SDL_GL_CreateContext(m_native_window);
    if(!m_context)
    {
      auto error = SDL_GetError();
      MGL_CORE_TRACE("BaseWindow: Error creating OpenGL context, '{0}'.", error);
      SDL_DestroyWindow(m_native_window);
      m_native_window = nullptr;
      SDL_Quit();
      return false;
    }

    SDL_GL_SetSwapInterval(m_state.current_config.VSync ? 1 : 0);

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);

    if(!SDL_GetWindowWMInfo(m_native_window, &wmi))
    {
      auto error = SDL_GetError();
      MGL_CORE_TRACE("BaseWindow: Error retrieving window information: {0}.", error);
      SDL_GL_DeleteContext(m_context);
      SDL_DestroyWindow(m_native_window);
      m_context = nullptr;
      m_native_window = nullptr;
      return false;
    }

    m_state.width = m_state.current_config.width;
    m_state.height = m_state.current_config.height;
    m_title = m_state.current_config.title;

    return true;
  }

  void sdl_window::destroy_window()
  {
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_native_window);
    m_context = nullptr;
    m_native_window = nullptr;
    SDL_Quit();
  }

  void sdl_window::swap_buffers()
  {
    SDL_GL_SwapWindow(m_native_window);
  }

  void sdl_window::set_title(const mgl_core::string& value)
  {
    m_title = value;
    SDL_SetWindowTitle(m_native_window, m_title.c_str());
  }

  const mgl_core::string& sdl_window::title() const
  {
    return m_title;
  }

  void sdl_window::initialize_event_handler(const event_handler& handler)
  {
    m_state.handler = handler;
    init();
    SDL_AddEventWatch(
        [](void* userdata, SDL_Event* currentEvent) -> int {
          window_state& m_state = *(window_state*)userdata;
          switch(currentEvent->type)
          {
            case SDL_WINDOWEVENT:
              switch(currentEvent->window.event)
              {
                case SDL_WINDOWEVENT_RESIZED: {
                  auto w = currentEvent->window.data1;
                  auto h = currentEvent->window.data2;
                  window_resize_event event(w, h);
                  m_state.width = w;
                  m_state.height = h;
                  m_state.handler(event);
                }
                break;
                case SDL_WINDOWEVENT_CLOSE: {
                  window_close_event event;
                  m_state.handler(event);
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
              update_state(currentEvent, m_state.handler);

              if(is_key_pressed(m_state.current_config.exit_key))
              {
                window::current().quit();
              }

              if(is_key_pressed(m_state.current_config.fullscreen_key))
              {
                window::current().toggle_full_screen();
              }
            }
            break;
            default: break;
          }

          return 0;
        },
        &m_state);
  }

  void sdl_window::toggle_full_screen()
  {
    m_state.fullscreen = !m_state.fullscreen;
    if(m_state.fullscreen)
    {
      SDL_SetWindowFullscreen(m_native_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
      return;
    }

    SDL_RestoreWindow(m_native_window);
    SDL_SetWindowSize(m_native_window, m_state.current_config.width, m_state.current_config.height);
    SDL_SetWindowPosition(m_native_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowFullscreen(m_native_window, 0);
  }

  void sdl_window::process_events()
  {
    SDL_Event e;
    SDL_PollEvent(&e);
  }

  int sdl_window::width()
  {
    return m_state.width;
  }

  int sdl_window::height()
  {
    return m_state.height;
  }

  float sdl_window::aspect_ratio()
  {
    return static_cast<float>(m_state.width) / m_state.height;
  }

  mgl_core::size sdl_window::get_drawable_size()
  {
    int x, y;
    SDL_GL_GetDrawableSize(m_native_window, &x, &y);
    return { x, y };
  }

} // namespace mgl_window
