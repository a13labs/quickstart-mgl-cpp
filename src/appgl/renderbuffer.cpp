
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
#include "renderbuffer.hpp"
#include "appcore/log.hpp"
#include "context.hpp"
#include "framebuffer.hpp"

namespace AppGL
{
  void Renderbuffer::release()
  {
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();
    if(m_released)
    {
      return;
    }

    m_released = true;

    gl.DeleteRenderbuffers(1, (GLuint*)&m_renderbuffer_obj);
  }

  void Renderbuffer::color_attachment(Framebuffer* fb, int index)
  {
    APPCORE_ASSERT(!m_released, "Context already released");
    APPCORE_ASSERT(fb != nullptr, "missing color attachments");
    APPCORE_ASSERT(index < fb->m_draw_buffers_len, "missing color attachments");
    APPCORE_ASSERT(fb->m_context == m_context, "Attachment and framebuffer belong to different contexts");
    const GLMethods& gl = m_context->gl();

    gl.FramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER, m_renderbuffer_obj);

    fb->m_draw_buffers[index] = GL_COLOR_ATTACHMENT0 + index;
    fb->m_color_mask[index * 4 + 0] = m_components >= 1;
    fb->m_color_mask[index * 4 + 1] = m_components >= 2;
    fb->m_color_mask[index * 4 + 2] = m_components >= 3;
    fb->m_color_mask[index * 4 + 3] = m_components >= 4;
  }

  void Renderbuffer::depth_attachment()
  {
    APPCORE_ASSERT(!m_released, "Context already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    gl.FramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer_obj);
  }

  Attachment::Type Renderbuffer::attachment_type()
  {
    return Attachment::Renderbuffer;
  }

  int Renderbuffer::width()
  {
    return m_width;
  }

  int Renderbuffer::height()
  {
    return m_height;
  }

  int Renderbuffer::samples()
  {
    return m_samples;
  }

  bool Renderbuffer::depth()
  {
    return m_depth;
  }

  const Context* Renderbuffer::context() const
  {
    return m_context;
  }
} // namespace AppGL
