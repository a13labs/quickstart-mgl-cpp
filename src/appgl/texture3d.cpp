
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
#include "texture3d.hpp"
#include "appcore/log.hpp"
#include "buffer.hpp"
#include "context.hpp"
#include "datatype.hpp"

namespace AppGL
{
  void Texture3D::release() { }

  Texture::Type Texture3D::texture_type()
  {
    return Texture::TEXTURE_3D;
  }

  void Texture3D::read(void* dst, int alignment, size_t write_offset)
  {
    APPCORE_ASSERT(!m_released, "Texture3D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("the alignment must be 1, 2, 4 or 8");
      return;
    }
    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    char* ptr = (char*)dst + write_offset;

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);
    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.GetTexImage(GL_TEXTURE_3D, 0, base_format, pixel_type, ptr);
  }

  void Texture3D::read(AppCore::Ref<Buffer>& dst, int alignment, size_t write_offset)
  {
    APPCORE_ASSERT(!m_released, "Texture3D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("the alignment must be 1, 2, 4 or 8");
      return;
    }
    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    gl.BindBuffer(GL_PIXEL_PACK_BUFFER, dst->m_buffer_obj);
    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);
    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.GetTexImage(GL_TEXTURE_3D, 0, base_format, pixel_type, (void*)write_offset);
    gl.BindBuffer(GL_PIXEL_PACK_BUFFER, 0);
  }

  void Texture3D::write(const void* src, const Viewport3D& viewport, int alignment)
  {
    APPCORE_ASSERT(!m_released, "Texture3D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("the alignment must be 1, 2, 4 or 8");
      return;
    }

    int x = viewport.x;
    int y = viewport.y;
    int z = viewport.z;
    int width = viewport.width;
    int height = viewport.height;
    int depth = viewport.depth;
    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, base_format, pixel_type, src);
  }

  void Texture3D::write(const void* src, int alignment)
  {
    APPCORE_ASSERT(!m_released, "Texture3D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("the alignment must be 1, 2, 4 or 8");
      return;
    }

    int x = 0;
    int y = 0;
    int z = 0;
    int width = m_width;
    int height = m_height;
    int depth = m_depth;
    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, base_format, pixel_type, src);
  }

  void Texture3D::write(const AppCore::Ref<Buffer>& src, const Viewport3D& viewport, int alignment)
  {
    APPCORE_ASSERT(!m_released, "Texture3D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("the alignment must be 1, 2, 4 or 8");
      return;
    }

    int x = viewport.x;
    int y = viewport.y;
    int z = viewport.z;
    int width = viewport.width;
    int height = viewport.height;
    int depth = viewport.depth;
    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);
    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, base_format, pixel_type, 0);
    gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }

  void Texture3D::write(const AppCore::Ref<Buffer>& src, int alignment)
  {
    APPCORE_ASSERT(!m_released, "Texture3D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("the alignment must be 1, 2, 4 or 8");
      return;
    }

    int x = 0;
    int y = 0;
    int z = 0;
    int width = m_width;
    int height = m_height;
    int depth = m_depth;
    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);
    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, base_format, pixel_type, 0);
    gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  }

  void Texture3D::bind_to_image(int unit, bool read, bool write, int level, int format)
  {
    APPCORE_ASSERT(!m_released, "Texture3D already released");
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

    gl.BindImageTexture(unit, m_texture_obj, level, GL_TRUE, 0, access, frmt);
  }

  void Texture3D::use(int index)
  {
    APPCORE_ASSERT(!m_released, "Texture3D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    gl.ActiveTexture(GL_TEXTURE0 + index);
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);
  }

  void Texture3D::build_mipmaps(int base, int max_level)
  {
    APPCORE_ASSERT(!m_released, "Texture3D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(base > m_max_level)
    {
      APPCORE_ERROR("invalid base");
      return;
    }

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, base);
    gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, m_max_level);

    gl.GenerateMipmap(GL_TEXTURE_3D);

    gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_filter = {GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR};
    m_max_level = max_level;
  }

  void Texture3D::set_repeat_x(bool value)
  {
    APPCORE_ASSERT(!m_released, "Texture3D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    m_repeat_x = value;

    if(m_repeat_x)
    {
      gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      return;
    }

    gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  }

  void Texture3D::set_repeat_y(bool value)
  {
    APPCORE_ASSERT(!m_released, "Texture3D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    m_repeat_y = value;

    if(m_repeat_y)
    {
      gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      return;
    }

    gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  void Texture3D::set_repeat_z(bool value)
  {
    APPCORE_ASSERT(!m_released, "Texture3D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    m_repeat_z = value;

    if(m_repeat_z)
    {
      gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
      return;
    }

    gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  }

  void Texture3D::set_filter(const Texture3D::Filter& value)
  {
    APPCORE_ASSERT(!m_released, "Texture3D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    m_filter = value;

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);
    gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, m_filter.min_filter);
    gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, m_filter.mag_filter);
  }

  AppCore::String Texture3D::swizzle()
  {
    APPCORE_ASSERT(!m_released, "Texture3D already released");
    APPCORE_ASSERT(!m_context, "No context");
    const GLMethods& gl = m_context->gl();

    if(m_depth)
    {
      APPCORE_ERROR("cannot get swizzle of depth textures");
      return "";
    }

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    int swizzle_r = 0;
    int swizzle_g = 0;
    int swizzle_b = 0;
    int swizzle_a = 0;

    gl.GetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_R, &swizzle_r);
    gl.GetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_G, &swizzle_g);
    gl.GetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_B, &swizzle_b);
    gl.GetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_A, &swizzle_a);

    char swizzle[5] = {
        char_from_swizzle(swizzle_r),
        char_from_swizzle(swizzle_g),
        char_from_swizzle(swizzle_b),
        char_from_swizzle(swizzle_a),
        0,
    };

    return swizzle;
  }

  void Texture3D::set_swizzle(const AppCore::String& value)
  {
    APPCORE_ASSERT(!m_released, "Texture3D already released");
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

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_R, tex_swizzle[0]);
    if(tex_swizzle[1] != -1)
    {
      gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_G, tex_swizzle[1]);
      if(tex_swizzle[2] != -1)
      {
        gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_B, tex_swizzle[2]);
        if(tex_swizzle[3] != -1)
        {
          gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_SWIZZLE_A, tex_swizzle[3]);
        }
      }
    }
  }
} // namespace AppGL
