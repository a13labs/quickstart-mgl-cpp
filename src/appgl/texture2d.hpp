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
#include "attachment.hpp"
#include "builtins.hpp"
#include "texture.hpp"

namespace mgl
{
  class Texture2D : public Attachment, public Texture
  {
public:
    ~Texture2D() = default;

    void release();
    bool released();

    virtual Attachment::type attachment_type() override;
    virtual Texture::type texture_type() override;
    int width();
    int height();
    int samples();
    bool depth();
    int components();

    bool repeat_x();
    void set_repeat_x(bool value);

    bool repeat_y();
    void set_repeat_y(bool value);

    const Texture::filter& filter() const;
    void set_filter(const Texture::filter& value);

    mgl_core::string swizzle();
    void set_swizzle(const mgl_core::string& value);

    mgl::compare_func compare_func();
    void set_compare_func(mgl::compare_func value);

    float anisotropy();
    void set_anisotropy(float value);

    bool read_into(mgl_core::mem_buffer<uint8_t>& dst, int level = 0, int alignment = 1, size_t write_offset = 0);
    bool read_into(mgl_core::ref<Buffer>& dst, int level = 0, int alignment = 1, size_t write_offset = 0);

    bool write(const mgl_core::mem_buffer<uint8_t>& src, const viewport_2d& viewport, int level = 0, int alignment = 1);
    bool write(const mgl_core::mem_buffer<uint8_t>& src, int level = 0, int alignment = 1);
    bool write(const mgl_core::ref<Buffer>& src, const viewport_2d& viewport, int level = 0, int alignment = 1);
    bool write(const mgl_core::ref<Buffer>& src, int level = 0, int alignment = 1);

    void bind_to_image(int unit, bool read_into = true, bool write = true, int level = 0, int format = 0);
    void use(int index = 0);
    void build_mipmaps(int base = 0, int max_level = 1000);

private:
    friend class Context;
    Texture2D() = default;

    Context* m_context;
    data_type* m_data_type;
    int m_texture_obj;
    int m_width;
    int m_height;
    bool m_depth;
    int m_samples;
    int m_components;
    Texture::filter m_filter;
    int m_max_level;
    mgl::compare_func m_compare_func;
    float m_anisotropy;
    bool m_repeat_x;
    bool m_repeat_y;
    bool m_released;
  };

  inline bool Texture2D::released()
  {
    return m_released;
  }

  inline int Texture2D::components()
  {
    return m_components;
  }

  inline int Texture2D::width()
  {
    return m_width;
  }

  inline int Texture2D::height()
  {
    return m_height;
  }

  inline int Texture2D::samples()
  {
    return m_samples;
  }

  inline bool Texture2D::depth()
  {
    return m_depth;
  }

  inline bool Texture2D::repeat_x()
  {
    return m_repeat_x;
  }

  inline bool Texture2D::repeat_y()
  {
    return m_repeat_y;
  }

  inline const Texture::filter& Texture2D::filter() const
  {
    return m_filter;
  }

  inline mgl::compare_func Texture2D::compare_func()
  {
    return m_compare_func;
  }

  inline float Texture2D::anisotropy()
  {
    return m_anisotropy;
  }

} // namespace mgl