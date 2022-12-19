
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
#include "context.hpp"
#include "appcore/log.hpp"
#include "buffer.hpp"
#include "computeshader.hpp"
#include "datatype.hpp"
#include "framebuffer.hpp"
#include "renderbuffer.hpp"
#include "texture.hpp"
#include "uniform.hpp"

namespace AppGL
{
  inline void framebuffer_error_message(int status)
  {
    const char* message = "the framebuffer is not complete";

    switch(status)
    {
      case GL_FRAMEBUFFER_UNDEFINED: message = "the framebuffer is not complete (UNDEFINED)"; break;

      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: message = "the framebuffer is not complete (INCOMPLETE_ATTACHMENT)"; break;

      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        message = "the framebuffer is not complete (INCOMPLETE_MISSING_ATTACHMENT)";
        break;

      case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: message = "the framebuffer is not complete (INCOMPLETE_DRAW_BUFFER)"; break;

      case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: message = "the framebuffer is not complete (INCOMPLETE_READ_BUFFER)"; break;

      case GL_FRAMEBUFFER_UNSUPPORTED: message = "the framebuffer is not complete (UNSUPPORTED)"; break;

      case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: message = "the framebuffer is not complete (INCOMPLETE_MULTISAMPLE)"; break;

      case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: message = "the framebuffer is not complete (INCOMPLETE_LAYER_TARGETS)"; break;
    }

    APPCORE_ERROR(message);
  }

  void Context::load_functions()
  {
    // Map OpenGL functions
    void** gl_function = (void**)&m_gl;
    for(int i = 0; GL_FUNCTIONS[i]; ++i)
    {
      APPCORE_INFO("Loading GL function: {0}", GL_FUNCTIONS[i]);
      auto func = load(GL_FUNCTIONS[i]);
      APPCORE_ASSERT(func != nullptr, "Loading GL function {0}", GL_FUNCTIONS[i]);
      gl_function[i] = func;
    }
  }

  AppCore::Ref<Context> Context::create_context(ContextMode::Enum mode, int required)
  {

    Context* ctx = nullptr;

#ifdef APPGL_EGL
    APPCORE_INFO("Trying EGL context!");
    ctx = new ContextEGL(mode, required);
    if(!ctx->is_valid())
    {
      APPCORE_INFO("EGL not supported!");
      delete(ctx);
      ctx = nullptr;
    }
    else
    {
      APPCORE_INFO("EGL supported!");
    }
#endif
#ifdef APPGL_GLX
    if(!ctx)
    {
      APPCORE_INFO("Trying GLX context!");
      ctx = new ContextGLX(mode, required);
      if(!ctx->is_valid())
      {
        APPCORE_INFO("GLX not supported!");
        delete(ctx);
        ctx = nullptr;
      }
    }
    else
    {
      APPCORE_INFO("GLX supported!");
    }
#endif
#ifdef APPGL_WGL
    if(!ctx)
    {
      APPCORE_INFO("Trying WGL context!");
      ctx = new ContextWGL(mode, required);
      if(!ctx->is_valid())
      {
        APPCORE_INFO("WGL not supported!");
        delete(ctx);
        ctx = nullptr;
      }
    }
    else
    {
      APPCORE_INFO("WGL supported!");
    }
#endif

    if(!ctx)
    {
      APPCORE_ERROR("Error creating context! No more backends available.");
      return nullptr;
    }

    const GLMethods& gl = ctx->m_gl;

    ctx->m_wireframe = false;

    ctx->load_functions();

    int major = 0;
    int minor = 0;

    gl.GetIntegerv(GL_MAJOR_VERSION, &major);
    gl.GetIntegerv(GL_MINOR_VERSION, &minor);

    APPCORE_INFO("GL Version: {0}.{1}", major, minor);

    ctx->m_version_code = major * 100 + minor * 10;

    // Load extensions
    int num_extensions = 0;
    gl.GetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);

