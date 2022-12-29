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

  class glsl_source
  {
public:
    enum type
    {
      VERTEX_SHADER,
      FRAGMENT_SHADER,
      GEOMETRY_SHADER,
      TESS_CONTROL_SHADER,
      TESS_EVALUATION_SHADER,

      GENERIC_PROGRAM
    };

    glsl_source(const mgl_core::string& source, glsl_source::type type = glsl_source::type::GENERIC_PROGRAM);
    ~glsl_source() = default;

    const mgl_core::string source(glsl_source::type type, const shader_defines& defines = {});
    const mgl_core::string source(const shader_defines& defines);

    const mgl_core::string_list outputs();

    const mgl_core::string vertex(const shader_defines& defines = {});
    const mgl_core::string fragment(const shader_defines& defines = {});
    const mgl_core::string geometry(const shader_defines& defines = {});
    const mgl_core::string tess_control(const shader_defines& defines = {});
    const mgl_core::string tess_evaluation(const shader_defines& defines = {});

private:
    void handle_includes(int depth = 0, int source_id = 0);

    mgl_core::string m_source;
    int m_version_code;
    glsl_source::type m_type;
    mgl_core::string_list m_attributes;
  };

  struct glsl_sources
  {
    const mgl_core::string_list sources;

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

    glsl_sources(glsl_source& source, const shader_defines& defines = {})
        : sources{ source.vertex(defines),
                   source.fragment(defines),
                   source.geometry(defines),
                   source.tess_control(defines),
                   source.tess_evaluation(defines) }
    { }

    inline bool empty() { return mgl_core::all_empty(sources); }
    inline const mgl_core::string& vertex() { return sources[glsl_source::type::VERTEX_SHADER]; }
    inline const mgl_core::string& fragment() { return sources[glsl_source::type::FRAGMENT_SHADER]; }
    inline const mgl_core::string& geometry() { return sources[glsl_source::type::GEOMETRY_SHADER]; }
    inline const mgl_core::string& tess_control() { return sources[glsl_source::type::TESS_CONTROL_SHADER]; }
    inline const mgl_core::string& tess_evaluation() { return sources[glsl_source::type::TESS_EVALUATION_SHADER]; }
  };

  inline const mgl_core::string glsl_source::vertex(const shader_defines& defines)
  {
    return source(glsl_source::type::VERTEX_SHADER, defines);
  }

  inline const mgl_core::string glsl_source::fragment(const shader_defines& defines)
  {
    return source(glsl_source::type::FRAGMENT_SHADER, defines);
  }

  inline const mgl_core::string glsl_source::geometry(const shader_defines& defines)
  {
    return source(glsl_source::type::GEOMETRY_SHADER, defines);
  }

  inline const mgl_core::string glsl_source::tess_control(const shader_defines& defines)
  {
    return source(glsl_source::type::TESS_CONTROL_SHADER, defines);
  }

  inline const mgl_core::string glsl_source::tess_evaluation(const shader_defines& defines)
  {
    return source(glsl_source::type::TESS_EVALUATION_SHADER, defines);
  }

} // namespace mgl_opengl