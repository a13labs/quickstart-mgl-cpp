
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

  inline void add_key_translation(uint16_t src, Key::Enum tgt)
  {
    key_forward_map[src & 0xff] = (uint8_t)tgt;
    key_reverse_map[tgt & 0xff] = (uint8_t)src;
  }

  inline int from_key(Key::Enum key)
  {
    return (int)key_reverse_map[key & 0xff];
  }

  inline Key::Enum to_key(int key)
  {
    return (Key::Enum)key_forward_map[key & 0xff];
  }

  inline void clear_key_map()
  {
    std::memset(key_forward_map, 0, sizeof(key_forward_map));
    std::memset(key_reverse_map, 0, sizeof(key_reverse_map));
    std::memset(input_state.pressed_keys, 0, sizeof(input_state.pressed_keys));
    std::memset(input_state.pressed_mouse_buttons, 0, sizeof(input_state.pressed_mouse_buttons));
  }

  inline void set_key_pressed_state(Key::Enum key, bool value)
  {
    input_state.pressed_keys[key & 0xff] = value;
  }

  inline void set_mouse_button_pressed_state(MouseButton::Enum button, bool value)
  {
    input_state.pressed_mouse_buttons[button & 0xff] = value;
  }

  void init()
  {
    clear_key_map();
    add_key_translation(SDL_SCANCODE_ESCAPE, Key::Esc);
    add_key_translation(SDL_SCANCODE_RETURN, Key::Return);
    add_key_translation(SDL_SCANCODE_TAB, Key::Tab);
    add_key_translation(SDL_SCANCODE_BACKSPACE, Key::Backspace);
    add_key_translation(SDL_SCANCODE_SPACE, Key::Space);
    add_key_translation(SDL_SCANCODE_UP, Key::Up);
    add_key_translation(SDL_SCANCODE_DOWN, Key::Down);
    add_key_translation(SDL_SCANCODE_LEFT, Key::Left);
    add_key_translation(SDL_SCANCODE_RIGHT, Key::Right);
    add_key_translation(SDL_SCANCODE_PAGEUP, Key::PageUp);
    add_key_translation(SDL_SCANCODE_PAGEDOWN, Key::PageDown);
    add_key_translation(SDL_SCANCODE_HOME, Key::Home);
    add_key_translation(SDL_SCANCODE_END, Key::End);
    add_key_translation(SDL_SCANCODE_PRINTSCREEN, Key::Print);
    add_key_translation(SDL_SCANCODE_KP_PLUS, Key::Plus);
    add_key_translation(SDL_SCANCODE_EQUALS, Key::Plus);
    add_key_translation(SDL_SCANCODE_KP_MINUS, Key::Minus);
    add_key_translation(SDL_SCANCODE_MINUS, Key::Minus);
    add_key_translation(SDL_SCANCODE_GRAVE, Key::Tilde);
    add_key_translation(SDL_SCANCODE_KP_COMMA, Key::Comma);
    add_key_translation(SDL_SCANCODE_COMMA, Key::Comma);
    add_key_translation(SDL_SCANCODE_KP_PERIOD, Key::Period);
    add_key_translation(SDL_SCANCODE_PERIOD, Key::Period);
    add_key_translation(SDL_SCANCODE_SLASH, Key::Slash);
    add_key_translation(SDL_SCANCODE_F1, Key::F1);
    add_key_translation(SDL_SCANCODE_F2, Key::F2);
    add_key_translation(SDL_SCANCODE_F3, Key::F3);
    add_key_translation(SDL_SCANCODE_F4, Key::F4);
    add_key_translation(SDL_SCANCODE_F5, Key::F5);
    add_key_translation(SDL_SCANCODE_F6, Key::F6);
    add_key_translation(SDL_SCANCODE_F7, Key::F7);
    add_key_translation(SDL_SCANCODE_F8, Key::F8);
    add_key_translation(SDL_SCANCODE_F9, Key::F9);
    add_key_translation(SDL_SCANCODE_F10, Key::F10);
    add_key_translation(SDL_SCANCODE_F11, Key::F11);
    add_key_translation(SDL_SCANCODE_F12, Key::F12);
    add_key_translation(SDL_SCANCODE_KP_0, Key::NumPad0);
    add_key_translation(SDL_SCANCODE_KP_1, Key::NumPad1);
    add_key_translation(SDL_SCANCODE_KP_2, Key::NumPad2);
    add_key_translation(SDL_SCANCODE_KP_3, Key::NumPad3);
    add_key_translation(SDL_SCANCODE_KP_4, Key::NumPad4);
    add_key_translation(SDL_SCANCODE_KP_5, Key::NumPad5);
    add_key_translation(SDL_SCANCODE_KP_6, Key::NumPad6);
    add_key_translation(SDL_SCANCODE_KP_7, Key::NumPad7);
    add_key_translation(SDL_SCANCODE_KP_8, Key::NumPad8);
    add_key_translation(SDL_SCANCODE_KP_9, Key::NumPad9);
    add_key_translation(SDL_SCANCODE_KP_ENTER, Key::NumPadEnter);
    add_key_translation(SDL_SCANCODE_0, Key::Key0);
    add_key_translation(SDL_SCANCODE_1, Key::Key1);
    add_key_translation(SDL_SCANCODE_2, Key::Key2);
    add_key_translation(SDL_SCANCODE_3, Key::Key3);
    add_key_translation(SDL_SCANCODE_4, Key::Key4);
    add_key_translation(SDL_SCANCODE_5, Key::Key5);
    add_key_translation(SDL_SCANCODE_6, Key::Key6);
    add_key_translation(SDL_SCANCODE_7, Key::Key7);
    add_key_translation(SDL_SCANCODE_8, Key::Key8);
    add_key_translation(SDL_SCANCODE_9, Key::Key9);
    add_key_translation(SDL_SCANCODE_A, Key::KeyA);
    add_key_translation(SDL_SCANCODE_B, Key::KeyB);
    add_key_translation(SDL_SCANCODE_C, Key::KeyC);
    add_key_translation(SDL_SCANCODE_D, Key::KeyD);
    add_key_translation(SDL_SCANCODE_E, Key::KeyE);
    add_key_translation(SDL_SCANCODE_F, Key::KeyF);
    add_key_translation(SDL_SCANCODE_G, Key::KeyG);
    add_key_translation(SDL_SCANCODE_H, Key::KeyH);
    add_key_translation(SDL_SCANCODE_I, Key::KeyI);
    add_key_translation(SDL_SCANCODE_J, Key::KeyJ);
    add_key_translation(SDL_SCANCODE_K, Key::KeyK);
    add_key_translation(SDL_SCANCODE_L, Key::KeyL);
    add_key_translation(SDL_SCANCODE_M, Key::KeyM);
    add_key_translation(SDL_SCANCODE_N, Key::KeyN);
    add_key_translation(SDL_SCANCODE_O, Key::KeyO);
    add_key_translation(SDL_SCANCODE_P, Key::KeyP);
    add_key_translation(SDL_SCANCODE_Q, Key::KeyQ);
    add_key_translation(SDL_SCANCODE_R, Key::KeyR);
    add_key_translation(SDL_SCANCODE_S, Key::KeyS);
    add_key_translation(SDL_SCANCODE_T, Key::KeyT);
    add_key_translation(SDL_SCANCODE_U, Key::KeyU);
    add_key_translation(SDL_SCANCODE_V, Key::KeyV);
    add_key_translation(SDL_SCANCODE_W, Key::KeyW);
    add_key_translation(SDL_SCANCODE_X, Key::KeyX);
    add_key_translation(SDL_SCANCODE_Y, Key::KeyY);
    add_key_translation(SDL_SCANCODE_Z, Key::KeyZ);
    add_key_translation(SDL_SCANCODE_LALT, Key::LeftAlt);
    add_key_translation(SDL_SCANCODE_RALT, Key::RightAlt);
    add_key_translation(SDL_SCANCODE_LCTRL, Key::LeftCtrl);
    add_key_translation(SDL_SCANCODE_RCTRL, Key::RightCtrl);
    add_key_translation(SDL_SCANCODE_LSHIFT, Key::LeftShift);
    add_key_translation(SDL_SCANCODE_RSHIFT, Key::RightShift);
    add_key_translation(SDL_SCANCODE_LGUI, Key::LeftMeta);
    add_key_translation(SDL_SCANCODE_RGUI, Key::RightMeta);
  }

  void update_state(SDL_Event* event, const event_handler& handler)
  {
    switch(event->type)
    {
      case SDL_MOUSEMOTION: {
        const SDL_MouseMotionEvent& mev = event->motion;
        MouseMovedEvent event(mev.x, mev.y);
        handler(event);
      }
      break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP: {
        const SDL_MouseButtonEvent& mev = event->button;
        MouseButton::Enum button;
        switch(mev.button)
        {
          default:
          case SDL_BUTTON_LEFT: button = MouseButton::Left; break;
          case SDL_BUTTON_MIDDLE: button = MouseButton::Middle; break;
          case SDL_BUTTON_RIGHT: button = MouseButton::Right; break;
        }

        switch(mev.state)
        {
          default:
          case SDL_PRESSED: {
            set_mouse_button_pressed_state(button, true);
            MouseButtonPressedEvent event(button);
            handler(event);
          }
          break;
          case SDL_RELEASED: {
            set_mouse_button_pressed_state(button, false);
            MouseButtonReleasedEvent event(button);
            handler(event);
          }
          break;
        }
      }
      break;
      case SDL_MOUSEWHEEL: {
        const SDL_MouseWheelEvent& mev = event->wheel;
        MouseScrolledEvent event(mev.x, mev.y);
        handler(event);
      }
      break;
      case SDL_KEYDOWN: {
        const SDL_KeyboardEvent& kev = event->key;
        uint8_t modifiers = (uint8_t)(kev.keysym.mod);
        Key::Enum key = to_key(kev.keysym.scancode);

        set_key_pressed_state(key, true);
        KeyPressedEvent event(key, modifiers, kev.repeat);
        handler(event);
      }
      break;
      case SDL_KEYUP: {
        const SDL_KeyboardEvent& kev = event->key;
        uint8_t modifiers = (uint8_t)(kev.keysym.mod);
        Key::Enum key = to_key(kev.keysym.scancode);

        set_key_pressed_state(key, false);
        KeyReleasedEvent event(key, modifiers);
        handler(event);
      }
      break;
      default: break;
    }
  }

  WindowSDL::WindowSDL(const window_config& config)
  {
    m_state.current_config = config;
    m_state.width = config.width;
    m_state.height = config.height;
  }

  bool WindowSDL::create_window()
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

  void WindowSDL::destroy_window()
  {
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_native_window);
    m_context = nullptr;
    m_native_window = nullptr;
    SDL_Quit();
  }

  void WindowSDL::swap_buffers()
  {
    SDL_GL_SwapWindow(m_native_window);
  }

  void WindowSDL::set_title(const mgl_core::string& value)
  {
    m_title = value;
    SDL_SetWindowTitle(m_native_window, m_title.c_str());
  }

  const mgl_core::string& WindowSDL::title() const
  {
    return m_title;
  }

  void WindowSDL::initialize_event_handler(const event_handler& handler)
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
                  WindowResizeEvent event(w, h);
                  m_state.width = w;
                  m_state.height = h;
                  m_state.handler(event);
                }
                break;
                case SDL_WINDOWEVENT_CLOSE: {
                  WindowCloseEvent event;
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
                Window::current().quit();
              }

              if(is_key_pressed(m_state.current_config.fullscreen_key))
              {
                Window::current().toggle_full_screen();
              }
            }
            break;
            default: break;
          }

          return 0;
        },
        &m_state);
  }

  void WindowSDL::toggle_full_screen()
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

  void WindowSDL::process_events()
  {
    SDL_Event e;
    SDL_PollEvent(&e);
  }

  int WindowSDL::width()
  {
    return m_state.width;
  }

  int WindowSDL::height()
  {
    return m_state.height;
  }

  int WindowSDL::aspect_ratio()
  {
    return m_state.width / m_state.height;
  }

  mgl_core::size WindowSDL::get_drawable_size()
  {
    int x, y;
    SDL_GL_GetDrawableSize(m_native_window, &x, &y);
    return { x, y };
  }

} // namespace mgl_window
