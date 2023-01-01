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

namespace mgl::opengl
{
  class texture_array
  {
public:
    ~texture_array() = default;

    void release();
    bool released();

    int width();
    int height();
    int layers();
    int components();

    bool repeat_x();
    void set_repeat_x(bool value);

    bool repeat_y();
    void set_repeat_y(bool value);

    const texture::filter& filter() const;
    void set_filter(const texture::filter& value);

    mgl::core::string swizzle();
    void set_swizzle(const mgl::core::string& value);

    float anisotropy();
    void set_anisotropy(float value);

    bool read_into(mgl::core::mem_buffer<uint8_t>& dst, int alignment = 1, size_t write_offset = 0);
    bool read_into(buffer_ref& dst, int alignment = 1, size_t write_offset = 0);

    bool write(const mgl::core::mem_buffer<uint8_t>& src, const mgl::core::viewport_3d& viewport, int alignment = 1);
    bool write(const mgl::core::mem_buffer<uint8_t>& src, int alignment = 1);
    bool write(const buffer_ref& src, const mgl::core::viewport_3d& viewport, int alignment = 1);
    bool write(const buffer_ref& src, int alignment = 1);

    void bind_to_image(int unit, bool read = true, bool write = true, int level = 0, int format = 0);
    void use(int index = 0);
    void build_mipmaps(int base = 0, int max_level = 1000);

    int glo();

private:
    friend class context;
    texture_array() = default;

    context* m_context;
    data_type* m_data_type;
    int m_texture_obj;
    int m_width;
    int m_height;
    int m_layers;
    int m_components;
    texture::filter m_filter;
    int m_max_level;
    bool m_repeat_x;
    bool m_repeat_y;
    float m_anisotropy;
    bool m_released;
  };

  inline int texture_array::glo()
  {
    return m_texture_obj;
  }

  inline bool texture_array::released()
  {
    return m_released;
  }

  inline int texture_array::components()
  {
    return m_components;
  }

  inline int texture_array::width()
  {
    return m_width;
  }

  inline int texture_array::height()
  {
    return m_height;
  }

  inline int texture_array::layers()
  {
    return m_layers;
  }

  inline bool texture_array::repeat_x()
  {
    return m_repeat_x;
  }

  inline bool texture_array::repeat_y()
  {
    return m_repeat_y;
  }

  inline const texture::filter& texture_array::filter() const
  {
    return m_filter;
  }

  inline float texture_array::anisotropy()
  {
    return m_anisotropy;
  }

} // namespace  mgl::opengl