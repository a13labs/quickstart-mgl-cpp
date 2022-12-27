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
  class VertexArray
  {
public:
    ~VertexArray() = default;

public:
    void release();
    bool released();

    void render(AppGL::RenderMode mode = AppGL::RenderMode::TRIANGLES, int vertices = -1, int first = 0, int instances = -1);
    void render(int instances);
    void render_indirect(const AppCore::Ref<Buffer>& buffer, AppGL::RenderMode mode, int count = -1, int first = -1);
    void
    transform(const AppCore::Ref<Buffer>& buffer, AppGL::RenderMode mode, int vertices = -1, int first = 0, int instances = -1);

    void transform(const AppCore::ListRef<Buffer>& buffers,
                   AppGL::RenderMode mode,
                   int vertices = -1,
                   int first = 0,
                   int instances = -1,
                   int buffer_offset = 0);

    void bind(int location,
              const char* type,
              const AppCore::Ref<Buffer>& buffer,
              const char* format,
              size_t offset = 0,
              int stride = 0,
              int divisor = 0,
              bool normalize = false);

    void set_index_buffer(const AppCore::Ref<Buffer>& value);

    int vertices();
    int instances();

private:
    friend class Context;
    VertexArray() = default;

    Context* m_context;
    AppCore::Ref<Program> m_program;
    AppCore::Ref<Buffer> m_index_buffer;
    int m_index_element_size;
    int m_index_element_type;
    unsigned* m_subroutines;
    int m_num_subroutines;
    int m_vertex_array_obj;
    int m_num_vertices;
    int m_num_instances;
    bool m_released;
  };

  inline void
  VertexArray::transform(const AppCore::Ref<Buffer>& buffer, AppGL::RenderMode mode, int vertices, int first, int instances)
  {
    transform({ buffer }, mode, vertices, first, instances);
  }

  inline void VertexArray::render(int instances)
  {
    render(AppGL::TRIANGLES, -1, 0, instances);
  }

  inline int VertexArray::vertices()
  {
    return m_num_vertices;
  }

  inline int VertexArray::instances()
  {
    return m_num_instances;
  }

} // namespace AppGL
