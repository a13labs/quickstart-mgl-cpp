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
#include "appcore.hpp"
#include "log.hpp"

#include "SDL2/SDL.h"

namespace AppCore
{
  namespace Input
  {

    struct Modifier
    {
      enum Enum
      {
        None = 0,
        LeftAlt = 0x0001,
        RightAlt = 0x0002,
        LeftCtrl = 0x0040,
        RightCtrl = 0x0080,
        LeftShift = 0x0100,
        RightShift = 0x0200,
        LeftMeta = 0x0400,
        RightMeta = 0x0800,
      };
    };

    struct Key
    {
      enum Enum
      {
        None = 0,
        Esc,
        Return,
        Tab,
        Space,
        Backspace,
        Up,
        Down,
        Left,
        Right,
        Insert,
        Delete,
        Home,
        End,
        PageUp,
        PageDown,
        Print,
        Plus,
        Minus,
        LeftBracket,
        RightBracket,
        Semicolon,
        Quote,
        Comma,
        Period,
        Slash,
        Backslash,
        Tilde,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        NumPad0,
        NumPad1,
        NumPad2,
        NumPad3,
        NumPad4,
        NumPad5,
        NumPad6,
        NumPad7,
        NumPad8,
        NumPad9,
        NumPadEnter,
        Key0,
        Key1,
        Key2,
        Key3,
        Key4,
        Key5,
        Key6,
        Key7,
        Key8,
        Key9,
        KeyA,
        KeyB,
        KeyC,
        KeyD,
        KeyE,
        KeyF,
        KeyG,
        KeyH,
        KeyI,
        KeyJ,
        KeyK,
        KeyL,
        KeyM,
        KeyN,
        KeyO,
        KeyP,
        KeyQ,
        KeyR,
        KeyS,
        KeyT,
        KeyU,
        KeyV,
        KeyW,
        KeyX,
        KeyY,
        KeyZ,

        LeftAlt,
        RightAlt,
        LeftCtrl,
        RightCtrl,
        LeftShift,
        RightShift,
        LeftMeta,
        RightMeta,

        Count
      };
    };

    struct MouseButton
    {
      enum Enum
      {
        Left,
        Middle,
        Right,
        None,

        Count
      };
    };

    struct GamepadAxis
    {
      enum Enum
      {
        LeftX,
        LeftY,
        LeftZ,
        RightX,
        RightY,
        RightZ,

        Count
      };
    };

    typedef struct
    {
      bool pressed_keys[Key::Count];
      bool pressed_mouse_buttons[MouseButton::Count];
    } InputState;

    extern InputState input_state;

    inline bool is_key_pressed(Key::Enum key)
    {
      return input_state.pressed_keys[key & 0xff];
    }

    inline bool IsMouseButtonPressed(MouseButton::Enum button)
    {
      return input_state.pressed_mouse_buttons[button & 0xff];
    }

  } // namespace Input
} // namespace AppCore
