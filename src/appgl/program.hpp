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

namespace mgl
{
  class Program
  {
public:
    ~Program() = default;

    void release();
    bool released();

    const mgl_core::ref<Attribute> attribute(const mgl_core::string& name) const;
    const mgl_core::ref<Uniform> uniform(const mgl_core::string& name) const;
    const mgl_core::ref<UniformBlock> uniform_block(const mgl_core::string& name) const;
    const mgl_core::ref<Varying> varying(const mgl_core::string& name) const;
    const mgl_core::ref<Subroutine> subroutine(const mgl_core::string& name) const;

    const mgl_core::string_list attributes();
    const mgl_core::string_list uniforms();
    const mgl_core::string_list uniform_blocks();
    const mgl_core::string_list varyings();
    const mgl_core::string_list subroutines();

    size_t num_attributes();
    size_t num_uniforms();
    size_t num_uniform_blocks();
    size_t num_varyings();
    size_t num_subroutines();

    const mgl_core::ref<Uniform> operator[](const mgl_core::string& name) const;

    int geometry_input();
    int geometry_output();
    int geometry_vertices();
    bool is_transform();

private:
    friend class Context;
    friend class VertexArray;
    Program() = default;

    Context* m_context;
    int m_program_obj;
    int m_geometry_input;
    int m_geometry_output;
    int m_geometry_vertices;
    bool m_transform;
    bool m_released;
    int m_num_vertex_shader_subroutines;
    int m_num_fragment_shader_subroutines;
    int m_num_geometry_shader_subroutines;
    int m_num_tess_evaluation_shader_subroutines;
    int m_num_tess_control_shader_subroutines;

    UniformsMap m_uniforms_map;
    UniformBlocksMap m_uniform_blocks_map;
    AttributesMap m_attributes_map;
    VaryingsMap m_varyings_map;
    SubroutinesMap m_subroutines_map;
  };

  inline bool Program::released()
  {
    return m_released;
  }

  inline const mgl_core::ref<Attribute> Program::attribute(const mgl_core::string& name) const
  {
    if(m_attributes_map.find(name) == m_attributes_map.end())
    {
      return nullptr;
    }
    return m_attributes_map.at(name);
  }

  inline const mgl_core::ref<Uniform> Program::uniform(const mgl_core::string& name) const
  {
    if(m_uniforms_map.find(name) == m_uniforms_map.end())
    {
      return nullptr;
    }
    return m_uniforms_map.at(name);
  }

  inline const mgl_core::ref<UniformBlock> Program::uniform_block(const mgl_core::string& name) const
  {
    if(m_uniform_blocks_map.find(name) == m_uniform_blocks_map.end())
    {
      return nullptr;
    }
    return m_uniform_blocks_map.at(name);
  }

  inline const mgl_core::ref<Varying> Program::varying(const mgl_core::string& name) const
  {
    if(m_varyings_map.find(name) == m_varyings_map.end())
    {
      return nullptr;
    }
    return m_varyings_map.at(name);
  }

  inline const mgl_core::ref<Subroutine> Program::subroutine(const mgl_core::string& name) const
  {
    if(m_subroutines_map.find(name) == m_subroutines_map.end())
    {
      return nullptr;
    }
    return m_subroutines_map.at(name);
  }

  inline int Program::geometry_input()
  {
    return m_geometry_input;
  }

  inline int Program::geometry_output()
  {
    return m_geometry_output;
  }

  inline int Program::geometry_vertices()
  {
    return m_geometry_vertices;
  }

  inline bool Program::is_transform()
  {
    return m_transform;
  }

  inline size_t Program::num_attributes()
  {
    return m_attributes_map.size();
  }

  inline size_t Program::num_uniforms()
  {
    return m_uniforms_map.size();
  }

  inline size_t Program::num_uniform_blocks()
  {
    return m_uniform_blocks_map.size();
  }

  inline size_t Program::num_varyings()
  {
    return m_varyings_map.size();
  }

  inline size_t Program::num_subroutines()
  {
    return m_subroutines_map.size();
  }

  inline const mgl_core::ref<Uniform> Program::operator[](const mgl_core::string& name) const
  {
    return uniform(name);
  }

} // namespace mgl
