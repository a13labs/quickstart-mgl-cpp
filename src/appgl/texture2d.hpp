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
#include "attachment.hpp"
#include "texture.hpp"

namespace AppGL
{
  class Texture2D : public Attachment, public Texture
  {
public:
    enum Func
    {
      NONE = 0x0000,
      NEVER = 0x0200,
      LESS = 0x0201,
      EQUAL = 0x0202,
      LESS_EQUAL = 0x0203,
      GREATER = 0x0204,
      NOT_EQUAL = 0x0205,
      GREATER_EQUAL = 0x0206,
      ALWAYS = 0x0207,
    };

    ~Texture2D() { release(); }

    void release();
    bool released();

    virtual Attachment::Type attachment_type() override;
    virtual Texture::Type texture_type() override;
    virtual int width() override;
    virtual int height() override;
    virtual int samples() override;
    virtual bool depth() override;

    bool repeat_x();
    void set_repeat_x(bool value);

    bool repeat_y();
    void set_repeat_y(bool value);

    const Texture2D::Filter& filter() const;
    void set_filter(const Texture2D::Filter& value);

    AppCore::String swizzle();
    void set_swizzle(const AppCore::String& value);

    Texture2D::Func compare_func();
    void set_compare_func(Texture2D::Func value);

    float anisotropy();
    void set_anisotropy(float value);

    void read(void* dst, int level, int alignment, size_t write_offset);
    void read(AppCore::Ref<Buffer>& dst, int level, int alignment, size_t write_offset);
    void write(const void* src, const Rect& viewport, int level, int alignment);
    void write(const void* src, int level, int alignment);
    void write(const AppCore::Ref<Buffer>& src, const Rect& viewport, int level, int alignment);
    void write(const AppCore::Ref<Buffer>& src, int level, int alignment);
    void bind_to_image(int unit, bool read = true, bool write = true, int level = 0, int format = 0);
    void use(int index = 0);
    void build_mipmaps(int base = 0, int max_level = 1000);

private:
    friend class Context;
    Texture2D() = default;

    virtual void color_attachment(Framebuffer* fb, int index) override;
    virtual void depth_attachment() override;
    virtual const Context* context() const override;

    Context* m_context;
    DataType* m_data_type;
    int m_texture_obj;
    int m_width;
    int m_height;
    bool m_depth;
    int m_samples;
    int m_components;
    Texture::Filter m_filter;
    int m_max_level;
    Texture2D::Func m_compare_func;
    float m_anisotropy;
    bool m_repeat_x;
    bool m_repeat_y;
    bool m_released;
  };

  inline bool Texture2D::released()
  {
    return m_released;
  }

  inline bool Texture2D::repeat_x()
  {
    return m_repeat_x;
  }

  inline bool Texture2D::repeat_y()
  {
    return m_repeat_y;
  }

  inline const Texture::Filter& Texture2D::filter() const
  {
    return m_filter;
  }

  inline Texture2D::Func Texture2D::compare_func()
  {
    return m_compare_func;
  }

  inline float Texture2D::anisotropy()
  {
    return m_anisotropy;
  }

} // namespace AppGL