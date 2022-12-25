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
#include "appgl.hpp"

namespace AppGL
{
  class Program
  {
public:
    ~Program() { release(); }

    void release();
    bool released();

    const AppCore::Ref<Attribute> attribute(const AppCore::String& name) const;
    const AppCore::Ref<Uniform> uniform(const AppCore::String& name) const;
    const AppCore::Ref<UniformBlock> uniform_block(const AppCore::String& name) const;
    const AppCore::Ref<Varying> varying(const AppCore::String& name) const;
    const AppCore::Ref<Subroutine> subroutine(const AppCore::String& name) const;

    const AppCore::StringList attributes();
    const AppCore::StringList uniforms();
    const AppCore::StringList uniform_blocks();
    const AppCore::StringList varyings();
    const AppCore::StringList subroutines();

    size_t num_attributes();
    size_t num_uniforms();
    size_t num_uniform_blocks();
    size_t num_varyings();
    size_t num_subroutines();

    const AppCore::Ref<Uniform> operator[](const AppCore::String& name) const;

    int geometry_input();
    int geometry_output();
    int geometry_vertices();
    bool is_transform();

private:
    friend class Context;
    Program() = default;

    Context* m_context;
    int m_program_obj;
    int m_geometry_input;
    int m_geometry_output;
    int m_geometry_vertices;
    bool m_transform;
    bool m_released;
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

  inline const AppCore::Ref<Attribute> Program::attribute(const AppCore::String& name) const
  {
    if(m_attributes_map.find(name) == m_attributes_map.end())
    {
      return nullptr;
    }
    return m_attributes_map.at(name);
  }

  inline const AppCore::Ref<Uniform> Program::uniform(const AppCore::String& name) const
  {
    if(m_uniforms_map.find(name) == m_uniforms_map.end())
    {
      return nullptr;
    }
    return m_uniforms_map.at(name);
  }

  inline const AppCore::Ref<UniformBlock> Program::uniform_block(const AppCore::String& name) const
  {
    if(m_uniform_blocks_map.find(name) == m_uniform_blocks_map.end())
    {
      return nullptr;
    }
    return m_uniform_blocks_map.at(name);
  }

  inline const AppCore::Ref<Varying> Program::varying(const AppCore::String& name) const
  {
    if(m_varyings_map.find(name) == m_varyings_map.end())
    {
      return nullptr;
    }
    return m_varyings_map.at(name);
  }

  inline const AppCore::Ref<Subroutine> Program::subroutine(const AppCore::String& name) const
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

  inline const AppCore::Ref<Uniform> Program::operator[](const AppCore::String& name) const
  {
    return uniform(name);
  }

} // namespace AppGL
