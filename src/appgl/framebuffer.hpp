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
#include "glm/glm.hpp"

namespace mgl
{
  class Framebuffer : public mgl_core::ref_from_this<Framebuffer>
  {
public:
    ~Framebuffer() = default;

    void release();
    bool released();

    const Viewport2D& viewport();
    void set_viewport(const Viewport2D& r);

    const Viewport2D& scissor();
    void set_scissor(const Viewport2D& r);

    void enable_scissor();
    void disable_scissor();

    const ColorMasks& color_mask() const;
    void set_color_mask(const ColorMask& mask);
    void set_color_mask(const ColorMasks& masks);

    bool depth_mask();
    void set_depth_mask(bool value);

    int width();
    int height();

    bool bits(int& red_bits, int& green_bits, int& blue_bits, int& alpha_bits, int& depth_bits, int& stencil_bits);

    void clear(const glm::vec4& color, float depth = 0.0, const Viewport2D& viewport = NullViewport2D);
    void clear(float r, float g, float b, float a = 0.0, float depth = 0.0, const Viewport2D& viewport = NullViewport2D);

    bool read_into(mgl_core::mem_buffer<u_int8_t>& dst,
                   const Viewport2D& viewport = NullViewport2D,
                   int components = 3,
                   int attachment = 0,
                   int alignment = 1,
                   const char* dtype = "f1",
                   size_t write_offset = 0);

    bool read_into(mgl_core::ref<Buffer> dst,
                   const Viewport2D& viewport = NullViewport2D,
                   int components = 3,
                   int attachment = 0,
                   int alignment = 1,
                   const char* dtype = "f1",
                   size_t write_offset = 0);

    void use();

private:
    friend class Context;
    friend class Renderbuffer;
    friend class Texture2D;

    Framebuffer() = default;

    Context* m_context;
    int m_framebuffer_obj;

    Viewport2D m_viewport;
    bool m_scissor_enabled;
    Viewport2D m_scissor;
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

  inline bool Framebuffer::released()
  {
    return m_released;
  }

  inline const Viewport2D& Framebuffer::viewport()
  {
    return m_viewport;
  }
  inline void Framebuffer::set_viewport(const Viewport2D& r)
  {
    m_viewport = r;
  }

  inline const Viewport2D& Framebuffer::scissor()
  {
    return m_scissor;
  }
  inline void Framebuffer::set_scissor(const Viewport2D& r)
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

  inline void Framebuffer::clear(const glm::vec4& color, float depth, const Viewport2D& viewport)
  {
    clear(color.r, color.g, color.b, color.a, depth, viewport);
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
    set_color_mask({ mask });
  }

} // namespace mgl