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
    enum RenderMode
    {
      POINTS = 0x0000,
      LINES = 0x0001,
      LINE_LOOP = 0x0002,
      LINE_STRIP = 0x0003,
      TRIANGLES = 0x0004,
      TRIANGLE_STRIP = 0x0005,
      TRIANGLE_FAN = 0x0006,
      LINES_ADJACENCY = 0x000A,
      LINE_STRIP_ADJACENCY = 0x000B,
      TRIANGLES_ADJACENCY = 0x000C,
      TRIANGLE_STRIP_ADJACENCY = 0x0000D,
      PATCHES = 0x000E,
    };

    ~VertexArray() { release(); }

public:
    void release();
    bool released();

    void render(VertexArray::RenderMode mode, int vertices = -1, int first = 0, int instances = -1);
    void render_indirect(const AppCore::Ref<Buffer>& buffer, VertexArray::RenderMode mode, int count = -1, int first = -1);
    void transform(
        const AppCore::Ref<Buffer>& buffer, VertexArray::RenderMode mode, int vertices = -1, int first = 0, int instances = -1);

    void transform(const AppCore::VectorRef<Buffer>& buffers,
                   VertexArray::RenderMode mode,
                   int vertices = -1,
                   int first = 0,
                   int instances = -1,
                   int buffer_offset = 0);

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
} // namespace AppGL
