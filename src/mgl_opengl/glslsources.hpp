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

namespace mgl_opengl
{
  struct glsl_sources
  {
    enum type
    {
      VERTEX_SHADER,
      FRAGMENT_SHADER,
      GEOMETRY_SHADER,
      TESS_EVALUATION_SHADER,
      TESS_CONTROL_SHADER,

      COUNT,
    };

    mgl_core::string sources[glsl_sources::COUNT];

    glsl_sources(const mgl_core::string& vs_source,
                 const mgl_core::string& fs_source,
                 const mgl_core::string& gs_source,
                 const mgl_core::string& tes_source,
                 const mgl_core::string& tcs_source)
        : sources{ vs_source, fs_source, gs_source, tes_source, tcs_source }
    { }

    glsl_sources(const mgl_core::string& vs_source,
                 const mgl_core::string& fs_source,
                 const mgl_core::string& gs_source,
                 const mgl_core::string& tes_source)
        : sources{ vs_source, fs_source, gs_source, tes_source, no_shader }
    { }

    glsl_sources(const mgl_core::string& vs_source, const mgl_core::string& fs_source, const mgl_core::string& gs_source)
        : sources{ vs_source, fs_source, gs_source, no_shader, no_shader }
    { }

    glsl_sources(const mgl_core::string& vs_source, const mgl_core::string& fs_source)
        : sources{ vs_source, fs_source, no_shader, no_shader, no_shader }
    { }

    glsl_sources(const mgl_core::string& vs_source)
        : sources{ vs_source, no_shader, no_shader, no_shader, no_shader }
    { }

    inline const mgl_core::string& vertex() { return sources[type::VERTEX_SHADER]; }
    inline const mgl_core::string& fragment() { return sources[type::FRAGMENT_SHADER]; }
    inline const mgl_core::string& geometry() { return sources[type::GEOMETRY_SHADER]; }
    inline const mgl_core::string& tess_control() { return sources[type::TESS_CONTROL_SHADER]; }
    inline const mgl_core::string& tess_evaluation() { return sources[type::TESS_EVALUATION_SHADER]; }
  };
} // namespace mgl_opengl