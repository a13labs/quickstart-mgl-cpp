
/*
   Copyright 2020 Alexandre Pires (c.alexandre.pires@gmail.com)

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

namespace AppGL
{
  void Buffer::release()
  {
    if(m_released)
    {
      return;
    }
    m_released = true;

    const GLMethods& gl = m_context->gl();
    gl.DeleteBuffers(1, (GLuint*)&m_buffer_obj);
  }

  void Buffer::write(void* src, size_t size, size_t offset)
  {
    APPCORE_ASSERT(src, "invalid src")
    APPCORE_ASSERT(size > 0, "invalid data size: {0}", size)
    APPCORE_ASSERT(offset > 0, "invalid offset: {0}", offset)

    if(m_released)
      return;

    const GLMethods& gl = m_context->gl();

    gl.BindBuffer(GL_ARRAY_BUFFER, m_buffer_obj);
    gl.BufferSubData(GL_ARRAY_BUFFER, (GLintptr)offset, size, src);
  }

  void Buffer::read(void* dst, size_t size, size_t offset)
  {
    APPCORE_ASSERT(dst, "invalid dst")
    APPCORE_ASSERT(size > 0, "invalid data size: {0}", size)
    APPCORE_ASSERT(offset > 0, "invalid offset: {0}", offset)

    if(m_released)
      return;

    const GLMethods& gl = m_context->gl();

    gl.BindBuffer(GL_ARRAY_BUFFER, m_buffer_obj);
    void* map = gl.MapBufferRange(GL_ARRAY_BUFFER, offset, size, GL_MAP_READ_BIT);

    char* ptr = (char*)dst + offset;
    memcpy(ptr, map, size);

    gl.UnmapBuffer(GL_ARRAY_BUFFER);
  }

  void Buffer::clear()
  {
    if(m_released)
      return;

    const GLMethods& gl = m_context->gl();
    gl.BindBuffer(GL_ARRAY_BUFFER, m_buffer_obj);

    char* map = (char*)gl.MapBufferRange(GL_ARRAY_BUFFER, 0, m_size, GL_MAP_WRITE_BIT);

    if(!map)
    {
      APPCORE_ERROR("Buffer::clear, cannot map the buffer");
      return;
    }

    memset(map, 0, m_size);

    gl.UnmapBuffer(GL_ARRAY_BUFFER);
  }

  void Buffer::orphan(size_t size)
  {
    const GLMethods& gl = m_context->gl();
    gl.BindBuffer(GL_ARRAY_BUFFER, m_buffer_obj);
    gl.BufferData(GL_ARRAY_BUFFER, m_size, 0, m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  }

  void Buffer::bind_to_uniform_block(int binding, size_t size, size_t offset)
  {
    APPCORE_ASSERT(size > 0, "invalid data size: {0}", size)
    APPCORE_ASSERT(offset > 0, "invalid offset: {0}", offset)
    const GLMethods& gl = m_context->gl();
    gl.BindBufferRange(GL_UNIFORM_BUFFER, binding, m_buffer_obj, offset, size);
  }

  void Buffer::bind_to_storage_buffer(int binding, size_t size, size_t offset)
  {
    APPCORE_ASSERT(size > 0, "invalid data size: {0}", size)
    APPCORE_ASSERT(offset > 0, "invalid offset: {0}", offset)
    const GLMethods& gl = m_context->gl();
    gl.BindBufferRange(GL_SHADER_STORAGE_BUFFER, binding, m_buffer_obj, offset, size);
  }

} // namespace AppGL
