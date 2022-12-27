
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
#include "appcore/log.hpp"
#include "appgl/context.hpp"
#include "appgl/window.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"
// #include "glad/glad.h"

namespace AppGL
{
  static uint8_t key_forward_map[256];
  static uint8_t key_reverse_map[256];

  inline void add_key_translation(uint16_t src, AppCore::Input::Key::Enum tgt)
  {
    key_forward_map[src & 0xff] = (uint8_t)tgt;
    key_reverse_map[tgt & 0xff] = (uint8_t)src;
  }

  inline int from_key(AppCore::Input::Key::Enum key)
  {
    return (int)key_reverse_map[key & 0xff];
  }

  inline AppCore::Input::Key::Enum to_key(int key)
  {
    return (AppCore::Input::Key::Enum)key_forward_map[key & 0xff];
  }

  inline void clear_key_map()
  {
    std::memset(key_forward_map, 0, sizeof(key_forward_map));
    std::memset(key_reverse_map, 0, sizeof(key_reverse_map));
    std::memset(AppCore::Input::input_state.pressed_keys, 0, sizeof(AppCore::Input::input_state.pressed_keys));
    std::memset(AppCore::Input::input_state.pressed_mouse_buttons, 0, sizeof(AppCore::Input::input_state.pressed_mouse_buttons));
  }

  inline void set_key_pressed_state(AppCore::Input::Key::Enum key, bool value)
  {
    AppCore::Input::input_state.pressed_keys[key & 0xff] = value;
  }

  inline void set_mouse_button_pressed_state(AppCore::Input::MouseButton::Enum button, bool value)
  {
    AppCore::Input::input_state.pressed_mouse_buttons[button & 0xff] = value;
  }

