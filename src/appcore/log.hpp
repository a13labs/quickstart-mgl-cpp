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
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace AppCore
{

  namespace Log
  {
    void init(const String& file = "app.log");
    extern Ref<spdlog::logger> logger;
  }; // namespace Log

} // namespace AppCore

// log macros
#define APPCORE_TRACE(...) ::AppCore::Log::logger->trace(__VA_ARGS__)
#define APPCORE_INFO(...) ::AppCore::Log::logger->info(__VA_ARGS__)
#define APPCORE_WARN(...) ::AppCore::Log::logger->warn(__VA_ARGS__)
#define APPCORE_ERROR(...) ::AppCore::Log::logger->error(__VA_ARGS__)
#define APPCORE_CRITICAL(...) ::AppCore::Log::logger->critical(__VA_ARGS__)
