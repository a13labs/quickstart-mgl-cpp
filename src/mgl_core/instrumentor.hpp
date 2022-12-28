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
#include "builtins.hpp"
#include "log.hpp"
#include <nlohmann/json.hpp>
#include <sys/types.h>

#ifdef MGL_PLATFORM_WINDOWS
#  include <process.h>
#  define GET_PROCESS_ID _getpid
#elif MGL_PLATFORM_LINUX
#  include <unistd.h>
#  define GET_PROCESS_ID getpid
#endif

namespace mgl_core
{
  using floating_point_microseconds = std::chrono::duration<double, std::micro>;

  struct profile_result
  {
    string name;

    floating_point_microseconds start;
    std::chrono::microseconds elapsed_time;
    std::thread::id thread_id;
    int process_id;
    string category;
  };

  struct instrumentation_session
  {
    string name;
  };

  class Instrumentor
  {
private:
    std::mutex m_mutex;
    instrumentation_session* m_current_session;
    string m_filepath;
    nlohmann::json m_trace_events;

public:
    Instrumentor()
        : m_current_session(nullptr)
    { }

    void begin_session(const string& name, const string& filepath = "results.json")
    {
      std::lock_guard lock(m_mutex);
      m_filepath = filepath;
      if(m_current_session)
      {
        // If there is already a current session, then close it before beginning new one.
        // Subsequent profiling output meant for the original session will end up in the
        // newly opened session instead.  That's better than having badly formatted
        // profiling output.
        if(log::logger) // Edge case: begin_session() might be before Log::Init()
        {
          MGL_CORE_ERROR("Instrumentor::begin_session('{0}') when session '{1}' already open.", name, m_current_session->name);
        }
        internal_end_session();
      }
      m_current_session = new instrumentation_session({ name });
    }

    void end_session()
    {
      std::lock_guard lock(m_mutex);
      internal_end_session();
    }

    void write_profile(const profile_result& result)
    {
      std::stringstream tidss;
      tidss << result.thread_id;
      uint64_t tid = std::stoull(tidss.str());

      nlohmann::json traceEvent;
      traceEvent["cat"] = "function," + result.category;
      traceEvent["dur"] = result.elapsed_time.count();
      traceEvent["name"] = result.name;
      traceEvent["ph"] = "X";
      traceEvent["pid"] = result.process_id;
      traceEvent["tid"] = tid;
      traceEvent["ts"] = result.start.count();

      std::lock_guard lock(m_mutex);
      m_trace_events.push_back(traceEvent);
    }

    static Instrumentor& get()
    {
      static Instrumentor instance;
      return instance;
    }

private:
    void write_trace_document()
    {
      nlohmann::json document;

      document["otherData"] = "{ \"version\":\"mgl_core v" MGL_SEM_VERSION "\"}"_json;
      document["traceEvents"] = m_trace_events;

      std::ofstream o(m_filepath);
      o << std::setw(4) << document << std::endl;
      o.flush();
    }

    // Note: you must already own lock on m_mutex before
    // calling internal_end_session()
    void internal_end_session()
    {
      if(m_current_session)
      {
        write_trace_document();
        delete m_current_session;
        m_current_session = nullptr;
      }
    }
  };

  class InstrumentationTimer
  {
public:
    InstrumentationTimer(const char* name, const char* category)
        : m_name(name)
        , m_category(category)
        , m_stopped(false)
    {
      m_start_timepoint = std::chrono::steady_clock::now();
    }

    ~InstrumentationTimer()
    {
      if(!m_stopped)
        stop();
    }

    void stop()
    {
      auto end_timepoint = std::chrono::steady_clock::now();
      auto high_res_start = floating_point_microseconds{ m_start_timepoint.time_since_epoch() };
      auto elapsed_time = std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint).time_since_epoch() -
                          std::chrono::time_point_cast<std::chrono::microseconds>(m_start_timepoint).time_since_epoch();

      Instrumentor::get().write_profile(
          { m_name, high_res_start, elapsed_time, std::this_thread::get_id(), GET_PROCESS_ID(), m_category });

      m_stopped = true;
    }

private:
    const char *m_name, *m_category;
    std::chrono::time_point<std::chrono::steady_clock> m_start_timepoint;
    bool m_stopped;
  };

  namespace instrumentor_utils
  {

    template <size_t N>
    struct change_result
    {
      char Data[N];
    };

    template <size_t N, size_t K>
    constexpr auto cleanup_output_string(const char (&expr)[N], const char (&remove)[K])
    {
      change_result<N> result = {};

      size_t src_index = 0;
      size_t dst_index = 0;
      while(src_index < N)
      {
        size_t matchIndex = 0;
        while(matchIndex < K - 1 && src_index + matchIndex < N - 1 && expr[src_index + matchIndex] == remove[matchIndex])
          matchIndex++;
        if(matchIndex == K - 1)
          src_index += matchIndex;
        result.Data[dst_index++] = expr[src_index] == '"' ? '\'' : expr[src_index];
        src_index++;
      }
      return result;
    }
  } // namespace instrumentor_utils
} // namespace mgl_core

#ifndef MGL_CORE_PROFILE
#  define MGL_CORE_PROFILE 0
#endif
#if MGL_CORE_PROFILE
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#  if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) ||              \
      defined(__ghs__)
#    define MGL_CORE_FUNC_SIG __PRETTY_FUNCTION__
#  elif defined(__DMC__) && (__DMC__ >= 0x810)
#    define MGL_CORE_FUNC_SIG __PRETTY_FUNCTION__
#  elif(defined(__FUNCSIG__) || (_MSC_VER))
#    define MGL_CORE_FUNC_SIG __FUNCSIG__
#  elif(defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#    define MGL_CORE_FUNC_SIG __FUNCTION__
#  elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#    define MGL_CORE_FUNC_SIG __FUNC__
#  elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#    define MGL_CORE_FUNC_SIG __func__
#  elif defined(__cplusplus) && (__cplusplus >= 201103)
#    define MGL_CORE_FUNC_SIG __func__
#  else
#    define MGL_CORE_FUNC_SIG "MGL_CORE_FUNC_SIG unknown!"
#  endif

#  define MGL_CORE_PROFILE_BEGIN_SESSION() ::mgl_core::Instrumentor::get().begin_session("mgl_core")
#  define MGL_CORE_PROFILE_END_SESSION() ::mgl_core::Instrumentor::get().end_session()
#  define MGL_CORE_PROFILE_SCOPE(name, category)                                                                                 \
    constexpr auto fixedName = ::mgl_core::instrumentor_utils::cleanup_output_string(name, "__cdecl ");                          \
    ::mgl_core::InstrumentationTimer timer##__LINE__(fixedName.Data, category)
#  define MGL_CORE_PROFILE_FUNCTION(category) MGL_CORE_PROFILE_SCOPE(MGL_CORE_FUNC_SIG, category)
#else
#  define MGL_CORE_PROFILE_BEGIN_SESSION()
#  define MGL_CORE_PROFILE_END_SESSION()
#  define MGL_CORE_PROFILE_SCOPE(name, category)
#  define MGL_CORE_PROFILE_FUNCTION(category)
#endif