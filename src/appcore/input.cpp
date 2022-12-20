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
#include "input.hpp"
#include "event.hpp"

namespace AppCore
{
  namespace Input
  {
    InputState input_state;

    inline void add_key_translation(uint16_t src, Key::Enum tgt)
    {
      input_state.key_forward_map[src & 0xff] = (uint8_t)tgt;
      input_state.key_reverse_map[tgt & 0xff] = (uint8_t)src;
    }

    inline int from_key(Key::Enum key)
    {
      return (int)input_state.key_reverse_map[key & 0xff];
    }

    inline Key::Enum to_key(int key)
    {
      return (Key::Enum)input_state.key_forward_map[key & 0xff];
    }

    inline void clear_key_map()
    {
      std::memset(input_state.key_forward_map, 0, sizeof(input_state.key_forward_map));
      std::memset(input_state.key_reverse_map, 0, sizeof(input_state.key_reverse_map));
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

    void update_state(SDL_Event* event, const EventHandler& handler)
    {
      switch(event->type)
      {
        case SDL_MOUSEMOTION: {
          const SDL_MouseMotionEvent& mev = event->motion;
          Events::MouseMovedEvent event(mev.x, mev.y);
          handler(event);
        }
        break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
          const SDL_MouseButtonEvent& mev = event->button;
          Input::MouseButton::Enum button;
          switch(mev.button)
          {
            default:
            case SDL_BUTTON_LEFT: button = Input::MouseButton::Left; break;
            case SDL_BUTTON_MIDDLE: button = Input::MouseButton::Middle; break;
            case SDL_BUTTON_RIGHT: button = Input::MouseButton::Right; break;
          }

          switch(mev.state)
          {
            default:
            case SDL_PRESSED: {
              set_mouse_button_pressed_state(button, true);
              Events::MouseButtonPressedEvent event(button);
              handler(event);
            }
            break;
            case SDL_RELEASED: {
              set_mouse_button_pressed_state(button, false);
              Events::MouseButtonReleasedEvent event(button);
              handler(event);
            }
            break;
          }
        }
        break;
        case SDL_MOUSEWHEEL: {
          const SDL_MouseWheelEvent& mev = event->wheel;
          Events::MouseScrolledEvent event(mev.x, mev.y);
          handler(event);
        }
        break;
        case SDL_KEYDOWN: {
          const SDL_KeyboardEvent& kev = event->key;
          uint8_t modifiers = (uint8_t)(kev.keysym.mod);
          Input::Key::Enum key = Input::to_key(kev.keysym.scancode);

          set_key_pressed_state(key, true);
          Events::KeyPressedEvent event(key, modifiers, kev.repeat);
          handler(event);
        }
        break;
        case SDL_KEYUP: {
          const SDL_KeyboardEvent& kev = event->key;
          uint8_t modifiers = (uint8_t)(kev.keysym.mod);
          Input::Key::Enum key = Input::to_key(kev.keysym.scancode);

          set_key_pressed_state(key, false);
          Events::KeyReleasedEvent event(key, modifiers);
          handler(event);
        }
        break;
        default: break;
      }
    }

  } // namespace Input
} // namespace AppCore