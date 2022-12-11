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
#include "input.hpp"
#include "event.hpp"

namespace AppCore
{
	namespace Input
	{

		static InputState sInputState;

		inline void AddKeyTranslation(uint16_t src, Key::Enum tgt)
		{
			sInputState.KeyForwardMap[src & 0xff] = (uint8_t)tgt;
			sInputState.KeyReverseMap[tgt & 0xff] = (uint8_t)src;
		}

		inline int FromKey(Key::Enum key)
		{
			return (int)sInputState.KeyReverseMap[key & 0xff];
		}

		inline Key::Enum ToKey(int key)
		{
			return (Key::Enum)sInputState.KeyForwardMap[key & 0xff];
		}

		inline void ClearKeyMap()
		{
			std::memset(sInputState.KeyForwardMap, 0, sizeof(sInputState.KeyForwardMap));
			std::memset(sInputState.KeyReverseMap, 0, sizeof(sInputState.KeyReverseMap));
			std::memset(sInputState.PressedKeys, 0, sizeof(sInputState.PressedKeys));
			std::memset(sInputState.PressedMouseButtons, 0, sizeof(sInputState.PressedMouseButtons));
		}

		inline void SetKeyPressedState(Key::Enum key, bool value)
		{
			sInputState.PressedKeys[key & 0xff] = value;
			APPCORE_INFO("SetKeyPressedState: {0}:{1}", key, value);
		}

		inline void SetMouseButtonPressedState(MouseButton::Enum button, bool value)
		{
			sInputState.PressedMouseButtons[button & 0xff] = value;
		}

