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
  class Program
  {
public:
    ~Program() = default;

    void release();
    bool released();

    const attribute_ref attribute(const mgl_core::string& name) const;
    const uniform_ref uniform(const mgl_core::string& name) const;
    const uniform_block_ref uniform_block(const mgl_core::string& name) const;
    const varying_ref varying(const mgl_core::string& name) const;
    const subroutine_ref subroutine(const mgl_core::string& name) const;

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

    const uniform_ref operator[](const mgl_core::string& name) const;

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

    uniforms_dict m_uniforms_map;
    uniform_blocks_dict m_uniform_blocks_map;
    attributes_dict m_attributes_map;
    varyings_dict m_varyings_map;
    subroutines_dict m_subroutines_map;
  };

  inline bool Program::released()
  {
    return m_released;
  }

  inline const attribute_ref Program::attribute(const mgl_core::string& name) const
  {
    if(m_attributes_map.find(name) == m_attributes_map.end())
    {
      return nullptr;
    }
    return m_attributes_map.at(name);
  }

  inline const uniform_ref Program::uniform(const mgl_core::string& name) const
  {
    if(m_uniforms_map.find(name) == m_uniforms_map.end())
    {
      return nullptr;
    }
    return m_uniforms_map.at(name);
  }

  inline const uniform_block_ref Program::uniform_block(const mgl_core::string& name) const
  {
    if(m_uniform_blocks_map.find(name) == m_uniform_blocks_map.end())
    {
      return nullptr;
    }
    return m_uniform_blocks_map.at(name);
  }

  inline const varying_ref Program::varying(const mgl_core::string& name) const
  {
    if(m_varyings_map.find(name) == m_varyings_map.end())
    {
      return nullptr;
    }
    return m_varyings_map.at(name);
  }

  inline const subroutine_ref Program::subroutine(const mgl_core::string& name) const
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

  inline const uniform_ref Program::operator[](const mgl_core::string& name) const
  {
    return uniform(name);
  }

} // namespace mgl_opengl
