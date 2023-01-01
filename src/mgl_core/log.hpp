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

namespace mgl::core
{
  namespace log
  {
    void init(const string& file = "app.log");
    extern ref<spdlog::logger> logger;
  }; // namespace log

} // namespace  mgl::core

// log macros
#define MGL_CORE_TRACE(...) ::mgl::core::log::logger->trace(__VA_ARGS__)
#define MGL_CORE_INFO(...) ::mgl::core::log::logger->info(__VA_ARGS__)
#define MGL_CORE_WARN(...) ::mgl::core::log::logger->warn(__VA_ARGS__)
#define MGL_CORE_ERROR(...) ::mgl::core::log::logger->error(__VA_ARGS__)
#define MGL_CORE_CRITICAL(...) ::mgl::core::log::logger->critical(__VA_ARGS__)
