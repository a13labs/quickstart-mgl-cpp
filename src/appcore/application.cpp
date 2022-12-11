
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
#include "input.hpp"
#include "log.hpp"

#include "SDL2/SDL_syswm.h"
#include "glad/glad.h"

namespace AppCore
{
	Application* Application::sInstance = nullptr;

	Application::Application(const ApplicationConfig& config)
	{
		APPCORE_ASSERT(!sInstance, "Application already running!");
		Log::Init();

		mData.Config = config;
		mWindow = nullptr;
		mGLContext = nullptr;
		if(SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			APPCORE_TRACE("Application: Error initializing SDL");
			return;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, mData.Config.GLMajor);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, mData.Config.GLMinor);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		// SDL_GL_SetAttribute(SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG, 1);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		if(mData.Config.Samples > 1)
		{
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, mData.Config.Samples);
		}

		auto flags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;

		mData.Fullscreen = mData.Config.Fullscreen;
		if(mData.Config.Fullscreen)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		else
		{
			if(mData.Config.Resizable)
			{
				flags |= SDL_WINDOW_RESIZABLE;
			}
		}

		APPCORE_INFO("Application: Creating window {0},{1} with OpenGL support", mData.Config.Width, mData.Config.Height);
		mWindow = SDL_CreateWindow(
			mData.Config.Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mData.Config.Width, mData.Config.Height, flags);

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

		SDL_GL_SetSwapInterval(mData.Config.VSync ? 1 : 0);

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

		Input::Init();
		mData.Handler = APPCORE_BIND_EVENT_FN(Application::OnEvent);

		SDL_AddEventWatch(
			[](void* userdata, SDL_Event* currentEvent) -> int {
				ApplicationStateData& mData = *(ApplicationStateData*)userdata;
				switch(currentEvent->type)
				{
					case SDL_WINDOWEVENT:
						switch(currentEvent->window.event)
						{
							case SDL_WINDOWEVENT_RESIZED: {
								auto w = currentEvent->window.data1;
								auto h = currentEvent->window.data2;
								Events::WindowResizeEvent event(w, h);
								mData.Handler(event);
							}
							break;
							case SDL_WINDOWEVENT_CLOSE: {
								Events::WindowCloseEvent event;
								mData.Handler(event);
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
						Input::UpdateState(currentEvent, mData.Handler);

						if(Input::IsKeyPressed(mData.Config.ExitKey))
						{
							Application::Current().Quit();
						}

						if(Input::IsKeyPressed(mData.Config.FullScreenKey))
						{
							Application::Current().ToggleFullScreen();
						}
					}
					break;
					default: break;
				}

				return 0;
			},
			&mData);

		sInstance = this;
		mRunning = false;
	}

	void Application::OnEvent(Events::Event& event)
	{
		Events::EventDispatcher dispatcher(event);

		// Dispatch Windows Events
		dispatcher.Dispatch<Events::WindowCloseEvent>(APPCORE_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<Events::WindowResizeEvent>(APPCORE_BIND_EVENT_FN(Application::OnWindowResize));

		// Dispatch key events to be handled by the application
		dispatcher.Dispatch<Events::KeyPressedEvent>(APPCORE_BIND_EVENT_FN(Application::OnKeyPressed));
		dispatcher.Dispatch<Events::KeyReleasedEvent>(APPCORE_BIND_EVENT_FN(Application::OnKeyReleased));

		// Dispatch mouse events to be handled by the application
		dispatcher.Dispatch<Events::MouseMovedEvent>(APPCORE_BIND_EVENT_FN(Application::OnMouseMoved));
		dispatcher.Dispatch<Events::MouseScrolledEvent>(APPCORE_BIND_EVENT_FN(Application::OnMouseScrolled));
		dispatcher.Dispatch<Events::MouseButtonPressedEvent>(APPCORE_BIND_EVENT_FN(Application::OnMouseButtonPressed));
		dispatcher.Dispatch<Events::MouseButtonReleasedEvent>(APPCORE_BIND_EVENT_FN(Application::OnMouseButtonReleased));
	}

	bool Application::OnWindowClose(Events::WindowCloseEvent& event)
	{
		mRunning = false;
		return true;
	}

	bool Application::OnWindowResize(Events::WindowResizeEvent& event)
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

	void Application::Quit()
	{
		mRunning = false;
	}

	void Application::ToggleFullScreen()
	{
		mData.Fullscreen = !mData.Fullscreen;
		if(mData.Fullscreen)
		{
			SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
			return;
		}

		SDL_RestoreWindow(mWindow);
		SDL_SetWindowSize(mWindow, mData.Config.Width, mData.Config.Height);
		SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		SDL_SetWindowFullscreen(mWindow, 0);
	}

} // namespace AppCore
