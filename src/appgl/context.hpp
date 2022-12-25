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
#include "glmethods.hpp"

namespace AppGL
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

  class Context : public AppCore::RefFromThis<Context>
  {
public:
    enum Flags
    {
      NOTHING = 0,
      BLEND = 1,
      DEPTH_TEST = 2,
      CULL_FACE = 4,
      RASTERIZER_DISCARD = 8,
      PROGRAM_POINT_SIZE = 16,
      INVALID = 0x40000000
    };

    virtual ~Context() = default;

    const GLMethods& gl() const;
    int version_code();
    int max_samples();
    int max_integer_samples();
    int max_color_attachments();
    int max_texture_units();
    int default_texture_unit();
    float max_anisotropy();
    const AppCore::StringList& extensions() const;
    const AppCore::Ref<Framebuffer>& framebuffer() const;

    int enable_flags();
    void set_enable_flags(int flags);

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

    // Buffer
    AppCore::Ref<Buffer> buffer(AppCore::MemoryBuffer<float>& dst, bool dynamic = false);
    AppCore::Ref<Buffer> buffer(AppCore::MemoryBuffer<uint32_t>& dst, bool dynamic = false);
    AppCore::Ref<Buffer> buffer(AppCore::MemoryBuffer<uint16_t>& dst, bool dynamic = false);
    AppCore::Ref<Buffer> buffer(AppCore::MemoryBuffer<uint8_t>& dst, bool dynamic = false);

    // Compute Shader
    AppCore::Ref<ComputeShader> compute_shader(const AppCore::String& source);

    // Create Shader
    static AppCore::Ref<Context> create_context(ContextMode::Enum mode, int required);

    // Framebuffer
    AppCore::Ref<Framebuffer> framebuffer(const AttachmentsRef& color_attachments, AppCore::Ref<Attachment> depth_attachment);
    AppCore::Ref<Framebuffer> framebuffer(const AttachmentsRef& color_attachments);
    AppCore::Ref<Framebuffer> framebuffer(AppCore::Ref<Attachment> depth_attachment);

    // Program
    AppCore::Ref<Program> program(const ShadersSources& shaders,
                                  const ShadersOutputs& outputs = {},
                                  const FragmentOutputs& fragment_outputs = {},
                                  bool interleaved = true);
    AppCore::Ref<Program> program(const ShadersSources& shaders, const ShadersOutputs& outputs, bool interleaved = true);
    AppCore::Ref<Program> program(const ShadersSources& shaders, bool interleaved = true);

    // Query
    AppCore::Ref<Query>
    query(bool samples = false, bool any_samples = false, bool time_elapsed = false, bool primitives_generated = false);

    // Renderbuffer
    AppCore::Ref<Renderbuffer> renderbuffer(int width, int height, int components = 4, int samples = 0, const char* dtype = "f1");
    AppCore::Ref<Renderbuffer> depth_renderbuffer(int width, int height, int samples = 0);

    // Sampler
    AppCore::Ref<Sampler> sampler();

    // Scope
    AppCore::Ref<Scope> scope(AppCore::Ref<Framebuffer> framebuffer = nullptr,
                              int enable_flags = 0,
                              const TextureBindings& textures = {},
                              const BufferBindings& uniform_buffers = {},
                              const BufferBindings& storage_buffers = {},
                              const SamplerBindings& samplers = {});

    // Texture
    AppCore::Ref<Texture2D> texture2d(int width,
                                      int height,
                                      int components,
                                      const void* data = nullptr,
                                      int samples = 0,
                                      int alignment = 1,
                                      const char* dtype = "f1",
                                      int internal_format_override = 0);

    AppCore::Ref<Texture2D>
    depth_texture2d(int width, int height, const void* data = nullptr, int samples = 0, int alignment = 0);

    // Texture3D
    AppCore::Ref<Texture3D> texture3d(int width,
                                      int height,
                                      int depth,
                                      int components,
                                      const void* data = nullptr,
                                      int alignment = 1,
                                      const char* dtype = "f1");

    // TextureArray
    AppCore::Ref<TextureArray> texture_array(int width,
                                             int height,
                                             int layers,
                                             int components,
                                             const void* data = nullptr,
                                             int alignment = 1,
                                             const char* dtype = "f1");

    // TextureCube
    AppCore::Ref<TextureCube> texture_cube(int width,
                                           int height,
                                           int components,
                                           const void* data = nullptr,
                                           int alignment = 1,
                                           const char* dtype = "f1",
                                           int internal_format_override = 0);

    virtual GLFunction load(const AppCore::String& method) = 0;
    virtual void enter() = 0;
    virtual void exit() = 0;

    virtual void release() = 0;
    virtual bool released() = 0;

    virtual bool is_valid() = 0;
    virtual ContextMode::Enum mode() = 0;

private:
    void load_functions();
    AppCore::Ref<Buffer> buffer(void* data, size_t size, bool dynamic);

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
    ContextEGL(ContextMode::Enum mode, int required);
    virtual ~ContextEGL() override;

public:
    virtual GLFunction load(const AppCore::String& method) override;
    virtual void enter() override;
    virtual void exit() override;
    virtual void release() override;
    virtual bool is_valid() override;
    virtual ContextMode::Enum mode() override { return m_mode; }
    virtual bool released() override { return m_released; }

private:
    ContextMode::Enum m_mode;
    GLContext m_context;
    bool m_released;
  };
