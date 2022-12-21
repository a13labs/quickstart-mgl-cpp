
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
#include "texture2d.hpp"
#include "appcore/log.hpp"
#include "context.hpp"
#include "framebuffer.hpp"

namespace AppGL
{
  void Texture2D::release() { }

  void Texture2D::color_attachment(Framebuffer* fb, int index)
  {
    APPCORE_ASSERT(!m_released, "Texture already released");
    APPCORE_ASSERT(fb != nullptr, "missing color attachments");
    APPCORE_ASSERT(index < fb->m_draw_buffers_len, "missing color attachments");
    APPCORE_ASSERT(fb->m_context == m_context, "Attachment and framebuffer belong to different contexts");
    const GLMethods& gl = m_context->gl();

    gl.FramebufferTexture2D(
        GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_texture_obj, 0);

    fb->m_draw_buffers[index] = GL_COLOR_ATTACHMENT0 + index;
    fb->m_color_masks[index] = {m_components >= 1, m_components >= 2, m_components >= 3, m_components >= 4};
  }

  void Texture2D::depth_attachment()
  {
    APPCORE_ASSERT(!m_released, "Texture already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    gl.FramebufferTexture2D(
        GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_texture_obj, 0);
  }

  Attachment::Type Texture2D::attachment_type()
  {
    return Attachment::TEXTURE;
  }

  Texture::Type Texture2D::texture_type()
  {
    return Texture::TEXTURE_2D;
  }

  int Texture2D::width()
  {
    return m_width;
  }

  int Texture2D::height()
  {
    return m_height;
  }

  int Texture2D::samples()
  {
    return m_samples;
  }

  bool Texture2D::depth()
  {
    return m_depth;
  }

  const Context* Texture2D::context() const
  {
    return m_context;
  }
} // namespace AppGL
