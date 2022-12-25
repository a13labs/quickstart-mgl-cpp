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
  class TextureArray
  {
public:
    ~TextureArray() { release(); }

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

    const Texture::Filter& filter() const;
    void set_filter(const Texture::Filter& value);

    AppCore::String swizzle();
    void set_swizzle(const AppCore::String& value);

    float anisotropy();
    void set_anisotropy(float value);

    bool read_into(AppCore::MemoryBuffer<uint8_t>& dst, int alignment = 1, size_t write_offset = 0);
    bool read_into(AppCore::Ref<Buffer>& dst, int alignment = 1, size_t write_offset = 0);

    bool write(const AppCore::MemoryBuffer<uint8_t>& src, const Viewport3D& viewport, int alignment = 1);
    bool write(const AppCore::MemoryBuffer<uint8_t>& src, int alignment = 1);
    bool write(const AppCore::Ref<Buffer>& src, const Viewport3D& viewport, int alignment = 1);
    bool write(const AppCore::Ref<Buffer>& src, int alignment = 1);

    void bind_to_image(int unit, bool read = true, bool write = true, int level = 0, int format = 0);
    void use(int index = 0);
    void build_mipmaps(int base = 0, int max_level = 1000);

private:
    friend class Context;
    TextureArray() = default;

    Context* m_context;
    DataType* m_data_type;
    int m_texture_obj;
    int m_width;
    int m_height;
    int m_layers;
    int m_components;
    Texture::Filter m_filter;
    int m_max_level;
    bool m_repeat_x;
    bool m_repeat_y;
    float m_anisotropy;
    bool m_released;
  };

  inline bool TextureArray::released()
  {
    return m_released;
  }

  inline int TextureArray::components()
  {
    return m_components;
  }

  inline int TextureArray::width()
  {
    return m_width;
  }

  inline int TextureArray::height()
  {
    return m_height;
  }

  inline int TextureArray::layers()
  {
    return m_layers;
  }

  inline bool TextureArray::repeat_x()
  {
    return m_repeat_x;
  }

  inline bool TextureArray::repeat_y()
  {
    return m_repeat_y;
  }

  inline const Texture::Filter& TextureArray::filter() const
  {
    return m_filter;
  }

  inline float TextureArray::anisotropy()
  {
    return m_anisotropy;
  }

} // namespace AppGL