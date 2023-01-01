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
#pragma once
#include "builtins.hpp"
#include "texture.hpp"

namespace mgl_opengl
{
  class texture_cube : public texture
  {
public:
    ~texture_cube() = default;
    virtual texture::type texture_type() override;

    void release();
    bool released();

    int width();
    int height();
    int components();

    const texture::filter& filter() const;
    void set_filter(const texture::filter& value);

    mgl_core::string swizzle();
    void set_swizzle(const mgl_core::string& value);

    float anisotropy();
    void set_anisotropy(float value);

    bool read_into(mgl_core::mem_buffer<uint8_t>& dst, int face, int alignment = 1, size_t write_offset = 0);
    bool read_into(buffer_ref& dst, int face, int alignment = 1, size_t write_offset = 0);

    bool write(const mgl_core::mem_buffer<uint8_t>& src, int face, const mgl_core::viewport_2d& viewport, int alignment = 1);
    bool write(const mgl_core::mem_buffer<uint8_t>& src, int face, int alignment = 1);
    bool write(const buffer_ref& src, int face, const mgl_core::viewport_2d& viewport, int alignment = 1);
    bool write(const buffer_ref& src, int face, int alignment = 1);

    void bind_to_image(int unit, bool read = true, bool write = true, int level = 0, int format = 0);
    void use(int index = 0);

    int glo();

private:
    friend class context;
    texture_cube() = default;

    context* m_context;
    data_type* m_data_type;
    int m_texture_obj;
    int m_width;
    int m_height;
    int m_components;
    texture::filter m_filter;
    int m_max_level;
    float m_anisotropy;
    bool m_released;
  };

  inline int texture_cube::glo()
  {
    return m_texture_obj;
  }

  inline bool texture_cube::released()
  {
    return m_released;
  }

  inline int texture_cube::components()
  {
    return m_components;
  }

  inline int texture_cube::width()
  {
    return m_width;
  }

  inline int texture_cube::height()
  {
    return m_height;
  }

  inline const texture::filter& texture_cube::filter() const
  {
    return m_filter;
  }

  inline float texture_cube::anisotropy()
  {
    return m_anisotropy;
  }
} // namespace mgl_opengl