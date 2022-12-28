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
#include "framebuffer.hpp"
#include "glm/glm.hpp"
#include "glmethods.hpp"

namespace mgl_opengl
{
  struct ContextMode
  {
    enum Enum
    {
      DETECT,
      SHARE,
      STANDALONE
    };
  };

  class Context
  {
public:
    virtual ~Context() = default;

    const GLMethods& gl() const;
    int version_code();
    int max_samples();
    int max_integer_samples();
    int max_color_attachments();
    int max_texture_units();
    int default_texture_unit();
    float max_anisotropy();
    const mgl_core::string_list& extensions() const;
    const mgl_core::ref<Framebuffer>& screen() const;

    int enable_flags();
    void set_enable_flags(int flags);
    void enable(int flags);
    void disable(int flags);
    void enable_direct(int value);
    void disable_direct(int value);
    void finish();

    void clear_samplers(int start = 0, int end = -1);

    int front_face();
    int cull_face();
    int depth_func();
    int blend_func_src();
    int blend_func_dst();
    bool wireframe();
    bool multisample();
    int provoking_vertex();
    float polygon_offset_factor();
    float polygon_offset_units();
    void copy_buffer(
        const mgl_core::ref<Buffer>& src, const mgl_core::ref<Buffer>& dst, size_t size, size_t read_offset, size_t write_offset);

    // Buffer
    mgl_core::ref<Buffer> buffer(const mgl_core::mem_buffer<float>& data, bool dynamic = false);
    mgl_core::ref<Buffer> buffer(const mgl_core::mem_buffer<uint32_t>& data, bool dynamic = false);
    mgl_core::ref<Buffer> buffer(const mgl_core::mem_buffer<uint16_t>& data, bool dynamic = false);
    mgl_core::ref<Buffer> buffer(const mgl_core::mem_buffer<uint8_t>& data, bool dynamic = false);

    // Compute Shader
    mgl_core::ref<ComputeShader> compute_shader(const mgl_core::string& source);

    // Create Shader
    static mgl_core::ref<Context> create_context(ContextMode::Enum mode, int required);

    // Framebuffer
    mgl_core::ref<Framebuffer> framebuffer(const attachments_ref& color_attachments, mgl_core::ref<Attachment> depth_attachment);
    mgl_core::ref<Framebuffer> framebuffer(const attachments_ref& color_attachments);
    mgl_core::ref<Framebuffer> framebuffer(mgl_core::ref<Attachment> depth_attachment);

    // Program
    mgl_core::ref<Program> program(const shaders_sources& shaders,
                                   const shaders_outputs& outputs = {},
                                   const fragment_outputs& fragment_outputs = {},
                                   bool interleaved = true);

    // Query
    mgl_core::ref<Query>
    query(bool samples = false, bool any_samples = false, bool time_elapsed = false, bool primitives_generated = false);

    // Renderbuffer
    mgl_core::ref<Renderbuffer>
    renderbuffer(int width, int height, int components = 4, int samples = 0, const char* dtype = "f1");
    mgl_core::ref<Renderbuffer> depth_renderbuffer(int width, int height, int samples = 0);

    // Sampler
    mgl_core::ref<Sampler> sampler();

    // Scope
    mgl_core::ref<Scope> scope(mgl_core::ref<Framebuffer> framebuffer = nullptr,
                               int enable_flags = 0,
                               const texture_bindings& textures = {},
                               const buffer_bindings& uniform_buffers = {},
                               const buffer_bindings& storage_buffers = {},
                               const sampler_bindings& samplers = {});

    // Texture
    mgl_core::ref<Texture2D> texture2d(int width,
                                       int height,
                                       int components,
                                       const void* data = nullptr,
                                       int samples = 0,
                                       int alignment = 1,
                                       const char* dtype = "f1",
                                       int internal_format_override = 0);

    mgl_core::ref<Texture2D>
    depth_texture2d(int width, int height, const void* data = nullptr, int samples = 0, int alignment = 0);

    // Texture3D
    mgl_core::ref<Texture3D> texture3d(int width,
                                       int height,
                                       int depth,
                                       int components,
                                       const void* data = nullptr,
                                       int alignment = 1,
                                       const char* dtype = "f1");

    // TextureArray
    mgl_core::ref<TextureArray> texture_array(int width,
                                              int height,
                                              int layers,
                                              int components,
                                              const void* data = nullptr,
                                              int alignment = 1,
                                              const char* dtype = "f1");

    // TextureCube
    mgl_core::ref<TextureCube> texture_cube(int width,
                                            int height,
                                            int components,
                                            const void* data = nullptr,
                                            int alignment = 1,
                                            const char* dtype = "f1",
                                            int internal_format_override = 0);

    // VertexArray
    mgl_core::ref<VertexArray> vertex_array(mgl_core::ref<Program> program,
                                            mgl_opengl::vertex_data_list vertex_data,
                                            mgl_core::ref<Buffer> index_buffer = nullptr,
                                            int index_element_size = 4,
                                            bool skip_errors = false,
                                            mgl_opengl::render_mode mode = mgl_opengl::render_mode::POINTS);

