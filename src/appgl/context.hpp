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
#include "glmethods.hpp"

namespace AppGL
{
  struct ContextMode
  {
    enum Enum
    {
      Detect,
      Share,
      Standalone
    };
  };

  class Context : public AppCore::RefFromThis<Context>
  {
public:
    virtual ~Context() = default;

public:
    virtual GLFunction load(const AppCore::String& method) = 0;
    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void release() = 0;
    virtual bool is_valid() = 0;

    inline ContextMode::Enum mode() { return m_mode; }
    inline const GLMethods& gl() const { return m_gl; }
    inline bool released() { return m_released; }
    inline int version_code() { return m_version_code; }
    inline int max_samples() { return m_max_samples; }
    inline int max_integer_samples() { return m_max_integer_samples; }
    inline int max_color_attachments() { return m_max_color_attachments; }
    inline int max_texture_units() { return m_max_texture_units; }
    inline int default_texture_unit() { return m_default_texture_unit; }
    inline float max_anisotropy() { return m_max_anisotropy; }
    inline const AppCore::StringList& extensions() const { return m_extensions; }

    inline const AppCore::Ref<Framebuffer> framebuffer() const { return m_default_framebuffer; }

    inline int enable_flags() { return m_enable_flags; }
    inline int front_face() { return m_front_face; }
    inline int cull_face() { return m_cull_face; }
    inline int depth_func() { return m_depth_func; }
    inline int blend_func_src() { return m_blend_func_src; }
    inline int blend_func_dst() { return m_blend_func_dst; }
    inline bool wireframe() { return m_wireframe; }
    inline bool multisample() { return m_multisample; }
    inline int provoking_vertex() { return m_provoking_vertex; }
    inline float polygon_offset_factor() { return m_polygon_offset_factor; }
    inline float polygon_offset_units() { return m_polygon_offset_units; }

    static AppCore::Ref<Context> create_context(ContextMode::Enum mode, int glversion);

    AppCore::Ref<Buffer> buffer(const uint8_t* data, size_t length, bool dynamic);

    AppCore::Ref<Framebuffer> framebuffer(const AppCore::List<Texture> color_attachments,
                                          const Texture& depth_attachment);
    AppCore::Ref<Framebuffer> framebuffer(const AppCore::List<Texture> color_attachments,
                                          const Renderbuffer& depth_attachment);
    AppCore::Ref<Framebuffer> framebuffer(const AppCore::List<Renderbuffer> color_attachments,
                                          const Texture& depth_attachment);
    AppCore::Ref<Framebuffer> framebuffer(const AppCore::List<Renderbuffer> color_attachments,
                                          const Renderbuffer& depth_attachment);

    AppCore::Ref<Renderbuffer> renderbuffer(uint32_t w,
                                            uint32_t h,
                                            uint8_t components,
                                            uint8_t samples,
                                            const char* dtype,
                                            size_t dtype_size);

private:
    void load_functions();

protected:
    ContextMode::Enum m_mode;
    bool m_released;

private:
    friend class Framebuffer;

    GLMethods m_gl;
    int m_version_code;
    int m_max_samples;
    int m_max_integer_samples;
    int m_max_color_attachments;
    int m_max_texture_units;
    int m_default_texture_unit;
    float m_max_anisotropy;
    int m_enable_flags;
    int m_front_face;
    int m_cull_face;
    int m_depth_func;
    int m_blend_func_src;
    int m_blend_func_dst;
    bool m_wireframe;
    bool m_multisample;
    int m_provoking_vertex;
    float m_polygon_offset_factor;
    float m_polygon_offset_units;
    AppCore::StringList m_extensions;
    AppCore::Ref<Framebuffer> m_default_framebuffer;
    AppCore::Ref<Framebuffer> m_bound_framebuffer;
  };

#ifdef APPGL_EGL
  class ContextEGL : public Context
  {

public:
    ContextEGL(ContextMode::Enum mode, int glversion);
    virtual ~ContextEGL() override;

public:
    virtual GLFunction load(const AppCore::String& method) override;
    virtual void enter() override;
    virtual void exit() override;
    virtual void release() override;
    virtual bool is_valid() override;

private:
    GLContext m_context;
  };
#endif

#ifdef APPGL_GLX
  class ContextGLX : public Context
  {

public:
    ContextGLX(ContextMode::Enum mode, int glversion);
    ContextGLX(){};
    virtual ~ContextGLX() override;

public:
    virtual GLFunction load(const AppCore::String& method) override;
    virtual void enter() override;
    virtual void exit() override;
    virtual void release() override;
    virtual bool is_valid() override;

private:
    GLContext m_context;
  };
#endif

#ifdef APPGL_WGL

#endif

} // namespace AppGL
