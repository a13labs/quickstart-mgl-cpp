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
#include "config.h"
#include "pch.hpp"

#define APPCORE_PROFILE 0

#ifdef APP_DEBUG
#  if defined(APP_PLATFORM_WINDOWS)
#    define APP_DEBUGBREAK() __debugbreak()
#  elif defined(APP_PLATFORM_LINUX)
#    include <signal.h>
#    define APP_DEBUGBREAK() raise(SIGTRAP)
#  else
#    error "Platform doesn't support debugbreak yet!"
#  endif
#  define APPCORE_ENABLE_ASSERTS
#else
#  define APP_DEBUGBREAK()
#endif

#ifdef APPCORE_ENABLE_ASSERTS
#  define APPCORE_ASSERT(x, ...)                                                                                                 \
    {                                                                                                                            \
      if(!(x))                                                                                                                   \
      {                                                                                                                          \
        APPCORE_TRACE("Assertion Failed: {0}", __VA_ARGS__);                                                                     \
        APP_DEBUGBREAK();                                                                                                        \
      }                                                                                                                          \
    }
#else
#  define APPCORE_ASSERT(x, y, ...)
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
  constexpr Scope<T> create_scope(Args&&... args)
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
  constexpr Ref<T> create_ref(Args&&... args)
  {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  using RefFromThis = std::enable_shared_from_this<T>;

  template <typename T>
  using MemoryBuffer = tcb::span<T>;

  template <typename T>
  using List = std::vector<T>;

  template <typename k, typename v>
  using Dict = std::map<k, v>;

  using String = std::string;
  using StringList = List<String>;

  using Timepoint = std::chrono::time_point<std::chrono::system_clock>;
} // namespace AppCore
