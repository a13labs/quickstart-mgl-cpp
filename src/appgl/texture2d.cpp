
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
#include "buffer.hpp"
#include "context.hpp"
#include "datatype.hpp"
#include "framebuffer.hpp"

namespace AppGL
{
  void Texture2D::release()
  {
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(m_released)
    {
      return;
    }

    m_released = true;

    gl.DeleteTextures(1, (GLuint*)&m_texture_obj);
  }

  void Texture2D::color_attachment(Framebuffer* fb, int index)
  {
    APPCORE_ASSERT(!m_released, "Texture2D already released");
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
    APPCORE_ASSERT(!m_released, "Texture2D already released");
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

  void Texture2D::read(void* dst, int level, int alignment, size_t write_offset)
  {
    APPCORE_ASSERT(!m_released, "Texture2D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("the alignment must be 1, 2, 4 or 8");
      return;
    }

    if(level > m_max_level)
    {
      APPCORE_ERROR("invalid level");
      return;
    }

    if(m_samples)
    {
      APPCORE_ERROR("multisample textures cannot be read directly");
      return;
    }

    int width = m_width / (1 << level);
    int height = m_height / (1 << level);

    width = width > 1 ? width : 1;
    height = height > 1 ? height : 1;

    int pixel_type = m_data_type->gl_type;
    int base_format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    char* ptr = (char*)dst + write_offset;

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_2D, m_texture_obj);
    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.GetTexImage(GL_TEXTURE_2D, level, base_format, pixel_type, ptr);
  }

