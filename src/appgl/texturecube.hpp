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

namespace mgl
{
  class TextureCube : public Texture
  {
public:
    ~TextureCube() = default;
    virtual Texture::Type texture_type() override;

    void release();
    bool released();

    int width();
    int height();
    int components();

    const Texture::Filter& filter() const;
    void set_filter(const Texture::Filter& value);

    mgl_core::String swizzle();
    void set_swizzle(const mgl_core::String& value);

    float anisotropy();
    void set_anisotropy(float value);

    bool read_into(mgl_core::MemoryBuffer<uint8_t>& dst, int face, int alignment = 1, size_t write_offset = 0);
    bool read_into(mgl_core::Ref<Buffer>& dst, int face, int alignment = 1, size_t write_offset = 0);

    bool write(const mgl_core::MemoryBuffer<uint8_t>& src, int face, const Viewport2D& viewport, int alignment = 1);
    bool write(const mgl_core::MemoryBuffer<uint8_t>& src, int face, int alignment = 1);
    bool write(const mgl_core::Ref<Buffer>& src, int face, const Viewport2D& viewport, int alignment = 1);
    bool write(const mgl_core::Ref<Buffer>& src, int face, int alignment = 1);

    void bind_to_image(int unit, bool read = true, bool write = true, int level = 0, int format = 0);
    void use(int index = 0);

private:
    friend class Context;
    TextureCube() = default;

    Context* m_context;
    DataType* m_data_type;
    int m_texture_obj;
    int m_width;
    int m_height;
    int m_components;
    Texture::Filter m_filter;
    int m_max_level;
    float m_anisotropy;
    bool m_released;
  };

  inline bool TextureCube::released()
  {
    return m_released;
  }

  inline int TextureCube::components()
  {
    return m_components;
  }

  inline int TextureCube::width()
  {
    return m_width;
  }

  inline int TextureCube::height()
  {
    return m_height;
  }

  inline const Texture::Filter& TextureCube::filter() const
  {
    return m_filter;
  }

  inline float TextureCube::anisotropy()
  {
    return m_anisotropy;
  }
} // namespace mgl