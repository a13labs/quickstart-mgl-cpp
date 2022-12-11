
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
#include "SDL2/SDL_syswm.h"
#include "glad/glad.h"

namespace AppCore
{
	Application* Application::sInstance = nullptr;
	uint8_t Application::sKeyForwardMap[256];
	uint8_t Application::sKeyReverseMap[256];

	void Application::InitMappings()
	{
		Application::ClearKeyMap();
		Application::AddKeyTranslation(SDL_SCANCODE_ESCAPE, Key::Esc);
		Application::AddKeyTranslation(SDL_SCANCODE_RETURN, Key::Return);
		Application::AddKeyTranslation(SDL_SCANCODE_TAB, Key::Tab);
		Application::AddKeyTranslation(SDL_SCANCODE_BACKSPACE, Key::Backspace);
		Application::AddKeyTranslation(SDL_SCANCODE_SPACE, Key::Space);
		Application::AddKeyTranslation(SDL_SCANCODE_UP, Key::Up);
		Application::AddKeyTranslation(SDL_SCANCODE_DOWN, Key::Down);
		Application::AddKeyTranslation(SDL_SCANCODE_LEFT, Key::Left);
		Application::AddKeyTranslation(SDL_SCANCODE_RIGHT, Key::Right);
		Application::AddKeyTranslation(SDL_SCANCODE_PAGEUP, Key::PageUp);
		Application::AddKeyTranslation(SDL_SCANCODE_PAGEDOWN, Key::PageDown);
		Application::AddKeyTranslation(SDL_SCANCODE_HOME, Key::Home);
		Application::AddKeyTranslation(SDL_SCANCODE_END, Key::End);
		Application::AddKeyTranslation(SDL_SCANCODE_PRINTSCREEN, Key::Print);
		Application::AddKeyTranslation(SDL_SCANCODE_KP_PLUS, Key::Plus);
		Application::AddKeyTranslation(SDL_SCANCODE_EQUALS, Key::Plus);
		Application::AddKeyTranslation(SDL_SCANCODE_KP_MINUS, Key::Minus);
		Application::AddKeyTranslation(SDL_SCANCODE_MINUS, Key::Minus);
		Application::AddKeyTranslation(SDL_SCANCODE_GRAVE, Key::Tilde);
		Application::AddKeyTranslation(SDL_SCANCODE_KP_COMMA, Key::Comma);
		Application::AddKeyTranslation(SDL_SCANCODE_COMMA, Key::Comma);
		Application::AddKeyTranslation(SDL_SCANCODE_KP_PERIOD, Key::Period);
		Application::AddKeyTranslation(SDL_SCANCODE_PERIOD, Key::Period);
		Application::AddKeyTranslation(SDL_SCANCODE_SLASH, Key::Slash);
		Application::AddKeyTranslation(SDL_SCANCODE_F1, Key::F1);
		Application::AddKeyTranslation(SDL_SCANCODE_F2, Key::F2);
		Application::AddKeyTranslation(SDL_SCANCODE_F3, Key::F3);
		Application::AddKeyTranslation(SDL_SCANCODE_F4, Key::F4);
		Application::AddKeyTranslation(SDL_SCANCODE_F5, Key::F5);
		Application::AddKeyTranslation(SDL_SCANCODE_F6, Key::F6);
		Application::AddKeyTranslation(SDL_SCANCODE_F7, Key::F7);
		Application::AddKeyTranslation(SDL_SCANCODE_F8, Key::F8);
		Application::AddKeyTranslation(SDL_SCANCODE_F9, Key::F9);
		Application::AddKeyTranslation(SDL_SCANCODE_F10, Key::F10);
		Application::AddKeyTranslation(SDL_SCANCODE_F11, Key::F11);
		Application::AddKeyTranslation(SDL_SCANCODE_F12, Key::F12);
		Application::AddKeyTranslation(SDL_SCANCODE_KP_0, Key::NumPad0);
		Application::AddKeyTranslation(SDL_SCANCODE_KP_1, Key::NumPad1);
		Application::AddKeyTranslation(SDL_SCANCODE_KP_2, Key::NumPad2);
		Application::AddKeyTranslation(SDL_SCANCODE_KP_3, Key::NumPad3);
		Application::AddKeyTranslation(SDL_SCANCODE_KP_4, Key::NumPad4);
		Application::AddKeyTranslation(SDL_SCANCODE_KP_5, Key::NumPad5);
		Application::AddKeyTranslation(SDL_SCANCODE_KP_6, Key::NumPad6);
		Application::AddKeyTranslation(SDL_SCANCODE_KP_7, Key::NumPad7);
		Application::AddKeyTranslation(SDL_SCANCODE_KP_8, Key::NumPad8);
		Application::AddKeyTranslation(SDL_SCANCODE_KP_9, Key::NumPad9);
		Application::AddKeyTranslation(SDL_SCANCODE_KP_ENTER, Key::NumPadEnter);
		Application::AddKeyTranslation(SDL_SCANCODE_0, Key::Key0);
		Application::AddKeyTranslation(SDL_SCANCODE_1, Key::Key1);
		Application::AddKeyTranslation(SDL_SCANCODE_2, Key::Key2);
		Application::AddKeyTranslation(SDL_SCANCODE_3, Key::Key3);
		Application::AddKeyTranslation(SDL_SCANCODE_4, Key::Key4);
		Application::AddKeyTranslation(SDL_SCANCODE_5, Key::Key5);
		Application::AddKeyTranslation(SDL_SCANCODE_6, Key::Key6);
		Application::AddKeyTranslation(SDL_SCANCODE_7, Key::Key7);
		Application::AddKeyTranslation(SDL_SCANCODE_8, Key::Key8);
		Application::AddKeyTranslation(SDL_SCANCODE_9, Key::Key9);
		Application::AddKeyTranslation(SDL_SCANCODE_A, Key::KeyA);
		Application::AddKeyTranslation(SDL_SCANCODE_B, Key::KeyB);
		Application::AddKeyTranslation(SDL_SCANCODE_C, Key::KeyC);
		Application::AddKeyTranslation(SDL_SCANCODE_D, Key::KeyD);
		Application::AddKeyTranslation(SDL_SCANCODE_E, Key::KeyE);
		Application::AddKeyTranslation(SDL_SCANCODE_F, Key::KeyF);
		Application::AddKeyTranslation(SDL_SCANCODE_G, Key::KeyG);
		Application::AddKeyTranslation(SDL_SCANCODE_H, Key::KeyH);
		Application::AddKeyTranslation(SDL_SCANCODE_I, Key::KeyI);
		Application::AddKeyTranslation(SDL_SCANCODE_J, Key::KeyJ);
		Application::AddKeyTranslation(SDL_SCANCODE_K, Key::KeyK);
		Application::AddKeyTranslation(SDL_SCANCODE_L, Key::KeyL);
		Application::AddKeyTranslation(SDL_SCANCODE_M, Key::KeyM);
		Application::AddKeyTranslation(SDL_SCANCODE_N, Key::KeyN);
		Application::AddKeyTranslation(SDL_SCANCODE_O, Key::KeyO);
		Application::AddKeyTranslation(SDL_SCANCODE_P, Key::KeyP);
		Application::AddKeyTranslation(SDL_SCANCODE_Q, Key::KeyQ);
		Application::AddKeyTranslation(SDL_SCANCODE_R, Key::KeyR);
		Application::AddKeyTranslation(SDL_SCANCODE_S, Key::KeyS);
		Application::AddKeyTranslation(SDL_SCANCODE_T, Key::KeyT);
		Application::AddKeyTranslation(SDL_SCANCODE_U, Key::KeyU);
		Application::AddKeyTranslation(SDL_SCANCODE_V, Key::KeyV);
		Application::AddKeyTranslation(SDL_SCANCODE_W, Key::KeyW);
		Application::AddKeyTranslation(SDL_SCANCODE_X, Key::KeyX);
		Application::AddKeyTranslation(SDL_SCANCODE_Y, Key::KeyY);
		Application::AddKeyTranslation(SDL_SCANCODE_Z, Key::KeyZ);
	}

