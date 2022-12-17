/*
   Copyright 2020 Alexandre Pires (c.alexandre.pires@gmail.com)

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

    void clear(float r, float g, float b, float a, float depth);
    void clear(float r, float g, float b, float a, float depth, int w, int h);
    void clear(const glm::vec4& color, float depth);
    void clear(const glm::vec4& color, float depth, int w, int h);
    void clear(const glm::vec4& color, float depth, const Rect& rect);
    void clear(float r, float g, float b, float a, float depth, const Rect& rect);
    void read(void* dst, int c, int att, int align, int clmp, const char* dtype, size_t dsize, int w_offset);
    void read(void* dst, const Rect& v, int c, int att, int align, int clmp, const char* dtype, size_t dsize, int w_offset);

    void release();
    void use();

private:
    friend class Context;
    Framebuffer(){};

    Context* m_context;
    int m_framebuffer_obj;

    Rect m_viewport;
    bool m_scissor_enabled;
    Rect m_scissor;
    bool* m_color_mask;

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

  inline void Framebuffer::read(void* dst, int c, int att, int align, int clmp, const char* dtype, size_t dsize, int w_offset)
  {
    read(dst, {0, 0, m_width, m_height}, c, att, align, clmp, dtype, dsize, w_offset);
  }

} // namespace AppGL