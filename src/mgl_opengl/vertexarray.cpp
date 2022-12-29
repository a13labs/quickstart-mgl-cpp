
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
#include "vertexarray.hpp"
#include "buffer.hpp"
#include "context.hpp"
#include "format.hpp"
#include "mgl_core/log.hpp"
#include "program.hpp"

namespace mgl_opengl
{
  void VertexArray::release()
  {
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    if(m_released)
    {
      return;
    }

    m_released = true;
    gl.DeleteVertexArrays(1, (GLuint*)&m_vertex_array_obj);
  }

  void VertexArray::render(mgl_opengl::render_mode mode, int vertices, int first, int instances)
  {
    MGL_CORE_ASSERT(!m_released, "Vertex Array already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    if(vertices < 0)
    {
      MGL_CORE_ASSERT(m_num_vertices >= 0, "cannot detect the number of vertices");
      vertices = m_num_vertices;
    }

    if(instances < 0)
    {
      instances = m_num_instances;
    }

    gl.UseProgram(m_program->m_program_obj);
    gl.BindVertexArray(m_vertex_array_obj);

    if(m_subroutines)
    {
      unsigned* subroutines = m_subroutines;

      if(m_program->m_num_vertex_shader_subroutines)
      {
        gl.UniformSubroutinesuiv(GL_VERTEX_SHADER, m_program->m_num_vertex_shader_subroutines, subroutines);
        subroutines += m_program->m_num_vertex_shader_subroutines;
      }

      if(m_program->m_num_fragment_shader_subroutines)
      {
        gl.UniformSubroutinesuiv(GL_FRAGMENT_SHADER, m_program->m_num_fragment_shader_subroutines, subroutines);
        subroutines += m_program->m_num_fragment_shader_subroutines;
      }

      if(m_program->m_num_geometry_shader_subroutines)
      {
        gl.UniformSubroutinesuiv(GL_GEOMETRY_SHADER, m_program->m_num_geometry_shader_subroutines, subroutines);
        subroutines += m_program->m_num_geometry_shader_subroutines;
      }

      if(m_program->m_num_tess_evaluation_shader_subroutines)
      {
        gl.UniformSubroutinesuiv(GL_TESS_EVALUATION_SHADER, m_program->m_num_tess_evaluation_shader_subroutines, subroutines);
        subroutines += m_program->m_num_tess_evaluation_shader_subroutines;
      }

      if(m_program->m_num_tess_control_shader_subroutines)
      {
        gl.UniformSubroutinesuiv(GL_TESS_CONTROL_SHADER, m_program->m_num_tess_control_shader_subroutines, subroutines);
      }
    }

    if(m_index_buffer != nullptr)
    {
      const void* ptr = (const void*)((GLintptr)first * m_index_element_size);
      gl.DrawElementsInstanced(mode, vertices, m_index_element_type, ptr, instances);
    }
    else
    {
      gl.DrawArraysInstanced(mode, first, vertices, instances);
    }
  }

  void VertexArray::render_indirect(const buffer_ref& buffer, mgl_opengl::render_mode mode, int count, int first)
  {
    MGL_CORE_ASSERT(!m_released, "Vertex Array already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    gl.UseProgram(m_program->m_program_obj);
    gl.BindVertexArray(m_vertex_array_obj);
    gl.BindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer->m_buffer_obj);

    if(m_subroutines)
    {
      unsigned* subroutines = m_subroutines;

      if(m_program->m_num_vertex_shader_subroutines)
      {
        gl.UniformSubroutinesuiv(GL_VERTEX_SHADER, m_program->m_num_vertex_shader_subroutines, subroutines);
        subroutines += m_program->m_num_vertex_shader_subroutines;
      }

      if(m_program->m_num_fragment_shader_subroutines)
      {
        gl.UniformSubroutinesuiv(GL_FRAGMENT_SHADER, m_program->m_num_fragment_shader_subroutines, subroutines);
        subroutines += m_program->m_num_fragment_shader_subroutines;
      }

      if(m_program->m_num_geometry_shader_subroutines)
      {
        gl.UniformSubroutinesuiv(GL_GEOMETRY_SHADER, m_program->m_num_geometry_shader_subroutines, subroutines);
        subroutines += m_program->m_num_geometry_shader_subroutines;
      }

      if(m_program->m_num_tess_evaluation_shader_subroutines)
      {
        gl.UniformSubroutinesuiv(GL_TESS_EVALUATION_SHADER, m_program->m_num_tess_evaluation_shader_subroutines, subroutines);
        subroutines += m_program->m_num_tess_evaluation_shader_subroutines;
      }

      if(m_program->m_num_tess_control_shader_subroutines)
      {
        gl.UniformSubroutinesuiv(GL_TESS_CONTROL_SHADER, m_program->m_num_tess_control_shader_subroutines, subroutines);
      }
    }

    const void* ptr = (const void*)((GLintptr)first * 20);

    if(m_index_buffer != nullptr)
    {
      gl.MultiDrawElementsIndirect(mode, m_index_element_type, ptr, count, 20);
    }
    else
    {
      gl.MultiDrawArraysIndirect(mode, ptr, count, 20);
    }
  }

  void VertexArray::transform(const mgl_core::ref_list<Buffer>& buffers,
                              mgl_opengl::render_mode mode,
                              int vertices,
                              int first,
                              int instances,
                              int buffer_offset)
  {
    MGL_CORE_ASSERT(!m_released, "Vertex Array already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    MGL_CORE_ASSERT(m_program->num_varyings(), "the program has no varyings")

    if(vertices < 0)
    {
      MGL_CORE_ASSERT(m_num_vertices >= 0, "cannot detect the number of vertices");
      vertices = m_num_vertices;
    }

    if(instances < 0)
    {
      instances = m_num_instances;
    }

    int output_mode = -1;

    // If a geo shader is present we need to sanity check the the rendering mode
    if(m_program->geometry_output() > -1)
    {
      output_mode = m_program->geometry_output();

      // The rendering mode must match the input type in the geometry shader
      // points, lines, lines_adjacency, triangles, triangles_adjacency
      switch(m_program->geometry_input())
      {
        case GL_POINTS:
          MGL_CORE_ASSERT(mode == GL_POINTS,
                          "Geometry shader expects POINTS as input. Change the "
                          "transform mode.");
          break;
        case GL_LINES:
          MGL_CORE_ASSERT(mode == GL_LINES || mode == GL_LINE_STRIP || mode == GL_LINE_LOOP || mode == GL_LINES_ADJACENCY,
                          "Geometry shader expects LINES, LINE_STRIP, GL_LINE_LOOP or "
                          "GL_LINES_ADJACENCY as input. Change the rendering mode.");
          break;
        case GL_LINES_ADJACENCY:
          MGL_CORE_ASSERT(mode == GL_LINES_ADJACENCY || mode == GL_LINE_STRIP_ADJACENCY,
                          "Geometry shader expects LINES_ADJACENCY or LINE_STRIP_ADJACENCY "
                          "as input. Change the rendering mode.");
          break;
        case GL_TRIANGLES:
          MGL_CORE_ASSERT(mode == GL_TRIANGLES || mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN,
                          "Geometry shader expects GL_TRIANGLES, GL_TRIANGLE_STRIP "
                          "or GL_TRIANGLE_FAN as input. Change the rendering mode.");
          break;
        case GL_TRIANGLES_ADJACENCY:
          MGL_CORE_ASSERT(mode == GL_TRIANGLES_ADJACENCY || mode == GL_TRIANGLE_STRIP_ADJACENCY,
                          "Geometry shader expects GL_TRIANGLES_ADJACENCY or "
                          "GL_TRIANGLE_STRIP_ADJACENCY as input. Change the rendering mode.");
          break;
        default:
          MGL_CORE_ERROR("Unexpected geometry shader input mode: %d", m_program->geometry_input());
          return;
          break;
      }
    }
    else
    {
      // If no geometry shader is present we need to determine the output mode by
      // looking at the input
      switch(mode)
      {
        case GL_POINTS: output_mode = GL_POINTS; break;
        case GL_LINES:
        case GL_LINE_LOOP:
        case GL_LINE_STRIP:
        case GL_LINES_ADJACENCY:
        case GL_LINE_STRIP_ADJACENCY: output_mode = GL_LINES; break;
        case GL_TRIANGLES:
        case GL_TRIANGLE_STRIP:
        case GL_TRIANGLE_FAN:
        case GL_TRIANGLES_ADJACENCY:
        case GL_TRIANGLE_STRIP_ADJACENCY: output_mode = GL_TRIANGLES; break;
        default: MGL_CORE_ASSERT(false, "Primitive mode not supported: %d", mode); return;
      }
    }

    gl.UseProgram(m_program->m_program_obj);
    gl.BindVertexArray(m_vertex_array_obj);

    int i = 0;
    for(auto&& buffer : buffers)
    {
      gl.BindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, i, buffer->m_buffer_obj, buffer_offset, buffer->m_size - buffer_offset);
      i++;
    }

    gl.Enable(GL_RASTERIZER_DISCARD);
    gl.BeginTransformFeedback(output_mode);

    if(m_subroutines)
    {
      unsigned* subroutines = m_subroutines;

      if(m_program->m_num_vertex_shader_subroutines)
      {
        gl.UniformSubroutinesuiv(GL_VERTEX_SHADER, m_program->m_num_vertex_shader_subroutines, subroutines);
        subroutines += m_program->m_num_vertex_shader_subroutines;
      }

      if(m_program->m_num_fragment_shader_subroutines)
      {
        gl.UniformSubroutinesuiv(GL_FRAGMENT_SHADER, m_program->m_num_fragment_shader_subroutines, subroutines);
        subroutines += m_program->m_num_fragment_shader_subroutines;
      }

      if(m_program->m_num_geometry_shader_subroutines)
      {
        gl.UniformSubroutinesuiv(GL_GEOMETRY_SHADER, m_program->m_num_geometry_shader_subroutines, subroutines);
        subroutines += m_program->m_num_geometry_shader_subroutines;
      }

      if(m_program->m_num_tess_evaluation_shader_subroutines)
      {
        gl.UniformSubroutinesuiv(GL_TESS_EVALUATION_SHADER, m_program->m_num_tess_evaluation_shader_subroutines, subroutines);
        subroutines += m_program->m_num_tess_evaluation_shader_subroutines;
      }

      if(m_program->m_num_tess_control_shader_subroutines)
      {
        gl.UniformSubroutinesuiv(GL_TESS_CONTROL_SHADER, m_program->m_num_tess_control_shader_subroutines, subroutines);
      }
    }

    if(m_index_buffer != nullptr)
    {
      const void* ptr = (const void*)((GLintptr)first * m_index_element_size);
      gl.DrawElementsInstanced(mode, vertices, m_index_element_type, ptr, instances);
    }
    else
    {
      gl.DrawArraysInstanced(mode, first, vertices, instances);
    }

    gl.EndTransformFeedback();
    if(~m_context->enable_flags() & mgl_opengl::enable_flag::RASTERIZER_DISCARD)
    {
      gl.Disable(GL_RASTERIZER_DISCARD);
    }
    gl.Flush();
  }

  void VertexArray::bind(int location,
                         const char* type,
                         const buffer_ref& buffer,
                         const char* format,
                         size_t offset,
                         int stride,
                         int divisor,
                         bool normalize)
  {
    MGL_CORE_ASSERT(!m_released, "Vertex Array already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    MGL_CORE_ASSERT(!(type[0] == 'f' && normalize), "invalid normalize");

    format_iterator it = format_iterator(format);

#ifdef MGL_CORE_ENABLE_ASSERTS
    format_info format_info = it.info();
    MGL_CORE_ASSERT(!(!format_info.valid || format_info.divisor || format_info.nodes != 1), "invalid format");
#endif

    format_node* node = it.next();
    MGL_CORE_ASSERT(node->type, "invalid format");

    char* ptr = (char*)offset;

    gl.BindVertexArray(m_vertex_array_obj);
    gl.BindBuffer(GL_ARRAY_BUFFER, buffer->m_buffer_obj);

    switch(type[0])
    {
      case 'f': gl.VertexAttribPointer(location, node->count, node->type, normalize, stride, ptr); break;
      case 'i': gl.VertexAttribIPointer(location, node->count, node->type, stride, ptr); break;
      case 'd': gl.VertexAttribLPointer(location, node->count, node->type, stride, ptr); break;
      default: MGL_CORE_ASSERT(false, "invalid type"); return;
    }

    gl.VertexAttribDivisor(location, divisor);
    gl.EnableVertexAttribArray(location);
  }

  void VertexArray::set_index_buffer(const buffer_ref& value)
  {
    m_index_buffer = value;
    m_num_vertices = (int)(m_index_buffer->size() / 4);
  }

} // namespace mgl_opengl
