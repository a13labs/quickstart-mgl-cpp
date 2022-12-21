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
#include "glm/glm.hpp"

namespace AppGL
{
  class Framebuffer : public AppCore::RefFromThis<Framebuffer>
  {
public:
    ~Framebuffer() { release(); }

    const Context* context() const;

    const Rect& viewport();
    void set_viewport(const Rect& r);

    const Rect& scissor();
    void set_scissor(const Rect& r);

    void enable_scissor();
    void disable_scissor();

    const ColorMasks& color_mask() const;
    void set_color_mask(const ColorMask& mask);
    void set_color_mask(const ColorMasks& masks);

    bool depth_mask();
    void set_depth_mask(bool value);

    int width();
    int height();

    void bits(int& red_bits, int& green_bits, int& blue_bits, int& alpha_bits, int& depth_bits, int& stencil_bits);

    void clear(const glm::vec4& color, float depth);
    void clear(float r, float g, float b, float a, float depth);
    void clear(const glm::vec4& color, float depth, int w, int h);
    void clear(const glm::vec4& color, float depth, const Rect& rect);
    void clear(float r, float g, float b, float a, float depth, int w, int h);
    void clear(float r, float g, float b, float a, float depth, const Rect& rect);

    void read(void* dst, int components, int attachment, int alignment, const char* dtype, size_t write_offset);
    void read(void* dst);
    void
    read(void* dst, const Rect& viewport, int components, int attachment, int alignment, const char* dtype, size_t write_offset);

    void read(AppCore::Ref<Buffer> dst, int components, int attachment, int alignment, const char* dtype, size_t write_offset);
    void read(AppCore::Ref<Buffer> dst);
    void read(AppCore::Ref<Buffer> dst,
              const Rect& viewport,
              int components,
              int attachment,
              int alignment,
              const char* dtype,
              size_t write_offset);

    void release();
    void use();

private:
    friend class Context;
    friend class Renderbuffer;
    friend class Texture2D;

    Framebuffer(){};

    Context* m_context;
    int m_framebuffer_obj;

    Rect m_viewport;
    bool m_scissor_enabled;
    Rect m_scissor;
    ColorMasks m_color_masks;

    int m_draw_buffers_len;
    unsigned* m_draw_buffers;

    // Flags this as a detected framebuffer we don't control the size of
    bool m_dynamic;
    int m_width;
    int m_height;

    int m_samples;
    bool m_depth_mask;
    bool m_released;
  };

  inline const Context* Framebuffer::context() const
  {
    return m_context;
  }

  inline const Rect& Framebuffer::viewport()
  {
    return m_viewport;
  }
  inline void Framebuffer::set_viewport(const Rect& r)
  {
    m_viewport = r;
  }

  inline const Rect& Framebuffer::scissor()
  {
    return m_scissor;
  }
  inline void Framebuffer::set_scissor(const Rect& r)
  {
    m_scissor = r;
  }

  inline void Framebuffer::enable_scissor()
  {
    m_scissor_enabled = true;
  }
  inline void Framebuffer::disable_scissor()
  {
    m_scissor_enabled = false;
  }

  inline void Framebuffer::clear(float r, float g, float b, float a, float depth)
  {
    clear(r, g, b, a, depth, {0, 0, m_width, m_height});
  }

  inline void Framebuffer::clear(float r, float g, float b, float a, float depth, int w, int h)
  {
    clear(r, g, b, a, depth, {0, 0, w, h});
  }

  inline void Framebuffer::clear(const glm::vec4& color, float depth)
  {
    clear(color.r, color.g, color.b, color.a, depth, {0, 0, m_width, m_height});
  }

  inline void Framebuffer::clear(const glm::vec4& color, float depth, int w, int h)
  {
    clear(color.r, color.g, color.b, color.a, depth, {0, 0, w, h});
  }

  inline void Framebuffer::clear(const glm::vec4& color, float depth, const Rect& rect)
  {
    clear(color.r, color.g, color.b, color.a, depth, rect);
  }

  inline void Framebuffer::read(void* dst, int components, int attachment, int alignment, const char* dtype, size_t write_offset)
  {
    read(dst, {0, 0, m_width, m_height}, components, attachment, alignment, dtype, write_offset);
  }

  inline void Framebuffer::read(
      AppCore::Ref<Buffer> dst, int components, int attachment, int alignment, const char* dtype, size_t write_offset)
  {
    read(dst, {0, 0, m_width, m_height}, components, attachment, alignment, dtype, write_offset);
  }

  inline void Framebuffer::read(void* dst)
  {
    read(dst, 3, 0, 1, "f1", 0);
  }

  inline void Framebuffer::read(AppCore::Ref<Buffer> dst)
  {
    read(dst, 3, 0, 1, "f1", 0);
  }

  inline const ColorMasks& Framebuffer::color_mask() const
  {
    return m_color_masks;
  }

  inline bool Framebuffer::depth_mask()
  {
    return m_depth_mask;
  }

  inline int Framebuffer::width()
  {
    return m_width;
  }

  inline int Framebuffer::height()
  {
    return m_height;
  }

  inline void Framebuffer::set_color_mask(const ColorMask& mask)
  {
    set_color_mask({mask});
  }

} // namespace AppGL