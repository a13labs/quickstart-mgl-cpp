
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
#include "framebuffer.hpp"
#include "appcore/log.hpp"
#include "context.hpp"

namespace AppGL
{
  void FrameBuffer::release()
  {
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
      delete[] m_color_mask;
    }
  }

  void FrameBuffer::clear(float r, float g, float b, float a, float depth, const Rect& rect)
  {
    if(m_released)
      return;

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
      gl.ColorMaski(i,
                    m_color_mask[i * 4 + 0],
                    m_color_mask[i * 4 + 1],
                    m_color_mask[i * 4 + 2],
                    m_color_mask[i * 4 + 3]);
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

  void FrameBuffer::use()
  {
    if(m_released)
      return;

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
      gl.ColorMaski(i,
                    m_color_mask[i * 4 + 0],
                    m_color_mask[i * 4 + 1],
                    m_color_mask[i * 4 + 2],
                    m_color_mask[i * 4 + 3]);
    }

    gl.DepthMask(m_depth_mask);

    m_context->m_bound_framebuffer = MakeThisRef();
  }

  AppCore::Ref<uint8_t> FrameBuffer::read(const Rect& v,
                                          int c,
                                          int att,
                                          int align,
                                          int clmp,
                                          const char* dtype,
                                          size_t dsize,
                                          int w_offset)
  {
    return nullptr;
  }

  void FrameBuffer::read_into(AppCore::Ref<uint8_t> buffer,
                              const Rect& v,
                              int c,
                              int att,
                              int align,
                              int clmp,
                              const char* dtype,
                              size_t dsize,
                              int w_offset)
  { }
} // namespace AppGL
