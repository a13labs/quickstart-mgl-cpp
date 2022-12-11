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
#pragma once
#include "SDL2/SDL.h"
#include "base.hpp"
#include "event.hpp"
#include "input.hpp"
#include "log.hpp"

namespace AppCore
{
	class Application
	{

	public:
		Application(std::string title = "Application",
					uint32_t width = 800,
					uint32_t height = 600,
					uint32_t glMajor = 3,
					uint32_t glMinor = 3,
					bool fullscreen = false,
					bool resizable = true,
					bool vsync = true,
					uint32_t samples = 0,
					bool cursor = true);
		virtual ~Application() = default;

	public:
		virtual void Run();

	public:
		static Application& Current() { return *sInstance; }

		// Events
		void OnEvent(Event& event);

		// Windows Events
		virtual bool OnWindowClose(WindowCloseEvent& event);
		virtual bool OnWindowResize(WindowResizeEvent& event);

		// Keys Events
		virtual bool OnKeyPressed(KeyPressedEvent& event) { return true; }
		virtual bool OnKeyReleased(KeyReleasedEvent& event) { return true; }

		// Mouse Events
		virtual bool OnMouseMoved(MouseMovedEvent& event) { return true; }
		virtual bool OnMouseScrolled(MouseScrolledEvent& event) { return true; }
		virtual bool OnMouseButtonPressed(MouseButtonPressedEvent& event) { return true; }
		virtual bool OnMouseButtonReleased(MouseButtonReleasedEvent& event) { return true; }

	private:
		inline static void AddKeyTranslation(uint16_t src, AppCore::Key::Enum tgt)
		{
			sKeyForwardMap[src & 0xff] = (uint8_t)tgt;
			sKeyReverseMap[tgt & 0xff] = (uint8_t)src;
		}

		inline static int FromKey(AppCore::Key::Enum key) { return (int)sKeyReverseMap[key & 0xff]; }

		inline static AppCore::Key::Enum ToKey(int key) { return (AppCore::Key::Enum)sKeyForwardMap[key & 0xff]; }

		inline static void ClearKeyMap()
		{
			std::memset(sKeyForwardMap, 0, sizeof(sKeyForwardMap));
			std::memset(sKeyReverseMap, 0, sizeof(sKeyReverseMap));
		}

		static void InitMappings();
		static uint8_t TranslateKeyModifierPress(uint16_t key);
		static uint8_t TranslateKeyModifiers(uint16_t key);

	private:
		static Application* sInstance;
		static uint8_t sKeyForwardMap[256];
		static uint8_t sKeyReverseMap[256];

		bool mRunning;
		SDL_Window* mWindow;
		SDL_GLContext mGLContext;
		EventHandler mHandler;
	};
} // namespace AppCore