    virtual gl_function load(const mgl_core::string& method) = 0;
    virtual void enter() = 0;
    virtual void exit() = 0;

    virtual void release() = 0;
    virtual bool is_valid() = 0;

    bool released();
    ContextMode::Enum mode();
    void clear(const glm::vec4& color, float depth = 0.0, const mgl_core::viewport_2d& viewport = mgl_core::null_viewport_2d);
    void clear(float r,
               float g,
               float b,
               float a = 0.0,
               float depth = 0.0,
               const mgl_core::viewport_2d& viewport = mgl_core::null_viewport_2d);

private:
    void load_functions();
    mgl_core::ref<Buffer> buffer(void* data, size_t size, bool dynamic);

protected:
    bool m_released;
    ContextMode::Enum m_mode;

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
    mgl_core::string_list m_extensions;
    mgl_core::ref<Framebuffer> m_default_framebuffer;
    mgl_core::ref<Framebuffer> m_bound_framebuffer;
  };

#ifdef MGL_OPENGL_EGL
  class ContextEGL : public Context
  {

public:
    ContextEGL(ContextMode::Enum mode, int required);
    virtual ~ContextEGL() override;

    virtual gl_function load(const mgl_core::string& method) override;
    virtual void enter() override;
    virtual void exit() override;

    virtual void release() override;

    virtual bool is_valid() override;

private:
    gl_context m_context;
  };
#endif

#ifdef MGL_OPENGL_GLX
  class ContextGLX : public Context
  {

public:
    ContextGLX(ContextMode::Enum mode, int required);
    ContextGLX(){};
    virtual ~ContextGLX() override;

public:
    virtual gl_function load(const mgl_core::string& method) override;

    virtual void enter() override;
    virtual void exit() override;

    virtual void release() override;

    virtual bool is_valid() override;

private:
    ContextMode::Enum m_mode;
    gl_context m_context;
  };
#endif

#ifdef MGL_OPENGL_WGL

#endif

  inline const GLMethods& Context::gl() const
  {
    return m_gl;
  }

  inline ContextMode::Enum Context::mode()
  {
    return m_mode;
  }

  inline bool Context::released()
  {
    return m_released;
  }

  inline int Context::version_code()
  {
    return m_version_code;
  }

  inline int Context::max_samples()
  {
    return m_max_samples;
  }

  inline int Context::max_integer_samples()
  {
    return m_max_integer_samples;
  }

  inline int Context::max_color_attachments()
  {
    return m_max_color_attachments;
  }

  inline int Context::max_texture_units()
  {
    return m_max_texture_units;
  }

  inline int Context::default_texture_unit()
  {
    return m_default_texture_unit;
  }

  inline float Context::max_anisotropy()
  {
    return m_max_anisotropy;
  }

  inline const mgl_core::string_list& Context::extensions() const
  {
    return m_extensions;
  }

  inline const mgl_core::ref<Framebuffer>& Context::screen() const
  {
    return m_default_framebuffer;
  }

  inline int Context::enable_flags()
  {
    return m_enable_flags;
  }

  inline int Context::front_face()
  {
    return m_front_face;
  }

  inline int Context::cull_face()
  {
    return m_cull_face;
  }

  inline int Context::depth_func()
  {
    return m_depth_func;
  }

  inline int Context::blend_func_src()
  {
    return m_blend_func_src;
  }

  inline int Context::blend_func_dst()
  {
    return m_blend_func_dst;
  }

  inline bool Context::wireframe()
  {
    return m_wireframe;
  }

  inline bool Context::multisample()
  {
    return m_multisample;
  }

  inline int Context::provoking_vertex()
  {
    return m_provoking_vertex;
  }

  inline float Context::polygon_offset_factor()
  {
    return m_polygon_offset_factor;
  }

  inline float Context::polygon_offset_units()
  {
    return m_polygon_offset_units;
  }

  inline mgl_core::ref<Buffer> Context::buffer(const mgl_core::mem_buffer<float>& data, bool dynamic)
  {
    return buffer(data.data(), data.size_bytes(), dynamic);
  }

  inline mgl_core::ref<Buffer> Context::buffer(const mgl_core::mem_buffer<uint32_t>& data, bool dynamic)
  {
    return buffer(data.data(), data.size_bytes(), dynamic);
  }

  inline mgl_core::ref<Buffer> Context::buffer(const mgl_core::mem_buffer<uint16_t>& data, bool dynamic)
  {
    return buffer(data.data(), data.size_bytes(), dynamic);
  }

  inline mgl_core::ref<Buffer> Context::buffer(const mgl_core::mem_buffer<uint8_t>& data, bool dynamic)
  {
    return buffer(data.data(), data.size_bytes(), dynamic);
  }

  inline mgl_core::ref<Framebuffer> Context::framebuffer(mgl_core::ref<Attachment> depth_attachment)
  {
    return framebuffer(attachments_ref(), depth_attachment);
  }

  inline mgl_core::ref<Framebuffer> Context::framebuffer(const attachments_ref& color_attachments)
  {
    return framebuffer(color_attachments, mgl_core::ref<Attachment>(nullptr));
  }
} // namespace mgl_opengl