	uint8_t Application::TranslateKeyModifiers(uint16_t key)
	{
		uint8_t modifiers = 0;
		modifiers |= key & KMOD_LALT ? AppCore::Modifier::LeftAlt : 0;
		modifiers |= key & KMOD_RALT ? AppCore::Modifier::RightAlt : 0;
		modifiers |= key & KMOD_LCTRL ? AppCore::Modifier::LeftCtrl : 0;
		modifiers |= key & KMOD_RCTRL ? AppCore::Modifier::RightCtrl : 0;
		modifiers |= key & KMOD_LSHIFT ? AppCore::Modifier::LeftShift : 0;
		modifiers |= key & KMOD_RSHIFT ? AppCore::Modifier::RightShift : 0;
		modifiers |= key & KMOD_LGUI ? AppCore::Modifier::LeftMeta : 0;
		modifiers |= key & KMOD_RGUI ? AppCore::Modifier::RightMeta : 0;
		return modifiers;
	}

	uint8_t Application::TranslateKeyModifierPress(uint16_t key)
	{
		uint8_t modifier;
		switch(key)
		{
			case SDL_SCANCODE_LALT: {
				modifier = AppCore::Modifier::LeftAlt;
			}
			break;
			case SDL_SCANCODE_RALT: {
				modifier = AppCore::Modifier::RightAlt;
			}
			break;
			case SDL_SCANCODE_LCTRL: {
				modifier = AppCore::Modifier::LeftCtrl;
			}
			break;
			case SDL_SCANCODE_RCTRL: {
				modifier = AppCore::Modifier::RightCtrl;
			}
			break;
			case SDL_SCANCODE_LSHIFT: {
				modifier = AppCore::Modifier::LeftShift;
			}
			break;
			case SDL_SCANCODE_RSHIFT: {
				modifier = AppCore::Modifier::RightShift;
			}
			break;
			case SDL_SCANCODE_LGUI: {
				modifier = AppCore::Modifier::LeftMeta;
			}
			break;
			case SDL_SCANCODE_RGUI: {
				modifier = AppCore::Modifier::RightMeta;
			}
			break;
			default: {
				modifier = 0;
			}
			break;
		}

		return modifier;
	}

