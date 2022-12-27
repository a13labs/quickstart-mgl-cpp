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

#define MGL_CORE_PROFILE 0

#ifdef MGL_DEBUG
#  if defined(MGL_PLATFORM_WINDOWS)
#    define MGL_DEBUGBREAK() __debugbreak()
#  elif defined(MGL_PLATFORM_LINUX)
#    include <signal.h>
#    define MGL_DEBUGBREAK() raise(SIGTRAP)
#  else
#    error "Platform doesn't support debugbreak yet!"
#  endif
#  define MGL_CORE_ENABLE_ASSERTS
#else
#  define MGL_DEBUGBREAK()
#endif

#ifdef MGL_CORE_ENABLE_ASSERTS
#  define MGL_CORE_ASSERT(x, ...)                                                                                                \
    {                                                                                                                            \
      if(!(x))                                                                                                                   \
      {                                                                                                                          \
        MGL_CORE_TRACE("Assertion Failed: {0}", __VA_ARGS__);                                                                    \
        MGL_DEBUGBREAK();                                                                                                        \
      }                                                                                                                          \
    }
#else
#  define MGL_CORE_ASSERT(x, y, ...)
#endif

#define BIT(x) 1 << x

#define MGL_CORE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace mgl_core
{

  /*************************************************************
 * Generics
 *************************************************************/

  template <typename T>
  using scope = std::unique_ptr<T>;

  template <typename T, typename... Args>
  constexpr scope<T> create_scope(Args&&... args)
  {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  using ref = std::shared_ptr<T>;

  template <typename T>
  using ref_list = std::vector<ref<T>>;

  template <typename T>
  using ref_queue = std::queue<ref<T>>;

  template <typename T, typename... Args>
  constexpr ref<T> create_ref(Args&&... args)
  {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

  template <typename T>
  using ref_from_this = std::enable_shared_from_this<T>;

  template <typename T>
  using mem_buffer = tcb::span<T>;

  template <typename T>
  using list = std::vector<T>;

  template <typename k, typename v>
  using dict = std::map<k, v>;

  using string = std::string;
  using string_list = list<string>;

  using timepoint = std::chrono::time_point<std::chrono::system_clock>;
} // namespace mgl_core