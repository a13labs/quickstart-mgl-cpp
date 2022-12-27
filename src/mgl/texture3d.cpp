
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
#include "buffer.hpp"
#include "context.hpp"
#include "datatype.hpp"
#include "mgl_core/log.hpp"

namespace mgl
{
  void Texture3D::release()
  {
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    if(m_released)
    {
      return;
    }

    m_released = true;
    gl.DeleteTextures(1, (GLuint*)&m_texture_obj);
  }

  Texture::type Texture3D::texture_type()
  {
    return Texture::TEXTURE_3D;
  }

  bool Texture3D::read_into(mgl_core::mem_buffer<uint8_t>& dst, int alignment, size_t write_offset)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8, "alignment must be 1, 2, 4 or 8");
    const GLMethods& gl = m_context->gl();

    size_t expected_size = m_width * m_components * m_data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * m_height * m_depth;
    MGL_CORE_ASSERT(dst.size_bytes() >= write_offset + expected_size, "out of bounds");

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    char* ptr = (char*)dst.data() + write_offset;

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.GetTexImage(GL_TEXTURE_3D, 0, base_format, pixel_type, ptr);

    return gl.GetError() == GL_NO_ERROR;
  }

  bool Texture3D::read_into(mgl_core::ref<Buffer>& dst, int alignment, size_t write_offset)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8, "alignment must be 1, 2, 4 or 8");
    const GLMethods& gl = m_context->gl();

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    gl.BindBuffer(GL_PIXEL_PACK_BUFFER, dst->m_buffer_obj);
    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.GetTexImage(GL_TEXTURE_3D, 0, base_format, pixel_type, (void*)write_offset);
    gl.BindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    return gl.GetError() == GL_NO_ERROR;
  }

  bool Texture3D::write(const mgl_core::mem_buffer<uint8_t>& src, const mgl_core::viewport_3d& viewport, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8, "alignment must be 1, 2, 4 or 8");
    const GLMethods& gl = m_context->gl();

    int x = viewport.x;
    int y = viewport.y;
    int z = viewport.z;
    int width = viewport.width;
    int height = viewport.height;
    int depth = viewport.depth;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * height * depth;
    MGL_CORE_ASSERT(src.size_bytes() >= expected_size, "out of bounds");

    int base_format = m_data_type->base_format[m_components];
    int pixel_type = m_data_type->gl_type;

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, base_format, pixel_type, src.data());

    return gl.GetError() == GL_NO_ERROR;
  }

  bool Texture3D::write(const mgl_core::mem_buffer<uint8_t>& src, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8, "alignment must be 1, 2, 4 or 8");
    const GLMethods& gl = m_context->gl();

    int x = 0;
    int y = 0;
    int z = 0;
    int width = m_width;
    int height = m_height;
    int depth = m_depth;

    size_t expected_size = width * m_components * m_data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * height * depth;
    MGL_CORE_ASSERT(src.size_bytes() >= expected_size, "out of bounds");

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, base_format, pixel_type, src.data());

    return gl.GetError() == GL_NO_ERROR;
  }

  bool Texture3D::write(const mgl_core::ref<Buffer>& src, const mgl_core::viewport_3d& viewport, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8, "alignment must be 1, 2, 4 or 8");
    const GLMethods& gl = m_context->gl();

    int x = viewport.x;
    int y = viewport.y;
    int z = viewport.z;
    int width = viewport.width;
    int height = viewport.height;
    int depth = viewport.depth;

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, src->m_buffer_obj);
    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, base_format, pixel_type, 0);
    gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    return gl.GetError() == GL_NO_ERROR;
  }

  bool Texture3D::write(const mgl_core::ref<Buffer>& src, int alignment)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8, "alignment must be 1, 2, 4 or 8");
    const GLMethods& gl = m_context->gl();

    int x = 0;
    int y = 0;
    int z = 0;
    int width = m_width;
    int height = m_height;
    int depth = m_depth;

    int pixel_type = m_data_type->gl_type;
    int base_format = m_data_type->base_format[m_components];

    gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, src->m_buffer_obj);
    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexSubImage3D(GL_TEXTURE_3D, 0, x, y, z, width, height, depth, base_format, pixel_type, 0);
    gl.BindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

    return gl.GetError() == GL_NO_ERROR;
  }

  void Texture3D::bind_to_image(int unit, bool read, bool write, int level, int format)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(read || write, "Illegal access mode. Read or write needs to be enabled.");
    const GLMethods& gl = m_context->gl();

    int access = GL_READ_WRITE;
    if(read && !write)
      access = GL_READ_ONLY;
    else if(!read && write)
      access = GL_WRITE_ONLY;

    int frmt = format ? format : m_data_type->internal_format[m_components];

    gl.BindImageTexture(unit, m_texture_obj, level, GL_TRUE, 0, access, frmt);
  }

  void Texture3D::use(int index)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    gl.ActiveTexture(GL_TEXTURE0 + index);
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);
  }

  void Texture3D::build_mipmaps(int base, int max_level)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(base <= max_level, "invalid base");
    const GLMethods& gl = m_context->gl();

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, base);
    gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, m_max_level);

    gl.GenerateMipmap(GL_TEXTURE_3D);

    gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_filter = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR };
    m_max_level = max_level;
  }

  void Texture3D::set_repeat_x(bool value)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
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
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
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
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
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

  void Texture3D::set_filter(const Texture::filter& value)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    m_filter = value;

    gl.ActiveTexture(GL_TEXTURE0 + m_context->default_texture_unit());
    gl.BindTexture(GL_TEXTURE_3D, m_texture_obj);

    gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, m_filter.min_filter);
    gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, m_filter.mag_filter);
  }

  mgl_core::string Texture3D::swizzle()
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

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
      char_from_swizzle(swizzle_r), char_from_swizzle(swizzle_g), char_from_swizzle(swizzle_b), char_from_swizzle(swizzle_a), 0,
    };

    return swizzle;
  }

  void Texture3D::set_swizzle(const mgl_core::string& value)
  {
    MGL_CORE_ASSERT(!m_released, "Texture3D already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    const char* swizzle = value.c_str();
    MGL_CORE_ASSERT(swizzle[0], "the swizzle is empty");

    int tex_swizzle[4] = { -1, -1, -1, -1 };

    for(int i = 0; swizzle[i]; ++i)
    {
      MGL_CORE_ASSERT(i < 4, "the swizzle is too long");
      tex_swizzle[i] = swizzle_from_char(swizzle[i]);
      MGL_CORE_ASSERT(tex_swizzle[i] != -1, "'{0}' is not a valid swizzle parameter", swizzle[i]);
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
} // namespace mgl
