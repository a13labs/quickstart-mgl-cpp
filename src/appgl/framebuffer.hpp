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
  class FrameBuffer : public AppCore::RefFromThis<FrameBuffer>
  {
public:
    ~FrameBuffer() { release(); }

    void clear(float r, float g, float b, float a, float depth, const Rect& rect);
    void use();
    AppCore::Ref<uint8_t> read(const Rect& v,
                               int c,
                               int att,
                               int align,
                               int clmp,
                               const char* dtype,
                               size_t dsize,
                               int w_offset);
    void read_into(AppCore::Ref<uint8_t> buffer,
                   const Rect& v,
                   int c,
                   int att,
                   int align,
                   int clmp,
                   const char* dtype,
                   size_t dsize,
                   int w_offset);
    void release();

    inline const Rect& viewport() { return m_viewport; }
    inline void set_viewport(const Rect& r) { m_viewport = r; }

    inline const Rect& scissor() { return m_scissor; }
    inline void set_scissor(const Rect& r) { m_scissor = r; }

    inline void enable_scissor() { m_scissor_enabled = true; }
    inline void disable_scissor() { m_scissor_enabled = false; }

    inline void clear(float r, float g, float b, float a, float depth)
    {
      clear(r, g, b, a, depth, {0, 0, m_width, m_height});
    }

    inline void clear(float r, float g, float b, float a, float depth, int w, int h)
    {
      clear(r, g, b, a, depth, {0, 0, w, h});
    }

    inline void clear(const glm::vec4& color, float depth)
    {
      clear(color.r, color.g, color.b, color.a, depth, {0, 0, m_width, m_height});
    }

    inline void clear(const glm::vec4& color, float depth, int w, int h)
    {
      clear(color.r, color.g, color.b, color.a, depth, {0, 0, w, h});
    }

    inline void clear(const glm::vec4& color, float depth, const Rect& rect)
    {
      clear(color.r, color.g, color.b, color.a, depth, rect);
    }

private:
    friend class Context;
    FrameBuffer(){};

    Context* m_context;
    Rect m_viewport;
    bool m_scissor_enabled;
    Rect m_scissor;
    bool* m_color_mask;
    int m_draw_buffers_len;
    unsigned* m_draw_buffers;
    int m_framebuffer_obj;

    // Flags this as a detected framebuffer we don't control the size of
    bool m_dynamic;
    int m_width;
    int m_height;

    int m_samples;
    bool m_depth_mask;
    bool m_released;
  };
} // namespace AppGL