  void init()
  {
    clear_key_map();
    add_key_translation(SDL_SCANCODE_ESCAPE, AppCore::Input::Key::Esc);
    add_key_translation(SDL_SCANCODE_RETURN, AppCore::Input::Key::Return);
    add_key_translation(SDL_SCANCODE_TAB, AppCore::Input::Key::Tab);
    add_key_translation(SDL_SCANCODE_BACKSPACE, AppCore::Input::Key::Backspace);
    add_key_translation(SDL_SCANCODE_SPACE, AppCore::Input::Key::Space);
    add_key_translation(SDL_SCANCODE_UP, AppCore::Input::Key::Up);
    add_key_translation(SDL_SCANCODE_DOWN, AppCore::Input::Key::Down);
    add_key_translation(SDL_SCANCODE_LEFT, AppCore::Input::Key::Left);
    add_key_translation(SDL_SCANCODE_RIGHT, AppCore::Input::Key::Right);
    add_key_translation(SDL_SCANCODE_PAGEUP, AppCore::Input::Key::PageUp);
    add_key_translation(SDL_SCANCODE_PAGEDOWN, AppCore::Input::Key::PageDown);
    add_key_translation(SDL_SCANCODE_HOME, AppCore::Input::Key::Home);
    add_key_translation(SDL_SCANCODE_END, AppCore::Input::Key::End);
    add_key_translation(SDL_SCANCODE_PRINTSCREEN, AppCore::Input::Key::Print);
    add_key_translation(SDL_SCANCODE_KP_PLUS, AppCore::Input::Key::Plus);
    add_key_translation(SDL_SCANCODE_EQUALS, AppCore::Input::Key::Plus);
    add_key_translation(SDL_SCANCODE_KP_MINUS, AppCore::Input::Key::Minus);
    add_key_translation(SDL_SCANCODE_MINUS, AppCore::Input::Key::Minus);
    add_key_translation(SDL_SCANCODE_GRAVE, AppCore::Input::Key::Tilde);
    add_key_translation(SDL_SCANCODE_KP_COMMA, AppCore::Input::Key::Comma);
    add_key_translation(SDL_SCANCODE_COMMA, AppCore::Input::Key::Comma);
    add_key_translation(SDL_SCANCODE_KP_PERIOD, AppCore::Input::Key::Period);
    add_key_translation(SDL_SCANCODE_PERIOD, AppCore::Input::Key::Period);
    add_key_translation(SDL_SCANCODE_SLASH, AppCore::Input::Key::Slash);
    add_key_translation(SDL_SCANCODE_F1, AppCore::Input::Key::F1);
    add_key_translation(SDL_SCANCODE_F2, AppCore::Input::Key::F2);
    add_key_translation(SDL_SCANCODE_F3, AppCore::Input::Key::F3);
    add_key_translation(SDL_SCANCODE_F4, AppCore::Input::Key::F4);
    add_key_translation(SDL_SCANCODE_F5, AppCore::Input::Key::F5);
    add_key_translation(SDL_SCANCODE_F6, AppCore::Input::Key::F6);
    add_key_translation(SDL_SCANCODE_F7, AppCore::Input::Key::F7);
    add_key_translation(SDL_SCANCODE_F8, AppCore::Input::Key::F8);
    add_key_translation(SDL_SCANCODE_F9, AppCore::Input::Key::F9);
    add_key_translation(SDL_SCANCODE_F10, AppCore::Input::Key::F10);
    add_key_translation(SDL_SCANCODE_F11, AppCore::Input::Key::F11);
    add_key_translation(SDL_SCANCODE_F12, AppCore::Input::Key::F12);
    add_key_translation(SDL_SCANCODE_KP_0, AppCore::Input::Key::NumPad0);
    add_key_translation(SDL_SCANCODE_KP_1, AppCore::Input::Key::NumPad1);
    add_key_translation(SDL_SCANCODE_KP_2, AppCore::Input::Key::NumPad2);
    add_key_translation(SDL_SCANCODE_KP_3, AppCore::Input::Key::NumPad3);
    add_key_translation(SDL_SCANCODE_KP_4, AppCore::Input::Key::NumPad4);
    add_key_translation(SDL_SCANCODE_KP_5, AppCore::Input::Key::NumPad5);
    add_key_translation(SDL_SCANCODE_KP_6, AppCore::Input::Key::NumPad6);
    add_key_translation(SDL_SCANCODE_KP_7, AppCore::Input::Key::NumPad7);
    add_key_translation(SDL_SCANCODE_KP_8, AppCore::Input::Key::NumPad8);
    add_key_translation(SDL_SCANCODE_KP_9, AppCore::Input::Key::NumPad9);
    add_key_translation(SDL_SCANCODE_KP_ENTER, AppCore::Input::Key::NumPadEnter);
    add_key_translation(SDL_SCANCODE_0, AppCore::Input::Key::Key0);
    add_key_translation(SDL_SCANCODE_1, AppCore::Input::Key::Key1);
    add_key_translation(SDL_SCANCODE_2, AppCore::Input::Key::Key2);
    add_key_translation(SDL_SCANCODE_3, AppCore::Input::Key::Key3);
    add_key_translation(SDL_SCANCODE_4, AppCore::Input::Key::Key4);
    add_key_translation(SDL_SCANCODE_5, AppCore::Input::Key::Key5);
    add_key_translation(SDL_SCANCODE_6, AppCore::Input::Key::Key6);
    add_key_translation(SDL_SCANCODE_7, AppCore::Input::Key::Key7);
    add_key_translation(SDL_SCANCODE_8, AppCore::Input::Key::Key8);
    add_key_translation(SDL_SCANCODE_9, AppCore::Input::Key::Key9);
    add_key_translation(SDL_SCANCODE_A, AppCore::Input::Key::KeyA);
    add_key_translation(SDL_SCANCODE_B, AppCore::Input::Key::KeyB);
    add_key_translation(SDL_SCANCODE_C, AppCore::Input::Key::KeyC);
    add_key_translation(SDL_SCANCODE_D, AppCore::Input::Key::KeyD);
    add_key_translation(SDL_SCANCODE_E, AppCore::Input::Key::KeyE);
    add_key_translation(SDL_SCANCODE_F, AppCore::Input::Key::KeyF);
    add_key_translation(SDL_SCANCODE_G, AppCore::Input::Key::KeyG);
    add_key_translation(SDL_SCANCODE_H, AppCore::Input::Key::KeyH);
    add_key_translation(SDL_SCANCODE_I, AppCore::Input::Key::KeyI);
    add_key_translation(SDL_SCANCODE_J, AppCore::Input::Key::KeyJ);
    add_key_translation(SDL_SCANCODE_K, AppCore::Input::Key::KeyK);
    add_key_translation(SDL_SCANCODE_L, AppCore::Input::Key::KeyL);
    add_key_translation(SDL_SCANCODE_M, AppCore::Input::Key::KeyM);
    add_key_translation(SDL_SCANCODE_N, AppCore::Input::Key::KeyN);
    add_key_translation(SDL_SCANCODE_O, AppCore::Input::Key::KeyO);
    add_key_translation(SDL_SCANCODE_P, AppCore::Input::Key::KeyP);
    add_key_translation(SDL_SCANCODE_Q, AppCore::Input::Key::KeyQ);
    add_key_translation(SDL_SCANCODE_R, AppCore::Input::Key::KeyR);
    add_key_translation(SDL_SCANCODE_S, AppCore::Input::Key::KeyS);
    add_key_translation(SDL_SCANCODE_T, AppCore::Input::Key::KeyT);
    add_key_translation(SDL_SCANCODE_U, AppCore::Input::Key::KeyU);
    add_key_translation(SDL_SCANCODE_V, AppCore::Input::Key::KeyV);
    add_key_translation(SDL_SCANCODE_W, AppCore::Input::Key::KeyW);
    add_key_translation(SDL_SCANCODE_X, AppCore::Input::Key::KeyX);
    add_key_translation(SDL_SCANCODE_Y, AppCore::Input::Key::KeyY);
    add_key_translation(SDL_SCANCODE_Z, AppCore::Input::Key::KeyZ);
    add_key_translation(SDL_SCANCODE_LALT, AppCore::Input::Key::LeftAlt);
    add_key_translation(SDL_SCANCODE_RALT, AppCore::Input::Key::RightAlt);
    add_key_translation(SDL_SCANCODE_LCTRL, AppCore::Input::Key::LeftCtrl);
    add_key_translation(SDL_SCANCODE_RCTRL, AppCore::Input::Key::RightCtrl);
    add_key_translation(SDL_SCANCODE_LSHIFT, AppCore::Input::Key::LeftShift);
    add_key_translation(SDL_SCANCODE_RSHIFT, AppCore::Input::Key::RightShift);
    add_key_translation(SDL_SCANCODE_LGUI, AppCore::Input::Key::LeftMeta);
    add_key_translation(SDL_SCANCODE_RGUI, AppCore::Input::Key::RightMeta);
  }