	Application::Application(std::string title,
							 uint32_t width,
							 uint32_t height,
							 uint32_t glMajor,
							 uint32_t glMinor,
							 bool fullscreen,
							 bool resizable,
							 bool vsync,
							 uint32_t samples,
							 bool cursor)
	{
		APPCORE_ASSERT(!sInstance, "Application already running!");
		Log::Init();

		mWindow = nullptr;
		mGLContext = nullptr;
		if(SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			APPCORE_TRACE("Application: Error initializing SDL");
			return;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, glMajor);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, glMinor);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		// SDL_GL_SetAttribute(SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		if(samples > 1)
		{
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, samples);
		}

		auto flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;

		if(fullscreen)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		else
		{
			if(resizable)
			{
				flags |= SDL_WINDOW_RESIZABLE;
			}
		}

		APPCORE_INFO("Application: Creating window {0},{1} with OpenGL support", width, height);
		mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

		if(!mWindow)
		{
			auto error = SDL_GetError();
			APPCORE_TRACE("Application: Error creating window, '{0}'.", error);
			SDL_Quit();
		}

		mGLContext = SDL_GL_CreateContext(mWindow);
		if(!mGLContext)
		{
			auto error = SDL_GetError();
			APPCORE_TRACE("Application: Error creating OpenGL context, '{0}'.", error);
			SDL_DestroyWindow(mWindow);
			mWindow = nullptr;
			SDL_Quit();
		}

		SDL_GL_SetSwapInterval(vsync ? 1 : 0);

		if(!gladLoadGL())
		{
			APPCORE_TRACE("Application: Error initializing GL extensions.");
			SDL_GL_DeleteContext(mGLContext);
			SDL_DestroyWindow(mWindow);
			mGLContext = nullptr;
			mWindow = nullptr;
			SDL_Quit();
		}

		SDL_SysWMinfo wmi;
		SDL_VERSION(&wmi.version);

		if(!SDL_GetWindowWMInfo(mWindow, &wmi))
		{
			auto error = SDL_GetError();
			APPCORE_TRACE("Application: Error retrieving window information: {0}.", error);
			SDL_GL_DeleteContext(mGLContext);
			mGLContext = nullptr;

			SDL_DestroyWindow(mWindow);
			mWindow = nullptr;
			SDL_Quit();
		}

		Application::InitMappings();
		mHandler = APPCORE_BIND_EVENT_FN(Application::OnEvent);

		SDL_AddEventWatch(
			[](void* userdata, SDL_Event* currentEvent) -> int {
				EventHandler& handler = *(EventHandler*)userdata;
				switch(currentEvent->type)
				{
					case SDL_WINDOWEVENT:
						switch(currentEvent->window.event)
						{
							case SDL_WINDOWEVENT_RESIZED: {
								auto w = currentEvent->window.data1;
								auto h = currentEvent->window.data2;
								WindowResizeEvent event(w, h);
								handler(event);
							}
							break;
							case SDL_WINDOWEVENT_CLOSE: {
								WindowCloseEvent event;
								handler(event);
							}
							break;
							default: break;
						}
					case SDL_MOUSEMOTION: {
						const SDL_MouseMotionEvent& mev = currentEvent->motion;
						MouseMovedEvent event(mev.x, mev.y);
						handler(event);
					}
					break;
					case SDL_MOUSEBUTTONDOWN:
					case SDL_MOUSEBUTTONUP: {
						const SDL_MouseButtonEvent& mev = currentEvent->button;
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
								MouseButtonPressedEvent event(button);
								handler(event);
							}
							break;
							case SDL_RELEASED: {
								MouseButtonReleasedEvent event(button);
								handler(event);
							}
							break;
						}
					}
					break;
					case SDL_MOUSEWHEEL: {
						const SDL_MouseWheelEvent& mev = currentEvent->wheel;
						MouseScrolledEvent event(mev.x, mev.y);
						handler(event);
					}
					break;
					case SDL_KEYDOWN: {
						const SDL_KeyboardEvent& kev = currentEvent->key;
						uint8_t modifiers = Application::TranslateKeyModifiers(kev.keysym.mod);
						Key::Enum key = Application::ToKey(kev.keysym.scancode);
						if(0 == key && 0 == modifiers)
						{
							modifiers = Application::TranslateKeyModifierPress(kev.keysym.scancode);
						}
						KeyPressedEvent event(key, modifiers, kev.repeat);
						handler(event);
					}
					break;
					case SDL_KEYUP: {
						const SDL_KeyboardEvent& kev = currentEvent->key;
						uint8_t modifiers = Application::TranslateKeyModifiers(kev.keysym.mod);
						Key::Enum key = Application::ToKey(kev.keysym.scancode);
						KeyReleasedEvent event(key, modifiers);
						handler(event);
					}
					break;
					default: break;
				}

				return 0;
			},
			&mHandler);

		sInstance = this;
		mRunning = false;
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		// Dispatch Windows Events
		dispatcher.Dispatch<WindowCloseEvent>(APPCORE_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(APPCORE_BIND_EVENT_FN(Application::OnWindowResize));

		// Dispatch key events to be handled by the application
		dispatcher.Dispatch<KeyPressedEvent>(APPCORE_BIND_EVENT_FN(Application::OnKeyPressed));
		dispatcher.Dispatch<KeyReleasedEvent>(APPCORE_BIND_EVENT_FN(Application::OnKeyReleased));

		// Dispatch mouse events to be handled by the application
		dispatcher.Dispatch<MouseMovedEvent>(APPCORE_BIND_EVENT_FN(Application::OnMouseMoved));
		dispatcher.Dispatch<MouseScrolledEvent>(APPCORE_BIND_EVENT_FN(Application::OnMouseScrolled));
		dispatcher.Dispatch<MouseButtonPressedEvent>(APPCORE_BIND_EVENT_FN(Application::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(APPCORE_BIND_EVENT_FN(Application::OnMouseButtonReleased));
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		mRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		return true;
	}

	void Application::Run()
	{
		if(mRunning)
			return;

		mRunning = true;

		APPCORE_PROFILE_BEGIN_SESSION();

		while(mRunning)
		{
			SDL_Event e;
			SDL_PollEvent(&e);
			SDL_GL_SwapWindow(mWindow);
#if APPCORE_PROFILE
			// Since we are profiling we just render one frame
			mRunning = false;
#endif
		}

		APPCORE_PROFILE_END_SESSION();
		SDL_GL_DeleteContext(mGLContext);
		SDL_DestroyWindow(mWindow);
		SDL_Quit();
	}

} // namespace AppCore