    for(int i = 0; i < num_extensions; i++)
    {
      const char* ext = (const char*)gl.GetStringi(GL_EXTENSIONS, i);
      APPCORE_INFO("Found GL extension: {0}", ext);
      ctx->m_extensions.push_back(ext);
    }

    gl.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gl.Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    gl.Enable(GL_PRIMITIVE_RESTART);
    gl.PrimitiveRestartIndex(-1);

    ctx->m_max_samples = 0;
    gl.GetIntegerv(GL_MAX_SAMPLES, (GLint*)&ctx->m_max_samples);

    ctx->m_max_integer_samples = 0;
    gl.GetIntegerv(GL_MAX_INTEGER_SAMPLES, (GLint*)&ctx->m_max_integer_samples);

    ctx->m_max_color_attachments = 0;
    gl.GetIntegerv(GL_MAX_COLOR_ATTACHMENTS, (GLint*)&ctx->m_max_color_attachments);

    ctx->m_max_texture_units = 0;
    gl.GetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint*)&ctx->m_max_texture_units);
    ctx->m_default_texture_unit = ctx->m_max_texture_units - 1;

    ctx->m_max_anisotropy = 0.0;
    gl.GetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, (GLfloat*)&ctx->m_max_anisotropy);

    int bound_framebuffer = 0;
    gl.GetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &bound_framebuffer);

#ifdef APP_OSX

    if(ctx->Mode() == ContextMode::Standalone)
    {
      int bound_framebuffer = 0;
      gl.GetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &bound_framebuffer);

      int renderbuffer = 0;
      gl.GenRenderbuffers(1, (GLuint*)&renderbuffer);
      gl.BindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
      gl.RenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, 4, 4);
      int framebuffer = 0;
      gl.GenFrameBuffers(1, (GLuint*)&framebuffer);
      gl.BindFrameBuffer(GL_FRAMEBUFFER, framebuffer);
      gl.FrameBufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
      bound_framebuffer = framebuffer;
    }
