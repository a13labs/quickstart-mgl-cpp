
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
#include "framebuffer.hpp"
#include "buffer.hpp"
#include "context.hpp"
#include "datatype.hpp"
#include "mgl_core/log.hpp"

namespace mgl_opengl
{
  void Framebuffer::release()
  {
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    if(m_released)
    {
      return;
    }

    m_released = true;

    if(m_framebuffer_obj)
    {
      gl.DeleteFramebuffers(1, (GLuint*)&m_framebuffer_obj);
      delete[] m_draw_buffers;
    }
  }

  void Framebuffer::clear(float r, float g, float b, float a, float depth, const mgl_core::viewport_2d& viewport)
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    gl.BindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_obj);

    if(m_framebuffer_obj)
    {
      gl.DrawBuffers(m_draw_buffers_len, m_draw_buffers);
    }

    gl.ClearColor(r, g, b, a);
    gl.ClearDepth(depth);

    for(int i = 0; i < m_draw_buffers_len; ++i)
    {
      gl.ColorMaski(i, m_color_masks[i].r, m_color_masks[i].g, m_color_masks[i].b, m_color_masks[i].a);
    }

    gl.DepthMask(m_depth_mask);

    // Respect the passed in viewport even with scissor enabled
    if(viewport != mgl_core::null_viewport_2d)
    {
      gl.Enable(GL_SCISSOR_TEST);
      gl.Scissor(viewport.x, viewport.y, viewport.width, viewport.height);
      gl.Clear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

      // restore scissor if enabled
      if(m_scissor_enabled)
      {
        gl.Scissor(m_scissor.x, m_scissor.y, m_scissor.width, m_scissor.height);
      }
      else
      {
        gl.Disable(GL_SCISSOR_TEST);
      }
    }
    else
    {
      // clear with scissor if enabled
      if(m_scissor_enabled)
      {
        gl.Enable(GL_SCISSOR_TEST);
        gl.Scissor(m_scissor.x, m_scissor.y, m_scissor.width, m_scissor.height);
      }

      gl.Clear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }

    gl.BindFramebuffer(GL_FRAMEBUFFER, m_context->m_bound_framebuffer->m_framebuffer_obj);
  }

  void Framebuffer::use()
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    gl.BindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_obj);

    if(m_framebuffer_obj)
    {
      gl.DrawBuffers(m_draw_buffers_len, m_draw_buffers);
    }

    gl.Viewport(m_viewport.x, m_viewport.y, m_viewport.width, m_viewport.height);

    if(m_scissor_enabled)
    {
      gl.Enable(GL_SCISSOR_TEST);
      gl.Scissor(m_scissor.x, m_scissor.y, m_scissor.width, m_scissor.height);
    }
    else
    {
      gl.Disable(GL_SCISSOR_TEST);
    }

    for(int i = 0; i < m_draw_buffers_len; ++i)
    {
      gl.ColorMaski(i, m_color_masks[i].r, m_color_masks[i].g, m_color_masks[i].b, m_color_masks[i].a);
    }

    gl.DepthMask(m_depth_mask);

    m_context->m_bound_framebuffer = shared_from_this();
  }

  bool Framebuffer::read_into(mgl_core::mem_buffer<uint8_t>& dst,
                              const mgl_core::viewport_2d& viewport,
                              int components,
                              int attachment,
                              int alignment,
                              const char* dtype,
                              size_t write_offset)
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8, "alignment must be 1, 2, 4 or 8");
    data_type* data_type = from_dtype(dtype, strlen(dtype));
    MGL_CORE_ASSERT(data_type != nullptr, "invalid dtype");

    bool read_depth = false;

    if(attachment == -1)
    {
      components = 1;
      read_depth = true;
    }

    int x = 0;
    int y = 0;
    int width = m_width;
    int height = m_height;
    if(viewport != mgl_core::null_viewport_2d)
    {
      x = viewport.x;
      y = viewport.y;
      width = viewport.width;
      height = viewport.height;
    }

    size_t expected_size = width * components * data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * height;

    MGL_CORE_ASSERT(dst.size_bytes() >= write_offset + expected_size, "out of bounds");

    int pixel_type = data_type->gl_type;
    int base_format = read_depth ? GL_DEPTH_COMPONENT : data_type->base_format[components];

    char* ptr = (char*)dst.data() + write_offset;

    gl.BindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_obj);
    gl.ReadBuffer(read_depth ? GL_NONE : (GL_COLOR_ATTACHMENT0 + attachment));
    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.ReadPixels(x, y, width, height, base_format, pixel_type, ptr);
    gl.BindFramebuffer(GL_FRAMEBUFFER, m_context->m_bound_framebuffer->m_framebuffer_obj);

    return gl.GetError() == GL_NO_ERROR;
  }

  bool Framebuffer::read_into(mgl_core::ref<Buffer> dst,
                              const mgl_core::viewport_2d& viewport,
                              int components,
                              int attachment,
                              int alignment,
                              const char* dtype,
                              size_t write_offset)
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8, "alignment must be 1, 2, 4 or 8");
    data_type* data_type = from_dtype(dtype, strlen(dtype));
    MGL_CORE_ASSERT(data_type != nullptr, "invalid dtype");

    bool read_depth = false;

    if(attachment == -1)
    {
      components = 1;
      read_depth = true;
    }

    int x = 0;
    int y = 0;
    int width = m_width;
    int height = m_height;
    if(viewport != mgl_core::null_viewport_2d)
    {
      x = viewport.x;
      y = viewport.y;
      width = viewport.width;
      height = viewport.height;
    }

    int pixel_type = data_type->gl_type;
    int base_format = read_depth ? GL_DEPTH_COMPONENT : data_type->base_format[components];

    gl.BindBuffer(GL_PIXEL_PACK_BUFFER, dst->m_buffer_obj);
    gl.BindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_obj);
    gl.ReadBuffer(read_depth ? GL_NONE : (GL_COLOR_ATTACHMENT0 + attachment));
    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.ReadPixels(x, y, width, height, base_format, pixel_type, (void*)write_offset);
    gl.BindFramebuffer(GL_FRAMEBUFFER, m_context->m_bound_framebuffer->m_framebuffer_obj);
    gl.BindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    return gl.GetError() == GL_NO_ERROR;
  }

  void Framebuffer::set_color_mask(const color_masks& masks)
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(masks.size() != (size_t)m_draw_buffers_len, "color_mask must be a match buffers len");
    const GLMethods& gl = m_context->gl();

    m_color_masks = masks;

    if(m_framebuffer_obj == m_context->m_bound_framebuffer->m_framebuffer_obj)
    {
      for(int i = 0; i < m_draw_buffers_len; ++i)
      {
        gl.ColorMaski(i, m_color_masks[i].r, m_color_masks[i].g, m_color_masks[i].b, m_color_masks[i].a);
      }
    }
  }

  void Framebuffer::set_depth_mask(bool value)
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    m_depth_mask = value;

    if(m_framebuffer_obj == m_context->m_bound_framebuffer->m_framebuffer_obj)
    {
      gl.DepthMask(m_depth_mask);
    }
  }

  bool Framebuffer::bits(int& red_bits, int& green_bits, int& blue_bits, int& alpha_bits, int& depth_bits, int& stencil_bits)
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(!m_framebuffer_obj, "Only the default_framebuffer have bits");
    const GLMethods& gl = m_context->gl();

    gl.BindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_obj);
    gl.GetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, &red_bits);
    gl.GetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, &green_bits);
    gl.GetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, &blue_bits);
    gl.GetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &alpha_bits);
    gl.GetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &depth_bits);
    gl.GetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &stencil_bits);
    gl.BindFramebuffer(GL_FRAMEBUFFER, m_context->m_bound_framebuffer->m_framebuffer_obj);

    return gl.GetError() == GL_NO_ERROR;
  }

  void Framebuffer::set_viewport(const mgl_core::viewport_2d& r)
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(m_context->released())
      return;

    m_viewport = r;

    if(m_framebuffer_obj == m_context->m_bound_framebuffer->m_framebuffer_obj)
    {
      gl.Viewport(m_viewport.x, m_viewport.y, m_viewport.width, m_viewport.height);
    }
  }

  void Framebuffer::set_scissor(const mgl_core::viewport_2d& r)
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(m_context->released())
      return;

    m_scissor = r;

    if(m_framebuffer_obj == m_context->m_bound_framebuffer->m_framebuffer_obj)
    {
      gl.Scissor(m_scissor.x, m_scissor.y, m_scissor.width, m_scissor.height);
    }
  }

} // namespace mgl_opengl
