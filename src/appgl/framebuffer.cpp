
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
#include "appcore/log.hpp"
#include "buffer.hpp"
#include "context.hpp"
#include "datatype.hpp"

namespace AppGL
{
  void Framebuffer::release()
  {
    APPCORE_ASSERT(!m_context, "No context");
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

  void Framebuffer::clear(float r, float g, float b, float a, float depth, const Rect& rect)
  {
    APPCORE_ASSERT(!m_released, "Framebuffer already released");
    APPCORE_ASSERT(!m_context, "No context");
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
      gl.ColorMaski(i, m_color_mask[i * 4 + 0], m_color_mask[i * 4 + 1], m_color_mask[i * 4 + 2], m_color_mask[i * 4 + 3]);
    }

    gl.DepthMask(m_depth_mask);

    bool viewport = !(rect.X == 0 && rect.Y == 0 && rect.W == m_width && rect.H == m_height);

    // Respect the passed in viewport even with scissor enabled
    if(viewport)
    {
      gl.Enable(GL_SCISSOR_TEST);
      gl.Scissor(rect.X, rect.Y, rect.W, rect.H);
      gl.Clear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

      // restore scissor if enabled
      if(m_scissor_enabled)
      {
        gl.Scissor(m_scissor.X, m_scissor.Y, m_scissor.W, m_scissor.H);
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
        gl.Scissor(m_scissor.X, m_scissor.Y, m_scissor.W, m_scissor.H);
      }

      gl.Clear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }

    gl.BindFramebuffer(GL_FRAMEBUFFER, m_context->m_bound_framebuffer->m_framebuffer_obj);
  }

  void Framebuffer::use()
  {
    APPCORE_ASSERT(!m_released, "Framebuffer already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    gl.BindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_obj);

    if(m_framebuffer_obj)
    {
      gl.DrawBuffers(m_draw_buffers_len, m_draw_buffers);
    }

    gl.Viewport(m_viewport.X, m_viewport.Y, m_viewport.W, m_viewport.H);

    if(m_scissor_enabled)
    {
      gl.Enable(GL_SCISSOR_TEST);
      gl.Scissor(m_scissor.X, m_scissor.Y, m_scissor.W, m_scissor.H);
    }
    else
    {
      gl.Disable(GL_SCISSOR_TEST);
    }

    for(int i = 0; i < m_draw_buffers_len; ++i)
    {
      gl.ColorMaski(i, m_color_mask[i * 4 + 0], m_color_mask[i * 4 + 1], m_color_mask[i * 4 + 2], m_color_mask[i * 4 + 3]);
    }

    gl.DepthMask(m_depth_mask);

    m_context->m_bound_framebuffer = MAKE_THIS_REF();
  }

  void Framebuffer::read(
      void* dst, const Rect& viewport, int components, int attachment, int alignment, const char* dtype, size_t write_offset)
  {
    APPCORE_ASSERT(!m_released, "Framebuffer already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    APPCORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8, "alignment must be 1, 2, 4 or 8");

    auto data_type = from_dtype(dtype, strlen(dtype));
    APPCORE_ASSERT(data_type != nullptr, "invalid dtype");

    bool read_depth = false;

    if(attachment == -1)
    {
      components = 1;
      read_depth = true;
    }

    int expected_size = viewport.W * components * data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * viewport.H;

    int pixel_type = data_type->gl_type;
    int base_format = read_depth ? GL_DEPTH_COMPONENT : data_type->base_format[components];

    char* ptr = (char*)dst + write_offset;

    gl.BindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_obj);
    gl.ReadBuffer(read_depth ? GL_NONE : (GL_COLOR_ATTACHMENT0 + attachment));
    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.ReadPixels(viewport.X, viewport.Y, viewport.W, viewport.H, base_format, pixel_type, ptr);
    gl.BindFramebuffer(GL_FRAMEBUFFER, m_context->m_bound_framebuffer->m_framebuffer_obj);
  }

  void Framebuffer::read(AppCore::Ref<Buffer> dst,
                         const Rect& viewport,
                         int components,
                         int attachment,
                         int alignment,
                         const char* dtype,
                         size_t write_offset)
  {
    APPCORE_ASSERT(!m_released, "Framebuffer already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    APPCORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8, "alignment must be 1, 2, 4 or 8");

    DataType* data_type = from_dtype(dtype, strlen(dtype));
    APPCORE_ASSERT(data_type != nullptr, "invalid dtype");

    bool read_depth = false;

    if(attachment == -1)
    {
      components = 1;
      read_depth = true;
    }

    int expected_size = viewport.W * components * data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * viewport.H;

    int pixel_type = data_type->gl_type;
    int base_format = read_depth ? GL_DEPTH_COMPONENT : data_type->base_format[components];

    gl.BindBuffer(GL_PIXEL_PACK_BUFFER, dst->m_buffer_obj);
    gl.BindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_obj);
    gl.ReadBuffer(read_depth ? GL_NONE : (GL_COLOR_ATTACHMENT0 + attachment));
    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.ReadPixels(viewport.X, viewport.Y, viewport.W, viewport.H, base_format, pixel_type, (void*)write_offset);
    gl.BindFramebuffer(GL_FRAMEBUFFER, m_context->m_bound_framebuffer->m_framebuffer_obj);
    gl.BindBuffer(GL_PIXEL_PACK_BUFFER, 0);
  }

  void Framebuffer::set_color_mask(const ColorMask& mask)
  {
    APPCORE_ASSERT(!m_released, "Framebuffer already released");
    APPCORE_ASSERT(!m_context, "No context");
    APPCORE_ASSERT(mask.size() % 4 == 0, "color_mask must be a multiple of 4");
    APPCORE_ASSERT(mask.size() / m_draw_buffers_len == 4, "color_mask must be a multiple of 4");
    const GLMethods& gl = m_context->gl();

    m_color_mask = mask;

    if(m_framebuffer_obj == m_context->m_bound_framebuffer->m_framebuffer_obj)
    {
      for(int i = 0; i < m_draw_buffers_len; ++i)
      {
        gl.ColorMaski(i, m_color_mask[i * 4 + 0], m_color_mask[i * 4 + 1], m_color_mask[i * 4 + 2], m_color_mask[i * 4 + 3]);
      }
    }
  }

  void Framebuffer::set_depth_mask(bool value)
  {
    APPCORE_ASSERT(!m_released, "Framebuffer already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    m_depth_mask = value;

    if(m_framebuffer_obj == m_context->m_bound_framebuffer->m_framebuffer_obj)
    {
      gl.DepthMask(m_depth_mask);
    }
  }

  void Framebuffer::bits(int& red_bits, int& green_bits, int& blue_bits, int& alpha_bits, int& depth_bits, int& stencil_bits)
  {
    APPCORE_ASSERT(!m_released, "Framebuffer already released");
    APPCORE_ASSERT(!m_context, "No context");
    APPCORE_ASSERT(!m_framebuffer_obj, "Only the default_framebuffer have bits");
    const GLMethods& gl = m_context->gl();

    gl.BindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_obj);
    gl.GetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, &red_bits);
    gl.GetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, &green_bits);
    gl.GetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, &blue_bits);
    gl.GetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &alpha_bits);
    gl.GetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &depth_bits);
    gl.GetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &stencil_bits);
    gl.BindFramebuffer(GL_FRAMEBUFFER, m_context->m_bound_framebuffer->m_framebuffer_obj);
  }

} // namespace AppGL
