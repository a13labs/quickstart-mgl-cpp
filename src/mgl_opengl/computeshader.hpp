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

namespace mgl::opengl
{
  class compute_shader
  {
public:
    ~compute_shader() = default;

    void release();
    bool released();

    const uniform_ref uniform(const mgl::core::string& name) const;
    const uniform_block_ref uniform_block(const mgl::core::string& name) const;

    const mgl::core::string_list uniforms();
    const mgl::core::string_list uniform_blocks();

    size_t num_uniforms();
    size_t num_uniform_blocks();

    void run(int x = 1, int y = 1, int z = 1);

    const uniform_ref operator[](const mgl::core::string& name) const;

    int glo();

private:
    friend class context;
    compute_shader() = default;

    context* m_context;
    int m_program_obj;
    int m_shader_obj;
    uniforms_dict m_uniforms_map;
    uniform_blocks_dict m_uniform_blocks_map;
    bool m_released;
  };

  inline int compute_shader::glo()
  {
    return m_program_obj;
  }

  inline const uniform_ref compute_shader::uniform(const mgl::core::string& name) const
  {
    if(m_uniforms_map.find(name) == m_uniforms_map.end())
    {
      return nullptr;
    }
    return m_uniforms_map.at(name);
  }

  inline const uniform_block_ref compute_shader::uniform_block(const mgl::core::string& name) const
  {
    if(m_uniform_blocks_map.find(name) == m_uniform_blocks_map.end())
    {
      return nullptr;
    }
    return m_uniform_blocks_map.at(name);
  }

  inline size_t compute_shader::num_uniforms()
  {
    return m_uniforms_map.size();
  }

  inline size_t compute_shader::num_uniform_blocks()
  {
    return m_uniform_blocks_map.size();
  }

  inline const uniform_ref compute_shader::operator[](const mgl::core::string& name) const
  {
    return uniform(name);
  }

  inline bool compute_shader::released()
  {
    return m_released;
  }

} // namespace  mgl::opengl