  void Texture2D::read(AppCore::Ref<Buffer>& dst, int level, int alignment, size_t write_offset)
  {
    APPCORE_ASSERT(!m_released, "Texture2D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("the alignment must be 1, 2, 4 or 8");
      return;
    }

    if(level > m_max_level)
    {
      APPCORE_ERROR("invalid level");
      return;
    }

    if(m_samples)
    {
      APPCORE_ERROR("multisample textures cannot be read directly");
      return;
    }

    int width = m_width / (1 << level);
    int height = m_height / (1 << level);

    width = width > 1 ? width : 1;
    height = height > 1 ? height : 1;

    int pixel_type = m_data_type->gl_type;
    int base_format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    gl.BindBuffer(GL_PIXEL_PACK_BUFFER, dst->m_buffer_obj);
    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_2D, m_texture_obj);
    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.GetTexImage(GL_TEXTURE_2D, level, base_format, pixel_type, (void*)write_offset);
    gl.BindBuffer(GL_PIXEL_PACK_BUFFER, 0);
  }

  void Texture2D::write(const void* src, const Rect& viewport, int level, int alignment)
  {
    APPCORE_ASSERT(!m_released, "Texture2D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("the alignment must be 1, 2, 4 or 8");
      return;
    }

    if(level > m_max_level)
    {
      APPCORE_ERROR("invalid level");
      return;
    }

    if(m_samples)
    {
      APPCORE_ERROR("multisample textures cannot be written directly");
      return;
    }

    int x = viewport.X;
    int y = viewport.Y;
    int width = viewport.W;
    int height = viewport.H;

    int pixel_type = m_data_type->gl_type;
    int format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_2D, m_texture_obj);
    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexSubImage2D(GL_TEXTURE_2D, level, x, y, width, height, format, pixel_type, src);
  }

  void Texture2D::write(const void* src, int level, int alignment)
  {
    APPCORE_ASSERT(!m_released, "Texture2D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("the alignment must be 1, 2, 4 or 8");
      return;
    }

    if(level > m_max_level)
    {
      APPCORE_ERROR("invalid level");
      return;
    }

    if(m_samples)
    {
      APPCORE_ERROR("multisample textures cannot be written directly");
      return;
    }

    int x = 0;
    int y = 0;
    int width = m_width / (1 << level);
    int height = m_height / (1 << level);

    width = width > 1 ? width : 1;
    height = height > 1 ? height : 1;

    int pixel_type = m_data_type->gl_type;
    int format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_2D, m_texture_obj);
    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexSubImage2D(GL_TEXTURE_2D, level, x, y, width, height, format, pixel_type, src);
  }

  void Texture2D::write(const AppCore::Ref<Buffer>& src, const Rect& viewport, int level, int alignment)
  {
    APPCORE_ASSERT(!m_released, "Texture2D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("the alignment must be 1, 2, 4 or 8");
      return;
    }

    if(level > m_max_level)
    {
      APPCORE_ERROR("invalid level");
      return;
    }

    if(m_samples)
    {
      APPCORE_ERROR("multisample textures cannot be written directly");
      return;
    }

    int x = viewport.X;
    int y = viewport.Y;
    int width = viewport.W;
    int height = viewport.H;

    int pixel_type = m_data_type->gl_type;
    int format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, src->m_buffer_obj);
    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_2D, m_texture_obj);
    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexSubImage2D(GL_TEXTURE_2D, level, x, y, width, height, format, pixel_type, 0);
    gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }

  void Texture2D::write(const AppCore::Ref<Buffer>& src, int level, int alignment)
  {
    APPCORE_ASSERT(!m_released, "Texture2D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("the alignment must be 1, 2, 4 or 8");
      return;
    }

    if(level > m_max_level)
    {
      APPCORE_ERROR("invalid level");
      return;
    }

    if(m_samples)
    {
      APPCORE_ERROR("multisample textures cannot be written directly");
      return;
    }

    int x = 0;
    int y = 0;
    int width = m_width / (1 << level);
    int height = m_height / (1 << level);

    width = width > 1 ? width : 1;
    height = height > 1 ? height : 1;

    int pixel_type = m_data_type->gl_type;
    int format = m_depth ? GL_DEPTH_COMPONENT : m_data_type->base_format[m_components];

    gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, src->m_buffer_obj);
    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_2D, m_texture_obj);
    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexSubImage2D(GL_TEXTURE_2D, level, x, y, width, height, format, pixel_type, 0);
    gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }

  void Texture2D::bind_to_image(int unit, bool read, bool write, int level, int format)
  {
    APPCORE_ASSERT(!m_released, "Texture2D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    int access = GL_READ_WRITE;
    if(read && !write)
      access = GL_READ_ONLY;
    else if(!read && write)
      access = GL_WRITE_ONLY;
    else if(!read && !write)
    {
      APPCORE_ERROR("Illegal access mode. Read or write needs to be enabled.");
      return;
    }

    int frmt = format ? format : m_data_type->internal_format[m_components];

    gl.BindImageTexture(unit, m_texture_obj, level, 0, 0, access, frmt);
  }

  void Texture2D::use(int index)
  {
    APPCORE_ASSERT(!m_released, "Texture2D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    gl.ActiveTexture(GL_TEXTURE0 + index);
    gl.BindTexture(texture_target, m_texture_obj);
  }

  void Texture2D::build_mipmaps(int base, int max_level)
  {
    APPCORE_ASSERT(!m_released, "Texture2D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(base > m_max_level)
    {
      APPCORE_ERROR("invalid base");
      return;
    }

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(texture_target, m_texture_obj);

    gl.TexParameteri(texture_target, GL_TEXTURE_BASE_LEVEL, base);
    gl.TexParameteri(texture_target, GL_TEXTURE_MAX_LEVEL, max_level);

    gl.GenerateMipmap(texture_target);

    gl.TexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gl.TexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_filter = {GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR};
    m_max_level = max_level;
  }

  void Texture2D::set_repeat_x(bool value)
  {
    APPCORE_ASSERT(!m_released, "Texture2D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(texture_target, m_texture_obj);

    m_repeat_x = value;

    if(m_repeat_x)
    {
      gl.TexParameteri(texture_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
      return;
    }

    gl.TexParameteri(texture_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  }

  void Texture2D::set_repeat_y(bool value)
  {
    APPCORE_ASSERT(!m_released, "Texture2D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(texture_target, m_texture_obj);

    m_repeat_y = value;

    if(m_repeat_y)
    {
      gl.TexParameteri(texture_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
      return;
    }

    gl.TexParameteri(texture_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  void Texture2D::set_filter(const Texture2D::Filter& value)
  {
    APPCORE_ASSERT(!m_released, "Texture2D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    m_filter = value;

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(texture_target, m_texture_obj);
    gl.TexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, m_filter.min_filter);
    gl.TexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, m_filter.mag_filter);
  }

  AppCore::String Texture2D::swizzle()
  {
    APPCORE_ASSERT(!m_released, "Texture2D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(m_depth)
    {
      APPCORE_ERROR("cannot get swizzle of depth textures");
      return "";
    }

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(texture_target, m_texture_obj);

    int swizzle_r = 0;
    int swizzle_g = 0;
    int swizzle_b = 0;
    int swizzle_a = 0;

    gl.GetTexParameteriv(texture_target, GL_TEXTURE_SWIZZLE_R, &swizzle_r);
    gl.GetTexParameteriv(texture_target, GL_TEXTURE_SWIZZLE_G, &swizzle_g);
    gl.GetTexParameteriv(texture_target, GL_TEXTURE_SWIZZLE_B, &swizzle_b);
    gl.GetTexParameteriv(texture_target, GL_TEXTURE_SWIZZLE_A, &swizzle_a);

    char swizzle[5] = {
        char_from_swizzle(swizzle_r),
        char_from_swizzle(swizzle_g),
        char_from_swizzle(swizzle_b),
        char_from_swizzle(swizzle_a),
        0,
    };

    return swizzle;
  }

  void Texture2D::set_swizzle(const AppCore::String& value)
  {
    APPCORE_ASSERT(!m_released, "Texture2D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();
    const char* swizzle = value.c_str();

    if(m_depth)
    {
      APPCORE_ERROR("cannot set swizzle for depth textures");
      return;
    }

    if(!swizzle[0])
    {
      APPCORE_ERROR("the swizzle is empty");
      return;
    }

    int tex_swizzle[4] = {-1, -1, -1, -1};

    for(int i = 0; swizzle[i]; ++i)
    {
      if(i > 3)
      {
        APPCORE_ERROR("the swizzle is too long");
        return;
      }

      tex_swizzle[i] = swizzle_from_char(swizzle[i]);

      if(tex_swizzle[i] == -1)
      {
        APPCORE_ERROR("'%c' is not a valid swizzle parameter", swizzle[i]);
        return;
      }
    }

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(texture_target, m_texture_obj);

    gl.TexParameteri(texture_target, GL_TEXTURE_SWIZZLE_R, tex_swizzle[0]);
    if(tex_swizzle[1] != -1)
    {
      gl.TexParameteri(texture_target, GL_TEXTURE_SWIZZLE_G, tex_swizzle[1]);
      if(tex_swizzle[2] != -1)
      {
        gl.TexParameteri(texture_target, GL_TEXTURE_SWIZZLE_B, tex_swizzle[2]);
        if(tex_swizzle[3] != -1)
        {
          gl.TexParameteri(texture_target, GL_TEXTURE_SWIZZLE_A, tex_swizzle[3]);
        }
      }
    }
  }

  void Texture2D::set_compare_func(Texture2D::Func value)
  {
    APPCORE_ASSERT(!m_released, "Texture2D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(!m_depth)
    {
      APPCORE_ERROR("only depth textures have compare_func");
      return;
    }

    m_compare_func = value;

    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(texture_target, m_texture_obj);

    if(m_compare_func == 0)
    {
      gl.TexParameteri(texture_target, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    }
    else
    {
      gl.TexParameteri(texture_target, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
      gl.TexParameteri(texture_target, GL_TEXTURE_COMPARE_FUNC, m_compare_func);
    }
  }

  void Texture2D::set_anisotropy(float value)
  {

    APPCORE_ASSERT(!m_released, "Texture2D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    m_anisotropy = (float)APPGL_MIN(APPGL_MAX(value, 1.0), m_context->max_anisotropy());
    int texture_target = m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(texture_target, m_texture_obj);
    gl.TexParameterf(texture_target, GL_TEXTURE_MAX_ANISOTROPY, m_anisotropy);
  }

} // namespace AppGL
