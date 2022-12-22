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
#include "appgl.hpp"
#include "texture.hpp"

namespace AppGL
{
  class Texture3D : public Texture
  {
public:
    ~Texture3D() { release(); }

    void release();
    bool released();

    virtual Texture::Type texture_type() override;
    int width();
    int height();
    int depth();

    bool repeat_x();
    void set_repeat_x(bool value);

    bool repeat_y();
    void set_repeat_y(bool value);

    bool repeat_z();
    void set_repeat_z(bool value);

    const Texture::Filter& filter() const;
    void set_filter(const Texture::Filter& value);

    AppCore::String swizzle();
    void set_swizzle(const AppCore::String& value);

    void read(void* dst, int alignment, size_t write_offset);
    void read(AppCore::Ref<Buffer>& dst, int alignment, size_t write_offset);
    void write(const void* src, const Viewport3D& viewport, int alignment);
    void write(const void* src, int alignment);
    void write(const AppCore::Ref<Buffer>& src, const Viewport3D& viewport, int alignment);
    void write(const AppCore::Ref<Buffer>& src, int alignment);
    void bind_to_image(int unit, bool read = true, bool write = true, int level = 0, int format = 0);
    void use(int index = 0);
    void build_mipmaps(int base = 0, int max_level = 1000);

private:
    friend class Context;
    Texture3D() = default;

    Context* m_context;
    DataType* m_data_type;
    int m_texture_obj;
    int m_width;
    int m_height;
    int m_depth;
    int m_components;
    Texture::Filter m_filter;
    int m_max_level;
    bool m_repeat_x;
    bool m_repeat_y;
    bool m_repeat_z;
    bool m_released;
  };

  inline bool Texture3D::released()
  {
    return m_released;
  }

  inline int Texture3D::width()
  {
    return m_width;
  }

  inline int Texture3D::height()
  {
    return m_height;
  }

  inline int Texture3D::depth()
  {
    return m_depth;
  }

  inline bool Texture3D::repeat_x()
  {
    return m_repeat_x;
  }

  inline bool Texture3D::repeat_y()
  {
    return m_repeat_y;
  }

  inline const Texture::Filter& Texture3D::filter() const
  {
    return m_filter;
  }

} // namespace AppGL