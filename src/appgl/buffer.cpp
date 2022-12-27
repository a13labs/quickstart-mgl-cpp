
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
#include "buffer.hpp"
#include "appcore/log.hpp"
#include "context.hpp"

namespace mgl
{
  void Buffer::release()
  {
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    if(m_released)
    {
      return;
    }

    m_released = true;

    gl.DeleteBuffers(1, (GLuint*)&m_buffer_obj);
  }

  bool Buffer::write(const void* src, size_t size, size_t offset)
  {
    MGL_CORE_ASSERT(!m_released, "Buffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(offset >= 0, "invalid offset: {0}", offset)
    MGL_CORE_ASSERT(size + offset <= m_size, "out of bounds")

    const GLMethods& gl = m_context->gl();

    gl.BindBuffer(GL_ARRAY_BUFFER, m_buffer_obj);
    gl.BufferSubData(GL_ARRAY_BUFFER, (GLintptr)offset, size, src);

    return gl.GetError() == GL_NO_ERROR;
  }

  bool Buffer::read_into(void* dst, size_t dst_size, size_t read_size, size_t read_offset, size_t write_offset)
  {
    MGL_CORE_ASSERT(!m_released, "Buffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(read_size >= 0, "invalid size: {0}", read_size)
    MGL_CORE_ASSERT(read_offset >= 0, "invalid offset: {0}", read_offset)
    MGL_CORE_ASSERT(write_offset >= 0, "invalid write offset: {0}", write_offset)
    MGL_CORE_ASSERT(m_size >= read_size + read_offset, "out of bounds")
    MGL_CORE_ASSERT(dst_size <= write_offset + read_size, "out of bounds")

    const GLMethods& gl = m_context->gl();

    gl.BindBuffer(GL_ARRAY_BUFFER, m_buffer_obj);
    auto map = gl.MapBufferRange(GL_ARRAY_BUFFER, read_offset, read_size, GL_MAP_READ_BIT);

    char* ptr = (char*)dst + write_offset;
    memcpy(ptr, map, read_size);

    gl.UnmapBuffer(GL_ARRAY_BUFFER);
    return gl.GetError() == GL_NO_ERROR;
  }

  void Buffer::clear()
  {
    MGL_CORE_ASSERT(!m_released, "Buffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    const GLMethods& gl = m_context->gl();
    gl.BindBuffer(GL_ARRAY_BUFFER, m_buffer_obj);

    char* map = (char*)gl.MapBufferRange(GL_ARRAY_BUFFER, 0, m_size, GL_MAP_WRITE_BIT);

    if(!map)
    {
      MGL_CORE_ERROR("Buffer::clear, cannot map the buffer");
      return;
    }

    memset(map, 0, m_size);

    gl.UnmapBuffer(GL_ARRAY_BUFFER);
  }

  void Buffer::orphan(size_t size)
  {
    MGL_CORE_ASSERT(!m_released, "Buffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    const GLMethods& gl = m_context->gl();
    gl.BindBuffer(GL_ARRAY_BUFFER, m_buffer_obj);
    gl.BufferData(GL_ARRAY_BUFFER, size, 0, m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  }

  void Buffer::bind_to_uniform_block(int binding, size_t size, size_t offset)
  {
    MGL_CORE_ASSERT(!m_released, "Buffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(size > 0, "invalid data size: {0}", size)
    MGL_CORE_ASSERT(offset > 0, "invalid offset: {0}", offset)

    const GLMethods& gl = m_context->gl();
    gl.BindBufferRange(GL_UNIFORM_BUFFER, binding, m_buffer_obj, offset, size);
  }

  void Buffer::bind_to_storage_buffer(int binding, size_t size, size_t offset)
  {
    MGL_CORE_ASSERT(!m_released, "Buffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(size > 0, "invalid data size: {0}", size)
    MGL_CORE_ASSERT(offset > 0, "invalid offset: {0}", offset)

    const GLMethods& gl = m_context->gl();
    gl.BindBufferRange(GL_SHADER_STORAGE_BUFFER, binding, m_buffer_obj, offset, size);
  }

} // namespace mgl