  void update_state(SDL_Event* event, const AppCore::EventHandler& handler)
  {
    switch(event->type)
    {
      case SDL_MOUSEMOTION: {
        const SDL_MouseMotionEvent& mev = event->motion;
        AppCore::Events::MouseMovedEvent event(mev.x, mev.y);
        handler(event);
      }
      break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP: {
        const SDL_MouseButtonEvent& mev = event->button;
        AppCore::Input::MouseButton::Enum button;
        switch(mev.button)
        {
          default:
          case SDL_BUTTON_LEFT: button = AppCore::Input::MouseButton::Left; break;
          case SDL_BUTTON_MIDDLE: button = AppCore::Input::MouseButton::Middle; break;
          case SDL_BUTTON_RIGHT: button = AppCore::Input::MouseButton::Right; break;
        }

        switch(mev.state)
        {
          default:
          case SDL_PRESSED: {
            set_mouse_button_pressed_state(button, true);
            AppCore::Events::MouseButtonPressedEvent event(button);
            handler(event);
          }
          break;
          case SDL_RELEASED: {
            set_mouse_button_pressed_state(button, false);
            AppCore::Events::MouseButtonReleasedEvent event(button);
            handler(event);
          }
          break;
        }
      }
      break;
      case SDL_MOUSEWHEEL: {
        const SDL_MouseWheelEvent& mev = event->wheel;
        AppCore::Events::MouseScrolledEvent event(mev.x, mev.y);
        handler(event);
      }
      break;
      case SDL_KEYDOWN: {
        const SDL_KeyboardEvent& kev = event->key;
        uint8_t modifiers = (uint8_t)(kev.keysym.mod);
        AppCore::Input::Key::Enum key = to_key(kev.keysym.scancode);

        set_key_pressed_state(key, true);
        AppCore::Events::KeyPressedEvent event(key, modifiers, kev.repeat);
        handler(event);
      }
      break;
      case SDL_KEYUP: {
        const SDL_KeyboardEvent& kev = event->key;
        uint8_t modifiers = (uint8_t)(kev.keysym.mod);
        AppCore::Input::Key::Enum key = to_key(kev.keysym.scancode);

        set_key_pressed_state(key, false);
        AppCore::Events::KeyReleasedEvent event(key, modifiers);
        handler(event);
      }
      break;
      default: break;
    }
  }

