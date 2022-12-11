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
#include "config.h"
#include "pch.hpp"

#define APPCORE_PROFILE 0

#ifdef APPCORE_DEBUG
#	if defined(APPCORE_PLATFORM_WINDOWS)
#		define APPCORE_DEBUGBREAK() __debugbreak()
#	elif defined(APPCORE_PLATFORM_LINUX)
#		include <signal.h>
#		define APPCORE_DEBUGBREAK() raise(SIGTRAP)
#	else
#		error "Platform doesn't support debugbreak yet!"
#	endif
#	define APPCORE_ENABLE_ASSERTS
#else
#	define APPCORE_DEBUGBREAK()
#endif

#ifdef APPCORE_ENABLE_ASSERTS
#	define APPCORE_ASSERT(x, ...)                                                                                                                   \
		{                                                                                                                                            \
			if(!(x))                                                                                                                                 \
			{                                                                                                                                        \
				APPCORE_TRACE("Assertion Failed: {0}", __VA_ARGS__);                                                                                 \
				APPCORE_DEBUGBREAK();                                                                                                                \
			}                                                                                                                                        \
		}
#else
#	define APPCORE_ASSERT(x, y, ...)
#endif

#define BIT(x) 1 << x

#define APPCORE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace AppCore
{

	/*************************************************************
 * Generics
 *************************************************************/

	template <typename T>
	using Scope = std::unique_ptr<T>;

	template <typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T>
	using VectorRef = std::vector<Ref<T>>;

	template <typename T>
	using QueueRef = std::queue<Ref<T>>;

	template <typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	/* Core */

	class Application;
	class Log;

	/* Event */

	namespace Events
	{
		class Event;
		// class EventListener;
		class WindowCloseEvent;
		class WindowResizeEvent;
		class KeyPressedEvent;
		class KeyReleasedEvent;
		class KeyTypedEvent;
		class MouseMovedEvent;
		class MouseScrolledEvent;
		class MouseButtonPressedEvent;
		class MouseButtonReleasedEvent;
	} // namespace Events

	using EventHandler = std::function<void(Events::Event&)>;

} // namespace AppCore