#endif

#ifdef APPGL_GLX
  class ContextGLX : public Context
  {

public:
    ContextGLX(ContextMode::Enum mode, int required);
    ContextGLX(){};
    virtual ~ContextGLX() override;

public:
    virtual GLFunction load(const AppCore::String& method) override;
    virtual void enter() override;
    virtual void exit() override;
    virtual void release() override;
    virtual bool is_valid() override;
    virtual ContextMode::Enum mode() override { return m_mode; }
    virtual bool released() override { return m_released; }

private:
    ContextMode::Enum m_mode;
    GLContext m_context;
    bool m_released;
  };
#endif

#ifdef APPGL_WGL

#endif

  inline const GLMethods& Context::gl() const
  {
    return m_gl;
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

  inline const AppCore::StringList& Context::extensions() const
  {
    return m_extensions;
  }

  inline const AppCore::Ref<Framebuffer>& Context::framebuffer() const
  {
    return m_default_framebuffer;
  }

  inline int Context::enable_flags()
  {
    return m_enable_flags;
  }

  inline void Context::set_enable_flags(int flags)
  {
    m_enable_flags = flags;
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

  inline AppCore::Ref<Buffer> Context::buffer(AppCore::MemoryBuffer<float>& dst, bool dynamic)
  {
    return buffer(dst.data(), dst.size_bytes(), dynamic);
  }

  inline AppCore::Ref<Buffer> Context::buffer(AppCore::MemoryBuffer<uint32_t>& dst, bool dynamic)
  {
    return buffer(dst.data(), dst.size_bytes(), dynamic);
  }

  inline AppCore::Ref<Buffer> Context::buffer(AppCore::MemoryBuffer<uint16_t>& dst, bool dynamic)
  {
    return buffer(dst.data(), dst.size_bytes(), dynamic);
  }

  inline AppCore::Ref<Buffer> Context::buffer(AppCore::MemoryBuffer<uint8_t>& dst, bool dynamic)
  {
    return buffer(dst.data(), dst.size_bytes(), dynamic);
  }

  inline AppCore::Ref<Framebuffer> Context::framebuffer(AppCore::Ref<Attachment> depth_attachment)
  {
    return framebuffer(AttachmentsRef(), depth_attachment);
  }

  inline AppCore::Ref<Framebuffer> Context::framebuffer(const AttachmentsRef& color_attachments)
  {
    return framebuffer(color_attachments, AppCore::Ref<Attachment>(nullptr));
  }

  inline AppCore::Ref<Program> Context::program(const ShadersSources& shaders, const ShadersOutputs& outputs, bool interleaved)
  {
    return program(shaders, outputs, FragmentOutputs(), interleaved);
  }

  inline AppCore::Ref<Program> Context::program(const ShadersSources& shaders, bool interleaved)
  {
    return program(shaders, ShadersOutputs(), FragmentOutputs(), interleaved);
  }

} // namespace AppGL