  bool Window::create_window()
  {
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      APPCORE_TRACE("BaseWindow: Error initializing SDL");
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

    APPCORE_INFO(
        "BaseWindow: Creating window {0},{1} with OpenGL support", m_state.current_config.width, m_state.current_config.height);
    native_window = SDL_CreateWindow(m_state.current_config.title.c_str(),
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     m_state.current_config.width,
                                     m_state.current_config.height,
                                     flags);

    if(!native_window)
    {
      auto error = SDL_GetError();
      APPCORE_TRACE("BaseWindow: Error creating window, '{0}'.", error);
      return false;
    }

    m_context = SDL_GL_CreateContext(native_window);
    if(!m_context)
    {
      auto error = SDL_GetError();
      APPCORE_TRACE("BaseWindow: Error creating OpenGL context, '{0}'.", error);
      SDL_DestroyWindow(native_window);
      native_window = nullptr;
      return false;
    }

    SDL_GL_SetSwapInterval(m_state.current_config.VSync ? 1 : 0);

    m_shared_context = Context::create_context(ContextMode::SHARE, 330);

    if(!m_shared_context)
    {
      APPCORE_TRACE("Window: Error initializing GL shared context.");
      SDL_GL_DeleteContext(m_context);
      SDL_DestroyWindow(native_window);
      m_shared_context = nullptr;
      m_context = nullptr;
      native_window = nullptr;
      return false;
    }

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);

    if(!SDL_GetWindowWMInfo(native_window, &wmi))
    {
      auto error = SDL_GetError();
      APPCORE_TRACE("BaseWindow: Error retrieving window information: {0}.", error);
      m_shared_context->release();
      SDL_GL_DeleteContext(m_context);
      SDL_DestroyWindow(native_window);
      m_shared_context = nullptr;
      m_context = nullptr;
      native_window = nullptr;
      return false;
    }

    m_state.width = m_state.current_config.width;
    m_state.height = m_state.current_config.height;
    m_title = m_state.current_config.title;

    return true;
  }

  void Window::destroy_window()
  {
    m_shared_context->release();
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(native_window);
    m_shared_context = nullptr;
    m_context = nullptr;
    native_window = nullptr;
    SDL_Quit();
  }

  void Window::swap_buffers()
  {
    SDL_GL_SwapWindow(native_window);
  }

  bool Window::on_window_resize(AppCore::Events::WindowResizeEvent& event)
  {
    return BaseWindow::on_window_resize(event);
  }

  void Window::set_title(const AppCore::String& value)
  {
    m_title = value;
    SDL_SetWindowTitle(native_window, m_title.c_str());
  }

  const AppCore::String& Window::title() const
  {
    return m_title;
  }

  void Window::initialize_event_handler()
  {
    init();
    SDL_AddEventWatch(
        [](void* userdata, SDL_Event* currentEvent) -> int {
          AppCore::Application::WindowState& m_state = *(AppCore::Application::WindowState*)userdata;
          switch(currentEvent->type)
          {
            case SDL_WINDOWEVENT:
              switch(currentEvent->window.event)
              {
                case SDL_WINDOWEVENT_RESIZED: {
                  auto w = currentEvent->window.data1;
                  auto h = currentEvent->window.data2;
                  AppCore::Events::WindowResizeEvent event(w, h);
                  m_state.handler(event);
                }
                break;
                case SDL_WINDOWEVENT_CLOSE: {
                  AppCore::Events::WindowCloseEvent event;
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

              if(AppCore::Input::is_key_pressed(m_state.current_config.exit_key))
              {
                BaseWindow::current().quit();
              }

              if(AppCore::Input::is_key_pressed(m_state.current_config.fullscreen_key))
              {
                BaseWindow::current().toggle_full_screen();
              }
            }
            break;
            default: break;
          }

          return 0;
        },
        &m_state);
  }

  void Window::toggle_full_screen()
  {
    m_state.fullscreen = !m_state.fullscreen;
    if(m_state.fullscreen)
    {
      SDL_SetWindowFullscreen(native_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
      return;
    }

    SDL_RestoreWindow(native_window);
    SDL_SetWindowSize(native_window, m_state.current_config.width, m_state.current_config.height);
    SDL_SetWindowPosition(native_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowFullscreen(native_window, 0);
  }

} // namespace AppGL
