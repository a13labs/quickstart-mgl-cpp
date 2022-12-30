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
#include <nlohmann/json.hpp>

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

#define MGL_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MGL_MIN(a, b) (((a) < (b)) ? (a) : (b))

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
  using mem_buffer = std::vector<T>;

  template <typename T>
  using span = tcb::span<T>;

  template <typename T>
  using list = std::vector<T>;

  template <typename T, typename U>
  using pair = std::pair<T, U>;

  template <typename T, typename U>
  using dict = std::map<T, U>;

  using string = std::string;
  using string_list = list<string>;

  using timepoint = std::chrono::time_point<std::chrono::system_clock>;

  struct viewport_2d
  {
    int x;
    int y;
    int width;
    int height;

    viewport_2d(int x, int y, int w, int h)
        : x(x)
        , y(y)
        , width(w)
        , height(h)
    { }

    viewport_2d(int w, int h)
        : x(0)
        , y(0)
        , width(w)
        , height(h)
    { }

    viewport_2d()
        : x(0)
        , y(0)
        , width(0)
        , height(0)
    { }
  };

  struct viewport_3d
  {
    int x;
    int y;
    int z;
    int width;
    int height;
    int depth;

    viewport_3d(int x, int y, int z, int w, int h, int d)
        : x(x)
        , y(y)
        , z(z)
        , width(w)
        , height(h)
        , depth(d)
    { }

    viewport_3d(int w, int h, int d)
        : x(0)
        , y(0)
        , z(0)
        , width(w)
        , height(h)
        , depth(d)
    { }

    viewport_3d()
        : x(0)
        , y(0)
        , z(0)
        , width(0)
        , height(0)
        , depth(0)
    { }
  };

  struct size
  {
    int width;
    int height;

    size(int w, int h)
        : width(w)
        , height(h)
    { }

    size()
        : width(0)
        , height(0)
    { }
  };

  extern const viewport_2d null_viewport_2d;
  extern const viewport_3d null_viewport_3d;
  extern const size null_size;

  inline bool operator==(const viewport_2d& lhs, const viewport_2d& rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.width == rhs.width && lhs.height == rhs.height;
  }

  inline bool operator==(const viewport_3d& lhs, const viewport_3d& rhs)
  {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.width == rhs.width && lhs.height == rhs.height &&
           lhs.depth == rhs.depth;
  }

  inline bool operator==(const size& lhs, const size& rhs)
  {
    return lhs.width == rhs.width && lhs.height == rhs.height;
  }

  inline bool operator!=(const viewport_2d& lhs, const viewport_2d& rhs)
  {
    return !(lhs == rhs);
  }

  inline bool operator!=(const viewport_3d& lhs, const viewport_3d& rhs)
  {
    return !(lhs == rhs);
  }

  inline bool operator!=(const size& lhs, const size& rhs)
  {
    return !(lhs == rhs);
  }

  using path = std::filesystem::path;
  using path_list = mgl_core::list<path>;
  using input_file = std::ifstream;
  using output_file = std::ofstream;
  using openmode = std::ifstream::openmode;
  extern const path null_path;

  using json = nlohmann::json;
} // namespace mgl_core