#endif
    {
      auto fb = new Framebuffer();

      fb->m_released = false;
      fb->m_framebuffer_obj = 0;

      fb->m_draw_buffers_len = 1;
      fb->m_draw_buffers = new unsigned[fb->m_draw_buffers_len];

      // According to glGet docs:
      // The initial value is GL_BACK if there are back buffers, otherwise it is GL_FRONT.

      // According to glDrawBuffer docs:
      // The symbolic constants GL_FRONT, GL_BACK, GL_LEFT, GL_RIGHT, and GL_FRONT_AND_BACK
      // are not allowed in the bufs array since they may refer to multiple buffers.

      // GL_COLOR_ATTACHMENT0 is causes error: 1282
      // This value is temporarily ignored

      // framebuffer->draw_buffers[0] = GL_COLOR_ATTACHMENT0;
      // framebuffer->draw_buffers[0] = GL_BACK_LEFT;

      gl.BindFramebuffer(GL_FRAMEBUFFER, 0);
      gl.GetIntegerv(GL_DRAW_BUFFER, (int*)&fb->m_draw_buffers[0]);
      gl.BindFramebuffer(GL_FRAMEBUFFER, bound_framebuffer);

      fb->m_color_mask = ColorMask(4);
      fb->m_color_mask[0] = true;
      fb->m_color_mask[1] = true;
      fb->m_color_mask[2] = true;
      fb->m_color_mask[3] = true;

      fb->m_depth_mask = true;
      fb->m_context = ctx;

      int scissor_box[4] = {};
      gl.GetIntegerv(GL_SCISSOR_BOX, scissor_box);

      Rect r = {scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3]};
      fb->m_scissor_enabled = false;
      fb->m_viewport = r;
      fb->m_scissor = r;

      fb->m_width = r.W;
      fb->m_height = r.H;
      fb->m_dynamic = true;

      ctx->m_default_framebuffer = AppCore::Ref<Framebuffer>(fb);
    }

    ctx->m_bound_framebuffer = ctx->m_default_framebuffer;

    ctx->m_enable_flags = 0;
    ctx->m_front_face = GL_CCW;

    ctx->m_depth_func = GL_LEQUAL;
    ctx->m_blend_func_src = GL_SRC_ALPHA;
    ctx->m_blend_func_dst = GL_ONE_MINUS_SRC_ALPHA;

    ctx->m_wireframe = false;
    ctx->m_multisample = true;

    ctx->m_provoking_vertex = GL_LAST_VERTEX_CONVENTION;

    ctx->m_polygon_offset_factor = 0.0f;
    ctx->m_polygon_offset_units = 0.0f;

    gl.GetError(); // clear errors

    return AppCore::Ref<Context>(ctx);
  }

  AppCore::Ref<Buffer> Context::buffer(void* data, size_t size, bool dynamic)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    APPCORE_ASSERT(size > 0, "invalid buffer size: {0}", size);

    const GLMethods& gl = m_gl;

    Buffer* buffer = new Buffer();
    buffer->m_released = false;

    buffer->m_size = size;
    buffer->m_dynamic = dynamic;

    buffer->m_buffer_obj = 0;
    gl.GenBuffers(1, (GLuint*)&buffer->m_buffer_obj);

    if(!buffer->m_buffer_obj)
    {
      APPCORE_ERROR("Cannot create buffer");
      delete buffer;
      return nullptr;
    }

    gl.BindBuffer(GL_ARRAY_BUFFER, buffer->m_buffer_obj);
    gl.BufferData(GL_ARRAY_BUFFER, size, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    buffer->m_context = this;

    return AppCore::Ref<Buffer>(buffer);
  }

  AppCore::Ref<ComputeShader> Context::compute_shader(const AppCore::String& source)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    const GLMethods& gl = m_gl;

    ComputeShader* compute_shader = new ComputeShader();

    compute_shader->m_released = false;
    compute_shader->m_context = this;

    int program_obj = gl.CreateProgram();

    if(!program_obj)
    {
      delete compute_shader;
      APPCORE_ERROR("cannot create program");
      return nullptr;
    }

    int shader_obj = gl.CreateShader(GL_COMPUTE_SHADER);

    if(!shader_obj)
    {
      delete compute_shader;
      APPCORE_ERROR("cannot create the shader object");
      return nullptr;
    }

    const GLchar* source_str = source.c_str();
    gl.ShaderSource(shader_obj, 1, &source_str, 0);
    gl.CompileShader(shader_obj);

    int compiled = GL_FALSE;
    gl.GetShaderiv(shader_obj, GL_COMPILE_STATUS, &compiled);

    if(!compiled)
    {
      const char* message = "GLSL Compiler failed";
      const char* title = "ComputeShader";
      const char* underline = "=============";

      int log_len = 0;
      gl.GetShaderiv(shader_obj, GL_INFO_LOG_LENGTH, &log_len);

      char* log = new char[log_len];
      gl.GetShaderInfoLog(shader_obj, log_len, &log_len, log);

      gl.DeleteShader(shader_obj);

      APPCORE_ERROR("{0}\n\n{1}\n{2}\n{3}\n", message, title, underline, log);

      delete[] log;
      delete compute_shader;
      return nullptr;
    }

    gl.AttachShader(program_obj, shader_obj);
    gl.LinkProgram(program_obj);

    int linked = GL_FALSE;
    gl.GetProgramiv(program_obj, GL_LINK_STATUS, &linked);

    if(!linked)
    {
      const char* message = "GLSL Linker failed";
      const char* title = "ComputeShader";
      const char* underline = "=============";

      int log_len = 0;
      gl.GetProgramiv(program_obj, GL_INFO_LOG_LENGTH, &log_len);

      char* log = new char[log_len];
      gl.GetProgramInfoLog(program_obj, log_len, &log_len, log);

      gl.DeleteProgram(program_obj);

      APPCORE_ERROR("{0}\n\n{1}\n{2}\n{3}\n", message, title, underline, log);

      delete[] log;
      delete compute_shader;
      return nullptr;
    }

    compute_shader->m_shader_obj = shader_obj;
    compute_shader->m_program_obj = program_obj;

    int num_uniforms = 0;

    gl.GetProgramiv(program_obj, GL_ACTIVE_UNIFORMS, &num_uniforms);

    for(int i = 0; i < num_uniforms; ++i)
    {
      int type = 0;
      int size = 0;
      int name_len = 0;
      char name[256];

      gl.GetActiveUniform(program_obj, i, 256, &name_len, &size, (GLenum*)&type, name);
      int location = gl.GetUniformLocation(program_obj, name);

      clean_glsl_name(name, name_len);

      if(location < 0)
      {
        continue;
      }

      compute_shader->m_uniforms_map.insert(
          {name, AppCore::Ref<Uniform>(new Uniform(name, type, program_obj, location, size, this))});
    }

    int num_uniform_blocks = 0;
    gl.GetProgramiv(program_obj, GL_ACTIVE_UNIFORM_BLOCKS, &num_uniform_blocks);

    for(int i = 0; i < num_uniform_blocks; ++i)
    {
      int size = 0;
      int name_len = 0;
      char name[256];

      gl.GetActiveUniformBlockName(program_obj, i, 256, &name_len, name);
      int index = gl.GetUniformBlockIndex(program_obj, name);
      gl.GetActiveUniformBlockiv(program_obj, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);

      clean_glsl_name(name, name_len);

      compute_shader->m_uniform_blocks_map.insert(
          {name, AppCore::Ref<UniformBlock>(new UniformBlock(name, program_obj, index, size, this))});
    }

    return AppCore::Ref<ComputeShader>(compute_shader);
  }

  AppCore::Ref<Framebuffer> Context::framebuffer(const AttachmentsRef& color_attachments,
                                                 AppCore::Ref<Attachment> depth_attachment)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    APPCORE_ASSERT(color_attachments.size(), "missing color attachments");
    const GLMethods& gl = m_gl;

    int width = 0;
    int height = 0;
    int samples = 0;

    if(color_attachments.size() > 0)
    {
      width = color_attachments[0]->width();
      height = color_attachments[0]->height();
      samples = color_attachments[0]->samples();
      auto attachment_type = color_attachments[0]->attachment_type();

      for(auto t : color_attachments)
      {
        APPCORE_ASSERT(t->context() == this, "color_attachment belongs to a different context");
        APPCORE_ASSERT(t->attachment_type() == attachment_type, "color_attachment are of different type");
        APPCORE_ASSERT(!t->depth(), "color_attachment is a depth attachment");
        APPCORE_ASSERT(t->width() == width && t->height() == height, "color_attachment have different sizes");
        APPCORE_ASSERT(t->samples() == samples, "color_attachment have different samples");
      }

      if(depth_attachment != nullptr)
      {
        APPCORE_ASSERT(depth_attachment->context() == this, "color_attachment belongs to a different context");
        APPCORE_ASSERT(depth_attachment->depth(), "color_attachment is a depth attachment");
        APPCORE_ASSERT(depth_attachment->width() == width && depth_attachment->height() == height,
                       "color_attachment have different sizes");
        APPCORE_ASSERT(depth_attachment->samples() == samples, "color_attachment have different samples");
      }
    }
    else if(depth_attachment != nullptr)
    {
      width = depth_attachment->width();
      height = depth_attachment->height();
      samples = depth_attachment->samples();
    }
    else
    {
      APPCORE_ASSERT(false, "the framebuffer is empty");
      return nullptr;
    }

    Framebuffer* framebuffer = new Framebuffer();
    framebuffer->m_released = false;
    framebuffer->m_draw_buffers_len = color_attachments.size();
    framebuffer->m_draw_buffers = new unsigned[color_attachments.size()];
    framebuffer->m_color_mask = ColorMask(color_attachments.size() * 4 + 1);
    framebuffer->m_depth_mask = (depth_attachment != nullptr);
    framebuffer->m_viewport = {0, 0, width, height};
    framebuffer->m_dynamic = false;
    framebuffer->m_scissor_enabled = false;
    framebuffer->m_scissor = {0, 0, width, height};
    framebuffer->m_width = width;
    framebuffer->m_height = height;
    framebuffer->m_samples = samples;
    framebuffer->m_context = this;

    framebuffer->m_framebuffer_obj = 0;
    gl.GenFramebuffers(1, (GLuint*)&framebuffer->m_framebuffer_obj);

    if(!framebuffer->m_framebuffer_obj)
    {
      APPCORE_ERROR("cannot create framebuffer");
      delete framebuffer;
      return nullptr;
    }

    gl.BindFramebuffer(GL_FRAMEBUFFER, framebuffer->m_framebuffer_obj);

    if(!color_attachments.size())
    {
      gl.DrawBuffer(GL_NONE); // No color buffer is drawn to.
    }
    else
    {
      for(size_t i = 0; i < color_attachments.size(); ++i)
      {
        auto attachment = color_attachments[i];
        attachment->color_attachment(framebuffer, i);
      }
    }

    if(depth_attachment != nullptr)
    {
      depth_attachment->depth_attachment();
    }

    int status = gl.CheckFramebufferStatus(GL_FRAMEBUFFER);

    gl.BindFramebuffer(GL_FRAMEBUFFER, m_bound_framebuffer->m_framebuffer_obj);

    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
      framebuffer_error_message(status);
      delete framebuffer;
      return nullptr;
    }

    return AppCore::Ref<Framebuffer>(framebuffer);
  }

  AppCore::Ref<Renderbuffer>
  Context::renderbuffer(uint32_t w, uint32_t h, uint8_t components, uint8_t samples, const char* dtype, size_t dtype_size)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    const GLMethods& gl = m_gl;

    if(components < 1 || components > 4)
    {
      APPCORE_ERROR("Components must be 1, 2, 3 or 4, got: {0}", components);
      return nullptr;
    }

    if((samples & (samples - 1)) || samples > m_max_samples)
    {
      APPCORE_ERROR("The number of samples is invalid: {0}", samples);
      return nullptr;
    }

    DataType* dataType = from_dtype(dtype, dtype_size);

    if(!dataType)
    {
      APPCORE_ERROR("Invalid data type: '{0}'", dtype);
      return nullptr;
    }

    int format = dataType->internal_format[components];

    Renderbuffer* renderbuffer = new Renderbuffer();
    renderbuffer->m_released = false;

    renderbuffer->m_renderbuffer_obj = 0;
    gl.GenRenderbuffers(1, (GLuint*)&renderbuffer->m_renderbuffer_obj);

    if(!renderbuffer->m_renderbuffer_obj)
    {
      APPCORE_ERROR("Cannot create RenderBuffer");
      delete(renderbuffer);
      return nullptr;
    }

    gl.BindRenderbuffer(GL_RENDERBUFFER, renderbuffer->m_renderbuffer_obj);

    if(samples == 0)
    {
      gl.RenderbufferStorage(GL_RENDERBUFFER, format, w, h);
    }
    else
    {
      gl.RenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, w, h);
    }

    renderbuffer->m_width = w;
    renderbuffer->m_height = h;
    renderbuffer->m_components = components;
    renderbuffer->m_samples = samples;
    renderbuffer->m_data_type = dataType;
    renderbuffer->m_depth = false;

    renderbuffer->m_context = this;

    return AppCore::Ref<Renderbuffer>(renderbuffer);
  }

  AppCore::Ref<Program> Context::program(const ShadersSources& shaders,
                                         const ShadersOutputs& outputs,
                                         const FragmentOutputs& fragment_outputs,
                                         bool interleaved)
  {
    return nullptr;
  }

} // namespace AppGL