		void Init()
		{
			ClearKeyMap();
			AddKeyTranslation(SDL_SCANCODE_ESCAPE, Key::Esc);
			AddKeyTranslation(SDL_SCANCODE_RETURN, Key::Return);
			AddKeyTranslation(SDL_SCANCODE_TAB, Key::Tab);
			AddKeyTranslation(SDL_SCANCODE_BACKSPACE, Key::Backspace);
			AddKeyTranslation(SDL_SCANCODE_SPACE, Key::Space);
			AddKeyTranslation(SDL_SCANCODE_UP, Key::Up);
			AddKeyTranslation(SDL_SCANCODE_DOWN, Key::Down);
			AddKeyTranslation(SDL_SCANCODE_LEFT, Key::Left);
			AddKeyTranslation(SDL_SCANCODE_RIGHT, Key::Right);
			AddKeyTranslation(SDL_SCANCODE_PAGEUP, Key::PageUp);
			AddKeyTranslation(SDL_SCANCODE_PAGEDOWN, Key::PageDown);
			AddKeyTranslation(SDL_SCANCODE_HOME, Key::Home);
			AddKeyTranslation(SDL_SCANCODE_END, Key::End);
			AddKeyTranslation(SDL_SCANCODE_PRINTSCREEN, Key::Print);
			AddKeyTranslation(SDL_SCANCODE_KP_PLUS, Key::Plus);
			AddKeyTranslation(SDL_SCANCODE_EQUALS, Key::Plus);
			AddKeyTranslation(SDL_SCANCODE_KP_MINUS, Key::Minus);
			AddKeyTranslation(SDL_SCANCODE_MINUS, Key::Minus);
			AddKeyTranslation(SDL_SCANCODE_GRAVE, Key::Tilde);
			AddKeyTranslation(SDL_SCANCODE_KP_COMMA, Key::Comma);
			AddKeyTranslation(SDL_SCANCODE_COMMA, Key::Comma);
			AddKeyTranslation(SDL_SCANCODE_KP_PERIOD, Key::Period);
			AddKeyTranslation(SDL_SCANCODE_PERIOD, Key::Period);
			AddKeyTranslation(SDL_SCANCODE_SLASH, Key::Slash);
			AddKeyTranslation(SDL_SCANCODE_F1, Key::F1);
			AddKeyTranslation(SDL_SCANCODE_F2, Key::F2);
			AddKeyTranslation(SDL_SCANCODE_F3, Key::F3);
			AddKeyTranslation(SDL_SCANCODE_F4, Key::F4);
			AddKeyTranslation(SDL_SCANCODE_F5, Key::F5);
			AddKeyTranslation(SDL_SCANCODE_F6, Key::F6);
			AddKeyTranslation(SDL_SCANCODE_F7, Key::F7);
			AddKeyTranslation(SDL_SCANCODE_F8, Key::F8);
			AddKeyTranslation(SDL_SCANCODE_F9, Key::F9);
			AddKeyTranslation(SDL_SCANCODE_F10, Key::F10);
			AddKeyTranslation(SDL_SCANCODE_F11, Key::F11);
			AddKeyTranslation(SDL_SCANCODE_F12, Key::F12);
			AddKeyTranslation(SDL_SCANCODE_KP_0, Key::NumPad0);
			AddKeyTranslation(SDL_SCANCODE_KP_1, Key::NumPad1);
			AddKeyTranslation(SDL_SCANCODE_KP_2, Key::NumPad2);
			AddKeyTranslation(SDL_SCANCODE_KP_3, Key::NumPad3);
			AddKeyTranslation(SDL_SCANCODE_KP_4, Key::NumPad4);
			AddKeyTranslation(SDL_SCANCODE_KP_5, Key::NumPad5);
			AddKeyTranslation(SDL_SCANCODE_KP_6, Key::NumPad6);
			AddKeyTranslation(SDL_SCANCODE_KP_7, Key::NumPad7);
			AddKeyTranslation(SDL_SCANCODE_KP_8, Key::NumPad8);
			AddKeyTranslation(SDL_SCANCODE_KP_9, Key::NumPad9);
			AddKeyTranslation(SDL_SCANCODE_KP_ENTER, Key::NumPadEnter);
			AddKeyTranslation(SDL_SCANCODE_0, Key::Key0);
			AddKeyTranslation(SDL_SCANCODE_1, Key::Key1);
			AddKeyTranslation(SDL_SCANCODE_2, Key::Key2);
			AddKeyTranslation(SDL_SCANCODE_3, Key::Key3);
			AddKeyTranslation(SDL_SCANCODE_4, Key::Key4);
			AddKeyTranslation(SDL_SCANCODE_5, Key::Key5);
			AddKeyTranslation(SDL_SCANCODE_6, Key::Key6);
			AddKeyTranslation(SDL_SCANCODE_7, Key::Key7);
			AddKeyTranslation(SDL_SCANCODE_8, Key::Key8);
			AddKeyTranslation(SDL_SCANCODE_9, Key::Key9);
			AddKeyTranslation(SDL_SCANCODE_A, Key::KeyA);
			AddKeyTranslation(SDL_SCANCODE_B, Key::KeyB);
			AddKeyTranslation(SDL_SCANCODE_C, Key::KeyC);
			AddKeyTranslation(SDL_SCANCODE_D, Key::KeyD);
			AddKeyTranslation(SDL_SCANCODE_E, Key::KeyE);
			AddKeyTranslation(SDL_SCANCODE_F, Key::KeyF);
			AddKeyTranslation(SDL_SCANCODE_G, Key::KeyG);
			AddKeyTranslation(SDL_SCANCODE_H, Key::KeyH);
			AddKeyTranslation(SDL_SCANCODE_I, Key::KeyI);
			AddKeyTranslation(SDL_SCANCODE_J, Key::KeyJ);
			AddKeyTranslation(SDL_SCANCODE_K, Key::KeyK);
			AddKeyTranslation(SDL_SCANCODE_L, Key::KeyL);
			AddKeyTranslation(SDL_SCANCODE_M, Key::KeyM);
			AddKeyTranslation(SDL_SCANCODE_N, Key::KeyN);
			AddKeyTranslation(SDL_SCANCODE_O, Key::KeyO);
			AddKeyTranslation(SDL_SCANCODE_P, Key::KeyP);
			AddKeyTranslation(SDL_SCANCODE_Q, Key::KeyQ);
			AddKeyTranslation(SDL_SCANCODE_R, Key::KeyR);
			AddKeyTranslation(SDL_SCANCODE_S, Key::KeyS);
			AddKeyTranslation(SDL_SCANCODE_T, Key::KeyT);
			AddKeyTranslation(SDL_SCANCODE_U, Key::KeyU);
			AddKeyTranslation(SDL_SCANCODE_V, Key::KeyV);
			AddKeyTranslation(SDL_SCANCODE_W, Key::KeyW);
			AddKeyTranslation(SDL_SCANCODE_X, Key::KeyX);
			AddKeyTranslation(SDL_SCANCODE_Y, Key::KeyY);
			AddKeyTranslation(SDL_SCANCODE_Z, Key::KeyZ);
			AddKeyTranslation(SDL_SCANCODE_LALT, Key::LeftAlt);
			AddKeyTranslation(SDL_SCANCODE_RALT, Key::RightAlt);
			AddKeyTranslation(SDL_SCANCODE_LCTRL, Key::LeftCtrl);
			AddKeyTranslation(SDL_SCANCODE_RCTRL, Key::RightCtrl);
			AddKeyTranslation(SDL_SCANCODE_LSHIFT, Key::LeftShift);
			AddKeyTranslation(SDL_SCANCODE_RSHIFT, Key::RightShift);
			AddKeyTranslation(SDL_SCANCODE_LGUI, Key::LeftMeta);
			AddKeyTranslation(SDL_SCANCODE_RGUI, Key::RightMeta);
		}

		void UpdateState(SDL_Event* event, const EventHandler& handler)
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
							SetMouseButtonPressedState(button, true);
							Events::MouseButtonPressedEvent event(button);
							handler(event);
						}
						break;
						case SDL_RELEASED: {
							SetMouseButtonPressedState(button, false);
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
					Input::Key::Enum key = Input::ToKey(kev.keysym.scancode);

					SetKeyPressedState(key, true);
					Events::KeyPressedEvent event(key, modifiers, kev.repeat);
					handler(event);
				}
				break;
				case SDL_KEYUP: {
					const SDL_KeyboardEvent& kev = event->key;
					uint8_t modifiers = (uint8_t)(kev.keysym.mod);
					Input::Key::Enum key = Input::ToKey(kev.keysym.scancode);

					SetKeyPressedState(key, false);
					Events::KeyReleasedEvent event(key, modifiers);
					handler(event);
				}
				break;
				default: break;
			}
		}

		bool IsKeyPressed(Key::Enum key)
		{
			return sInputState.PressedKeys[key & 0xff];
		}

		bool IsMouseButtonPressed(MouseButton::Enum button)
		{
			return sInputState.PressedMouseButtons[button & 0xff];
		}

	} // namespace Input
} // namespace AppCore