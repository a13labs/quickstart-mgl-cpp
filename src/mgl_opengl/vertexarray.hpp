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
  class vertex_array
  {
public:
    ~vertex_array() = default;

public:
    void release();
    bool released();

    void render(mgl::opengl::render_mode mode = mgl::opengl::render_mode::TRIANGLES,
                int vertices = -1,
                int first = 0,
                int instances = -1);
    void render(int instances);
    void render_indirect(const buffer_ref& buffer, mgl::opengl::render_mode mode, int count = -1, int first = -1);
    void transform(const buffer_ref& b, mgl::opengl::render_mode mode, int vertices = -1, int first = 0, int instances = -1);

    void transform(const mgl::core::ref_list<buffer>& buffers,
                   mgl::opengl::render_mode mode,
                   int vertices = -1,
                   int first = 0,
                   int instances = -1,
                   int buffer_offset = 0);

    void bind(int location,
              const char* type,
              const buffer_ref& buffer,
              const char* format,
              size_t offset = 0,
              int stride = 0,
              int divisor = 0,
              bool normalize = false);

    void set_index_buffer(const buffer_ref& value);

    int vertices();
    int instances();

private:
    friend class context;
    vertex_array() = default;

    context* m_context;
    program_ref m_program;
    buffer_ref m_index_buffer;
    int m_index_element_size;
    int m_index_element_type;
    int m_vertex_array_obj;
    int m_num_vertices;
    int m_num_instances;
    bool m_released;
  };

  inline void vertex_array::transform(const buffer_ref& b, mgl::opengl::render_mode mode, int vertices, int first, int instances)
  {
    transform(mgl::core::ref_list<buffer>({ b }), mode, vertices, first, instances);
  }

  inline void vertex_array::render(int instances)
  {
    render(mgl::opengl::TRIANGLES, -1, 0, instances);
  }

  inline int vertex_array::vertices()
  {
    return m_num_vertices;
  }

  inline int vertex_array::instances()
  {
    return m_num_instances;
  }

} // namespace  mgl::opengl
