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
#include "base.hpp"
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace AppCore
{

  class Log
  {
public:
    static void Init();
    static Ref<spdlog::logger>& GetLogger() { return sLogger; }

private:
    static Ref<spdlog::logger> sLogger;
  };

} // namespace AppCore

// log macros
#define APPCORE_TRACE(...) ::AppCore::Log::GetLogger()->trace(__VA_ARGS__)
#define APPCORE_INFO(...) ::AppCore::Log::GetLogger()->info(__VA_ARGS__)
#define APPCORE_WARN(...) ::AppCore::Log::GetLogger()->warn(__VA_ARGS__)
#define APPCORE_ERROR(...) ::AppCore::Log::GetLogger()->error(__VA_ARGS__)
#define APPCORE_CRITICAL(...) ::AppCore::Log::GetLogger()->critical(__VA_ARGS__)
