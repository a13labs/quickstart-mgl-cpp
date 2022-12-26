
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
#include "context.hpp"
#include "appcore/log.hpp"
#include "attribute.hpp"
#include "buffer.hpp"
#include "computeshader.hpp"
#include "datatype.hpp"
#include "format.hpp"
#include "framebuffer.hpp"
#include "program.hpp"
#include "query.hpp"
#include "renderbuffer.hpp"
#include "sampler.hpp"
#include "scope.hpp"
#include "shaderssources.hpp"
#include "subroutine.hpp"
#include "texture2d.hpp"
#include "texture3d.hpp"
#include "texturearray.hpp"
#include "texturecube.hpp"
#include "uniform.hpp"
#include "uniformblock.hpp"
#include "varying.hpp"
#include "vertexarray.hpp"

namespace AppGL
{

  static const int SHADER_TYPE[5] = {
    GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER,
  };

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
      auto framebuffer = new Framebuffer();
      framebuffer->m_released = false;
      framebuffer->m_context = ctx;
      framebuffer->m_framebuffer_obj = 0;
      framebuffer->m_draw_buffers_len = 1;
      framebuffer->m_draw_buffers = new unsigned[1];

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
      gl.GetIntegerv(GL_DRAW_BUFFER, (int*)&framebuffer->m_draw_buffers[0]);
      gl.BindFramebuffer(GL_FRAMEBUFFER, bound_framebuffer);

      framebuffer->m_color_masks = { { true, true, true, true } };
      framebuffer->m_depth_mask = true;

      int scissor_box[4] = {};
      gl.GetIntegerv(GL_SCISSOR_BOX, scissor_box);

      framebuffer->m_viewport = { scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3] };

      framebuffer->m_scissor_enabled = false;
      framebuffer->m_scissor = { scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3] };

      framebuffer->m_width = scissor_box[2];
      framebuffer->m_height = scissor_box[3];
      framebuffer->m_dynamic = true;

      ctx->m_default_framebuffer = AppCore::Ref<Framebuffer>(framebuffer);
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

  AppCore::Ref<Buffer> Context::buffer(void* data, size_t reserve, bool dynamic)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    APPCORE_ASSERT(reserve >= 0, "invalid buffer size: {0}", reserve);

    const GLMethods& gl = m_gl;

    auto buffer = new Buffer();
    buffer->m_released = false;

    buffer->m_size = reserve;
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
    gl.BufferData(GL_ARRAY_BUFFER, reserve, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    buffer->m_context = this;

    return AppCore::Ref<Buffer>(buffer);
  }

  AppCore::Ref<ComputeShader> Context::compute_shader(const AppCore::String& source)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    const GLMethods& gl = m_gl;

    auto compute_shader = new ComputeShader();

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
          { name, AppCore::Ref<Uniform>(new Uniform(name, type, program_obj, location, size, this)) });
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
          { name, AppCore::Ref<UniformBlock>(new UniformBlock(name, program_obj, index, size, this)) });
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

    if(!color_attachments.size() && depth_attachment == nullptr)
    {
      APPCORE_ERROR("the framebuffer is empty");
      return nullptr;
    }

    int i = 0;
    for(auto&& attachment : color_attachments)
    {
      if(attachment->attachment_type() == Attachment::Type::TEXTURE)
      {
        auto texture = std::dynamic_pointer_cast<Texture2D>(attachment);
        APPCORE_ASSERT(texture, "Not a texture2D");

        if(texture->m_depth)
        {
          APPCORE_ERROR("color_attachments[{0}] is a depth attachment", i);
          return nullptr;
        }

        if(i == 0)
        {
          width = texture->m_width;
          height = texture->m_height;
          samples = texture->m_samples;
        }
        else
        {
          if(texture->m_width != width || texture->m_height != height || texture->m_samples != samples)
          {
            APPCORE_ERROR("the color_attachments have different sizes or samples");
            return nullptr;
          }
        }
      }
      else if(attachment->attachment_type() == Attachment::Type::TEXTURE)
      {
        auto renderbuffer = std::dynamic_pointer_cast<Renderbuffer>(attachment);
        APPCORE_ASSERT(renderbuffer, "Not a Renderbuffer");

        if(renderbuffer->m_depth)
        {
          APPCORE_ERROR("color_attachments[{0}] is a depth attachment", i);
          return nullptr;
        }

        if(i == 0)
        {
          width = renderbuffer->m_width;
          height = renderbuffer->m_height;
          samples = renderbuffer->m_samples;
        }
        else
        {
          if(renderbuffer->m_width != width || renderbuffer->m_height != height || renderbuffer->m_samples != samples)
          {
            APPCORE_ERROR("the color_attachments have different sizes or samples");
            return nullptr;
          }
        }
      }
      i++;
    }

    if(depth_attachment != nullptr)
    {
      if(depth_attachment->attachment_type() == Attachment::Type::TEXTURE)
      {
        auto texture = std::dynamic_pointer_cast<Texture2D>(depth_attachment);
        APPCORE_ASSERT(texture, "Not a texture2D");

        if(!texture->m_depth)
        {
          APPCORE_ERROR("the depth_attachment is a color attachment");
          return nullptr;
        }

        if(texture->m_context != this)
        {
          APPCORE_ERROR("the depth_attachment belongs to a different context");
          return nullptr;
        }

        if(color_attachments.size())
        {
          if(texture->m_width != width || texture->m_height != height || texture->m_samples != samples)
          {
            APPCORE_ERROR("the depth_attachment have different sizes or samples");
            return nullptr;
          }
        }
        else
        {
          width = texture->m_width;
          height = texture->m_height;
          samples = texture->m_samples;
        }
      }
      else if(depth_attachment->attachment_type() == Attachment::Type::TEXTURE)
      {
        auto renderbuffer = std::dynamic_pointer_cast<Renderbuffer>(depth_attachment);
        APPCORE_ASSERT(renderbuffer, "Not a Renderbuffer");

        if(!renderbuffer->m_depth)
        {
          APPCORE_ERROR("the depth_attachment is a color attachment");
          return nullptr;
        }

        if(renderbuffer->m_context != this)
        {
          APPCORE_ERROR("the depth_attachment belongs to a different context");
          return nullptr;
        }

        if(color_attachments.size())
        {
          if(renderbuffer->m_width != width || renderbuffer->m_height != height || renderbuffer->m_samples != samples)
          {
            APPCORE_ERROR("the depth_attachment have different sizes or samples");
            return nullptr;
          }
        }
        else
        {
          width = renderbuffer->m_width;
          height = renderbuffer->m_height;
          samples = renderbuffer->m_samples;
        }
      }
      else
      {
        APPCORE_ERROR("the depth_attachment must be a Renderbuffer or Texture not %s");
        return nullptr;
      }
    }

    auto framebuffer = new Framebuffer();
    framebuffer->m_released = false;
    framebuffer->m_draw_buffers_len = color_attachments.size();
    framebuffer->m_draw_buffers = new unsigned[color_attachments.size()];
    framebuffer->m_color_masks = ColorMasks(color_attachments.size());

    for(size_t i = 0; i < color_attachments.size(); ++i)
    {
      auto attachment = color_attachments[i];

      framebuffer->m_draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;

      if(attachment->attachment_type() == Attachment::Type::TEXTURE)
      {
        auto texture = std::dynamic_pointer_cast<Texture2D>(attachment);
        APPCORE_ASSERT(texture, "Not a texture2D");
        framebuffer->m_color_masks[i] = {
          texture->m_components >= 1, texture->m_components >= 2, texture->m_components >= 3, texture->m_components >= 4
        };
      }
      else if(attachment->attachment_type() == Attachment::Type::TEXTURE)
      {
        auto renderbuffer = std::dynamic_pointer_cast<Renderbuffer>(attachment);
        APPCORE_ASSERT(renderbuffer, "Not a Renderbuffer");
        framebuffer->m_color_masks[i] = { renderbuffer->m_components >= 1,
                                          renderbuffer->m_components >= 2,
                                          renderbuffer->m_components >= 3,
                                          renderbuffer->m_components >= 4 };
      }
    }

    framebuffer->m_depth_mask = (depth_attachment != nullptr);
    framebuffer->m_viewport = { 0, 0, width, height };
    framebuffer->m_dynamic = false;
    framebuffer->m_scissor_enabled = false;
    framebuffer->m_scissor = { 0, 0, width, height };
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

    for(auto&& attachment : color_attachments)
    {
      if(attachment->attachment_type() == Attachment::Type::TEXTURE)
      {
        auto texture = std::dynamic_pointer_cast<Texture2D>(attachment);
        APPCORE_ASSERT(texture, "Not a texture2D");

        gl.FramebufferTexture2D(GL_FRAMEBUFFER,
                                GL_COLOR_ATTACHMENT0 + i,
                                texture->m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
                                texture->m_texture_obj,
                                0);
      }
      else if(attachment->attachment_type() == Attachment::Type::TEXTURE)
      {
        auto renderbuffer = std::dynamic_pointer_cast<Renderbuffer>(attachment);
        APPCORE_ASSERT(renderbuffer, "Not a Renderbuffer");

        gl.FramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, renderbuffer->m_renderbuffer_obj);
      }
    }

    if(depth_attachment)
    {
      if(depth_attachment->attachment_type() == Attachment::Type::TEXTURE)
      {
        auto texture = std::dynamic_pointer_cast<Texture2D>(depth_attachment);
        APPCORE_ASSERT(texture, "Not a texture2D");

        gl.FramebufferTexture2D(GL_FRAMEBUFFER,
                                GL_DEPTH_ATTACHMENT,
                                texture->m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
                                texture->m_texture_obj,
                                0);
      }
      else if(depth_attachment->attachment_type() == Attachment::Type::TEXTURE)
      {
        auto renderbuffer = std::dynamic_pointer_cast<Renderbuffer>(depth_attachment);
        APPCORE_ASSERT(renderbuffer, "Not a Renderbuffer");

        gl.FramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer->m_renderbuffer_obj);
      }
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

  AppCore::Ref<Program> Context::program(const ShadersSources& shaders,
                                         const ShadersOutputs& outputs,
                                         const FragmentOutputs& fragment_outputs,
                                         bool interleaved)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    const GLMethods& gl = m_gl;

    Program* program = new Program();
    program->m_released = false;
    program->m_context = this;
    program->m_transform = shaders.sources[ShadersSources::FRAGMENT_SHADER].empty();

    int program_obj = gl.CreateProgram();

    if(!program_obj)
    {
      APPCORE_ERROR("cannot create program");
      delete program;
      return nullptr;
    }

    int shader_objs[] = { 0, 0, 0, 0, 0 };

    for(int i = 0; i < ShadersSources::COUNT; ++i)
    {
      if(shaders.sources[i] == "")
      {
        continue;
      }

      const char* source_str = shaders.sources[i].c_str();

      int shader_obj = gl.CreateShader(SHADER_TYPE[i]);

      if(!shader_obj)
      {
        APPCORE_ERROR("cannot create shader");
        delete program;
        return nullptr;
      }

      gl.ShaderSource(shader_obj, 1, &source_str, 0);
      gl.CompileShader(shader_obj);

      int compiled = GL_FALSE;
      gl.GetShaderiv(shader_obj, GL_COMPILE_STATUS, &compiled);

      if(!compiled)
      {
        const char* SHADER_NAME[] = {
          "vertex_shader", "fragment_shader", "geometry_shader", "tess_control_shader", "tess_evaluation_shader",
        };

        const char* SHADER_NAME_UNDERLINE[] = {
          "=============", "===============", "===============", "===================", "======================",
        };

        const char* message = "GLSL Compiler failed";
        const char* title = SHADER_NAME[i];
        const char* underline = SHADER_NAME_UNDERLINE[i];

        int log_len = 0;
        gl.GetShaderiv(shader_obj, GL_INFO_LOG_LENGTH, &log_len);

        char* log = new char[log_len];
        gl.GetShaderInfoLog(shader_obj, log_len, &log_len, log);

        gl.DeleteShader(shader_obj);

        APPCORE_ERROR("{0}\n\n{1}\n{2}\n{3}\n", message, title, underline, log);

        delete[] log;
        delete program;
        return nullptr;
      }

      shader_objs[i] = shader_obj;
      gl.AttachShader(program_obj, shader_obj);
    }

    if(outputs.size())
    {
      const char** varyings_array = new const char*[outputs.size()];

      for(int i = 0; i < (int)outputs.size(); ++i)
      {
        varyings_array[i] = outputs[i].c_str();
      }

      int capture_mode = interleaved ? GL_INTERLEAVED_ATTRIBS : GL_SEPARATE_ATTRIBS;

      gl.TransformFeedbackVaryings(program_obj, outputs.size(), varyings_array, capture_mode);

      delete[] varyings_array;
    }

    for(auto&& fo : fragment_outputs)
    {
      gl.BindFragDataLocation(program_obj, fo.second, fo.first.c_str());
    }

    gl.LinkProgram(program_obj);

    for(int i = 0; i < ShadersSources::COUNT; ++i)
    {
      if(shader_objs[i])
      {
        gl.DeleteShader(shader_objs[i]);
      }
    }

    int linked = GL_FALSE;
    gl.GetProgramiv(program_obj, GL_LINK_STATUS, &linked);

    if(!linked)
    {
      const char* message = "GLSL Linker failed";
      const char* title = "Program";
      const char* underline = "=======";

      int log_len = 0;
      gl.GetProgramiv(program_obj, GL_INFO_LOG_LENGTH, &log_len);

      char* log = new char[log_len];
      gl.GetProgramInfoLog(program_obj, log_len, &log_len, log);

      gl.DeleteProgram(program_obj);

      APPCORE_ERROR("{0}\n\n{1}\n{2}\n{3}\n", message, title, underline, log);

      delete[] log;
      delete program;
      return nullptr;
    }

    program->m_program_obj = program_obj;

    int num_vertex_shader_subroutines = 0;
    int num_fragment_shader_subroutines = 0;
    int num_geometry_shader_subroutines = 0;
    int num_tess_evaluation_shader_subroutines = 0;
    int num_tess_control_shader_subroutines = 0;

    int num_vertex_shader_subroutine_uniforms = 0;
    int num_fragment_shader_subroutine_uniforms = 0;
    int num_geometry_shader_subroutine_uniforms = 0;
    int num_tess_evaluation_shader_subroutine_uniforms = 0;
    int num_tess_control_shader_subroutine_uniforms = 0;

    if(program->m_context->version_code() >= 400)
    {
      if(!shaders.sources[ShadersSources::Type::VERTEX_SHADER].empty())
      {
        gl.GetProgramStageiv(program_obj, GL_VERTEX_SHADER, GL_ACTIVE_SUBROUTINES, &num_vertex_shader_subroutines);
        gl.GetProgramStageiv(
            program_obj, GL_VERTEX_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &num_vertex_shader_subroutine_uniforms);
      }

      if(!shaders.sources[ShadersSources::Type::FRAGMENT_SHADER].empty())
      {
        gl.GetProgramStageiv(program_obj, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINES, &num_fragment_shader_subroutines);
        gl.GetProgramStageiv(
            program_obj, GL_FRAGMENT_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &num_fragment_shader_subroutine_uniforms);
      }

      if(!shaders.sources[ShadersSources::Type::GEOMETRY_SHADER].empty())
      {
        gl.GetProgramStageiv(program_obj, GL_GEOMETRY_SHADER, GL_ACTIVE_SUBROUTINES, &num_geometry_shader_subroutines);
        gl.GetProgramStageiv(
            program_obj, GL_GEOMETRY_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &num_geometry_shader_subroutine_uniforms);
      }

      if(!shaders.sources[ShadersSources::Type::TESS_EVALUATION_SHADER].empty())
      {
        gl.GetProgramStageiv(
            program_obj, GL_TESS_EVALUATION_SHADER, GL_ACTIVE_SUBROUTINES, &num_tess_evaluation_shader_subroutines);
        gl.GetProgramStageiv(program_obj,
                             GL_TESS_EVALUATION_SHADER,
                             GL_ACTIVE_SUBROUTINE_UNIFORMS,
                             &num_tess_evaluation_shader_subroutine_uniforms);
      }

      if(!shaders.sources[ShadersSources::Type::TESS_CONTROL_SHADER].empty())
      {
        gl.GetProgramStageiv(program_obj, GL_TESS_CONTROL_SHADER, GL_ACTIVE_SUBROUTINES, &num_tess_control_shader_subroutines);
        gl.GetProgramStageiv(
            program_obj, GL_TESS_CONTROL_SHADER, GL_ACTIVE_SUBROUTINE_UNIFORMS, &num_tess_control_shader_subroutine_uniforms);
      }
    }

    program->m_num_vertex_shader_subroutines = num_vertex_shader_subroutine_uniforms;
    program->m_num_fragment_shader_subroutines = num_fragment_shader_subroutine_uniforms;
    program->m_num_geometry_shader_subroutines = num_geometry_shader_subroutine_uniforms;
    program->m_num_tess_evaluation_shader_subroutines = num_tess_evaluation_shader_subroutine_uniforms;
    program->m_num_tess_control_shader_subroutines = num_tess_control_shader_subroutine_uniforms;

    if(!shaders.sources[ShadersSources::Type::GEOMETRY_SHADER].empty())
    {

      int geometry_in = 0;
      int geometry_out = 0;
      program->m_geometry_vertices = 0;

      gl.GetProgramiv(program_obj, GL_GEOMETRY_INPUT_TYPE, &geometry_in);
      gl.GetProgramiv(program_obj, GL_GEOMETRY_OUTPUT_TYPE, &geometry_out);
      gl.GetProgramiv(program_obj, GL_GEOMETRY_VERTICES_OUT, &program->m_geometry_vertices);

      switch(geometry_in)
      {
        case GL_TRIANGLES: program->m_geometry_input = GL_TRIANGLES; break;

        case GL_TRIANGLE_STRIP: program->m_geometry_input = GL_TRIANGLE_STRIP; break;

        case GL_TRIANGLE_FAN: program->m_geometry_input = GL_TRIANGLE_FAN; break;

        case GL_LINES: program->m_geometry_input = GL_LINES; break;

        case GL_LINE_STRIP: program->m_geometry_input = GL_LINE_STRIP; break;

        case GL_LINE_LOOP: program->m_geometry_input = GL_LINE_LOOP; break;

        case GL_POINTS: program->m_geometry_input = GL_POINTS; break;

        case GL_LINE_STRIP_ADJACENCY: program->m_geometry_input = GL_LINE_STRIP_ADJACENCY; break;

        case GL_LINES_ADJACENCY: program->m_geometry_input = GL_LINES_ADJACENCY; break;

        case GL_TRIANGLE_STRIP_ADJACENCY: program->m_geometry_input = GL_TRIANGLE_STRIP_ADJACENCY; break;

        case GL_TRIANGLES_ADJACENCY: program->m_geometry_input = GL_TRIANGLES_ADJACENCY; break;

        default: program->m_geometry_input = -1; break;
      }

      switch(geometry_out)
      {
        case GL_TRIANGLES: program->m_geometry_output = GL_TRIANGLES; break;

        case GL_TRIANGLE_STRIP: program->m_geometry_output = GL_TRIANGLES; break;

        case GL_TRIANGLE_FAN: program->m_geometry_output = GL_TRIANGLES; break;

        case GL_LINES: program->m_geometry_output = GL_LINES; break;

        case GL_LINE_STRIP: program->m_geometry_output = GL_LINES; break;

        case GL_LINE_LOOP: program->m_geometry_output = GL_LINES; break;

        case GL_POINTS: program->m_geometry_output = GL_POINTS; break;

        case GL_LINE_STRIP_ADJACENCY: program->m_geometry_output = GL_LINES; break;

        case GL_LINES_ADJACENCY: program->m_geometry_output = GL_LINES; break;

        case GL_TRIANGLE_STRIP_ADJACENCY: program->m_geometry_output = GL_TRIANGLES; break;

        case GL_TRIANGLES_ADJACENCY: program->m_geometry_output = GL_TRIANGLES; break;

        default: program->m_geometry_output = -1; break;
      }
    }
    else
    {
      program->m_geometry_input = -1;
      program->m_geometry_output = -1;
      program->m_geometry_vertices = 0;
    }

    int num_attributes = 0;
    int num_varyings = 0;
    int num_uniforms = 0;
    int num_uniform_blocks = 0;

    gl.GetProgramiv(program->m_program_obj, GL_ACTIVE_ATTRIBUTES, &num_attributes);
    gl.GetProgramiv(program->m_program_obj, GL_TRANSFORM_FEEDBACK_VARYINGS, &num_varyings);
    gl.GetProgramiv(program->m_program_obj, GL_ACTIVE_UNIFORMS, &num_uniforms);
    gl.GetProgramiv(program->m_program_obj, GL_ACTIVE_UNIFORM_BLOCKS, &num_uniform_blocks);

    for(int i = 0; i < num_attributes; ++i)
    {
      int type = 0;
      int array_length = 0;
      int name_len = 0;
      char name[256];

      gl.GetActiveAttrib(program->m_program_obj, i, 256, &name_len, &array_length, (GLenum*)&type, name);
      int location = gl.GetAttribLocation(program->m_program_obj, name);

      clean_glsl_name(name, name_len);

      program->m_attributes_map.insert(
          { name, AppCore::Ref<Attribute>(new Attribute(name, type, program->m_program_obj, location, array_length)) });
    }

    for(int i = 0; i < num_varyings; ++i)
    {
      int type = 0;
      int array_length = 0;
      int dimension = 0;
      int name_len = 0;
      char name[256];

      gl.GetTransformFeedbackVarying(program->m_program_obj, i, 256, &name_len, &array_length, (GLenum*)&type, name);

      program->m_varyings_map.insert({ name, AppCore::Ref<Varying>(new Varying(name, i, array_length, dimension)) });
    }

    for(int i = 0; i < num_uniforms; ++i)
    {
      int type = 0;
      int size = 0;
      int name_len = 0;
      char name[256];

      gl.GetActiveUniform(program->m_program_obj, i, 256, &name_len, &size, (GLenum*)&type, name);
      int location = gl.GetUniformLocation(program->m_program_obj, name);

      clean_glsl_name(name, name_len);

      if(location < 0)
      {
        continue;
      }

      program->m_uniforms_map.insert({ name, AppCore::Ref<Uniform>(new Uniform(name, type, program_obj, location, size, this)) });
    }

    for(int i = 0; i < num_uniform_blocks; ++i)
    {
      int size = 0;
      int name_len = 0;
      char name[256];

      gl.GetActiveUniformBlockName(program->m_program_obj, i, 256, &name_len, name);
      int index = gl.GetUniformBlockIndex(program->m_program_obj, name);
      gl.GetActiveUniformBlockiv(program->m_program_obj, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);

      clean_glsl_name(name, name_len);

      program->m_uniform_blocks_map.insert(
          { name, AppCore::Ref<UniformBlock>(new UniformBlock(name, program_obj, index, size, this)) });
    }

    if(program->m_context->version_code() >= 400)
    {
      for(int st = 0; st < 5; ++st)
      {
        int num_subroutines = 0;
        auto type = AppGL::Subroutine::Type(SHADER_TYPE[st]);

        gl.GetProgramStageiv(program_obj, type, GL_ACTIVE_SUBROUTINES, &num_subroutines);

        int num_subroutine_uniforms = 0;
        gl.GetProgramStageiv(program_obj, type, GL_ACTIVE_SUBROUTINE_UNIFORMS, &num_subroutine_uniforms);

        for(int i = 0; i < num_subroutines; ++i)
        {
          int name_len = 0;
          char name[256];

          gl.GetActiveSubroutineName(program_obj, type, i, 256, &name_len, name);
          int index = gl.GetSubroutineIndex(program_obj, type, name);

          program->m_subroutines_map.insert({ name, AppCore::Ref<Subroutine>(new Subroutine(name, index, type)) });
        }
      }
    }

    return AppCore::Ref<Program>(program);
  }

  AppCore::Ref<Query> Context::query(bool samples, bool any_samples, bool time_elapsed, bool primitives_generated)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    const GLMethods& gl = m_gl;

    if(!(samples || any_samples || time_elapsed || primitives_generated))
    {
      samples = true;
      any_samples = true;
      time_elapsed = true;
      primitives_generated = true;
    }

    auto query = new Query();
    query->m_context = this;

    if(samples)
    {
      gl.GenQueries(1, (GLuint*)&query->m_query_obj[Query::Keys::SAMPLES_PASSED]);
    }
    else
    {
      query->m_query_obj[Query::Keys::SAMPLES_PASSED] = 0;
    }

    if(any_samples)
    {
      gl.GenQueries(1, (GLuint*)&query->m_query_obj[Query::Keys::ANY_SAMPLES_PASSED]);
    }
    else
    {
      query->m_query_obj[Query::Keys::ANY_SAMPLES_PASSED] = 0;
    }

    if(time_elapsed)
    {
      gl.GenQueries(1, (GLuint*)&query->m_query_obj[Query::Keys::TIME_ELAPSED]);
    }
    else
    {
      query->m_query_obj[Query::Keys::TIME_ELAPSED] = 0;
    }

    if(primitives_generated)
    {
      gl.GenQueries(1, (GLuint*)&query->m_query_obj[Query::Keys::PRIMITIVES_GENERATED]);
    }
    else
    {
      query->m_query_obj[Query::Keys::PRIMITIVES_GENERATED] = 0;
    }

    return AppCore::Ref<Query>(query);
  }

  AppCore::Ref<Renderbuffer> Context::renderbuffer(int width, int height, int components, int samples, const char* dtype)
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
      APPCORE_ERROR("The number of samples is invalid got: {0}", samples);
      return nullptr;
    }

    auto data_type = from_dtype(dtype, strlen(dtype));

    if(!data_type)
    {
      APPCORE_ERROR("Invalid data type got: '{0}'", dtype);
      return nullptr;
    }

    int format = data_type->internal_format[components];

    auto renderbuffer = new Renderbuffer();
    renderbuffer->m_released = false;
    renderbuffer->m_context = this;
    renderbuffer->m_width = width;
    renderbuffer->m_height = height;
    renderbuffer->m_components = components;
    renderbuffer->m_samples = samples;
    renderbuffer->m_data_type = data_type;
    renderbuffer->m_depth = false;

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
      gl.RenderbufferStorage(GL_RENDERBUFFER, format, width, height);
    }
    else
    {
      gl.RenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, width, height);
    }

    return AppCore::Ref<Renderbuffer>(renderbuffer);
  }

  AppCore::Ref<Renderbuffer> Context::depth_renderbuffer(int width, int height, int samples)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    const GLMethods& gl = m_gl;

    if((samples & (samples - 1)) || samples > m_max_samples)
    {
      APPCORE_ERROR("The number of samples is invalid got: {0}", samples);
      return nullptr;
    }

    auto renderbuffer = new Renderbuffer();
    renderbuffer->m_released = false;
    renderbuffer->m_context = this;
    renderbuffer->m_width = width;
    renderbuffer->m_height = height;
    renderbuffer->m_components = 1;
    renderbuffer->m_samples = samples;
    renderbuffer->m_data_type = from_dtype("f4", 2);
    renderbuffer->m_depth = true;

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
      gl.RenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    }
    else
    {
      gl.RenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, width, height);
    }

    return AppCore::Ref<Renderbuffer>(renderbuffer);
  }

  AppCore::Ref<Sampler> Context::sampler()
  {
    APPCORE_ASSERT(!released(), "Context already released");
    const GLMethods& gl = m_gl;

    auto sampler = new Sampler();
    sampler->m_released = false;
    sampler->m_context = this;
    sampler->m_filter = { GL_LINEAR, GL_LINEAR };
    sampler->m_anisotropy = 0.0;
    sampler->m_repeat_x = true;
    sampler->m_repeat_y = true;
    sampler->m_repeat_z = true;
    sampler->m_compare_func = Sampler::Func::NONE;
    sampler->m_border_color[0] = 0.0;
    sampler->m_border_color[1] = 0.0;
    sampler->m_border_color[2] = 0.0;
    sampler->m_border_color[3] = 0.0;
    sampler->m_min_lod = -1000.0;
    sampler->m_max_lod = 1000.0;

    gl.GenSamplers(1, (GLuint*)&sampler->m_sampler_obj);

    return AppCore::Ref<Sampler>(sampler);
  }

  AppCore::Ref<Scope> Context::scope(AppCore::Ref<Framebuffer> framebuffer,
                                     int enable_flags,
                                     const TextureBindings& textures,
                                     const BufferBindings& uniform_buffers,
                                     const BufferBindings& storage_buffers,
                                     const SamplerBindings& samplers)
  {

    APPCORE_ASSERT(!released(), "Context already released");

    auto scope = new Scope();
    scope->m_released = false;
    scope->m_context = this;
    scope->m_enable_flags = enable_flags;
    scope->m_old_enable_flags = Context::EnableFlag::INVALID;
    scope->m_framebuffer = framebuffer;
    scope->m_old_framebuffer = m_bound_framebuffer;
    scope->m_textures = AppCore::List<Scope::BindingData>(textures.size());
    scope->m_buffers = AppCore::List<Scope::BindingData>(uniform_buffers.size() + storage_buffers.size());
    scope->m_samplers = samplers;

    int i = 0;
    for(auto&& t : textures)
    {
      int texture_type;
      int texture_obj;

      APPCORE_ASSERT(t.texture, "Texture is null");

      switch(t.texture->texture_type())
      {
        case Texture::Type::TEXTURE_2D: {
          auto texture = std::dynamic_pointer_cast<Texture2D>(t.texture);
          APPCORE_ASSERT(texture != nullptr, "invalid texture");
          texture_type = texture->m_samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
          texture_obj = texture->m_texture_obj;
        }
        /* code */
        break;
        case Texture::Type::TEXTURE_3D: {
          auto texture = std::dynamic_pointer_cast<Texture3D>(t.texture);
          APPCORE_ASSERT(texture != nullptr, "invalid texture");
          texture_type = GL_TEXTURE_3D;
          texture_obj = texture->m_texture_obj;
        }
        break;
        case Texture::Type::TEXTURE_CUBE: {
          auto texture = std::dynamic_pointer_cast<Texture3D>(t.texture);
          APPCORE_ASSERT(texture != nullptr, "invalid texture");
          texture_type = GL_TEXTURE_CUBE_MAP;
          texture_obj = texture->m_texture_obj;
        }
        break;
        default:
          delete scope;
          APPCORE_ERROR("invalid texture");
          return nullptr;
      }

      int binding = t.binding;
      scope->m_textures[i].binding = GL_TEXTURE0 + binding;
      scope->m_textures[i].type = texture_type;
      scope->m_textures[i].gl_object = texture_obj;
      i++;
    }

    i = 0;
    for(auto&& b : uniform_buffers)
    {
      APPCORE_ASSERT(b.buffer, "buffer is null");

      scope->m_buffers[i].binding = b.binding;
      scope->m_buffers[i].gl_object = b.buffer->m_buffer_obj;
      scope->m_buffers[i].type = GL_UNIFORM_BUFFER;
      i++;
    }

    for(auto&& b : storage_buffers)
    {
      APPCORE_ASSERT(b.buffer, "buffer is null");

      scope->m_buffers[i].binding = b.binding;
      scope->m_buffers[i].gl_object = b.buffer->m_buffer_obj;
      scope->m_buffers[i].type = GL_SHADER_STORAGE_BUFFER;
      i++;
    }

    return AppCore::Ref<Scope>(scope);
  }

  AppCore::Ref<Texture2D> Context::texture2d(int width,
                                             int height,
                                             int components,
                                             const void* data,
                                             int samples,
                                             int alignment,
                                             const char* dtype,
                                             int internal_format_override)
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
      APPCORE_ERROR("The number of samples is invalid got: {0}", samples);
      return nullptr;
    }

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("The alignment must be 1, 2, 4 or 8, got: {0}", alignment);
      return nullptr;
    }

    if(data != nullptr && samples)
    {
      APPCORE_ERROR("Multisample textures are not writable directly", alignment);
      return nullptr;
    }

    auto data_type = from_dtype(dtype, strlen(dtype));

    if(!data_type)
    {
      APPCORE_ERROR("Invalid data type got: '{0}'", dtype);
      return nullptr;
    }

    int texture_target = samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    int pixel_type = data_type->gl_type;
    int base_format = data_type->base_format[components];
    int internal_format = internal_format_override ? internal_format_override : data_type->internal_format[components];

    gl.ActiveTexture(GL_TEXTURE0 + m_default_texture_unit);

    auto texture = new Texture2D();
    texture->m_released = false;
    texture->m_context = this;
    texture->m_width = width;
    texture->m_height = height;
    texture->m_components = components;
    texture->m_samples = samples;
    texture->m_data_type = data_type;
    texture->m_max_level = 0;
    texture->m_compare_func = Texture2D::Func::NONE;
    texture->m_anisotropy = 1.0f;
    texture->m_depth = false;

    auto filter = data_type->float_type ? GL_LINEAR : GL_NEAREST;
    texture->m_filter = { filter, filter };

    texture->m_repeat_x = true;
    texture->m_repeat_y = true;
    texture->m_texture_obj = 0;

    gl.GenTextures(1, (GLuint*)&texture->m_texture_obj);

    if(!texture->m_texture_obj)
    {
      APPCORE_ERROR("cannot create texture");
      delete texture;
      return nullptr;
    }

    gl.BindTexture(texture_target, texture->m_texture_obj);

    if(samples)
    {
      gl.TexImage2DMultisample(texture_target, samples, internal_format, width, height, true);
    }
    else
    {
      gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
      gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
      gl.TexImage2D(texture_target, 0, internal_format, width, height, 0, base_format, pixel_type, data);
      if(data_type->float_type)
      {
        gl.TexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        gl.TexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      }
      else
      {
        gl.TexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        gl.TexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      }
    }

    return AppCore::Ref<Texture2D>(texture);
  }

  AppCore::Ref<Texture2D> Context::depth_texture2d(int width, int height, const void* data, int samples, int alignment)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    const GLMethods& gl = m_gl;

    if((samples & (samples - 1)) || samples > m_max_samples)
    {
      APPCORE_ERROR("The number of samples is invalid got: {0}", samples);
      return nullptr;
    }

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("The alignment must be 1, 2, 4 or 8, got: {0}", alignment);
      return nullptr;
    }

    if(data != nullptr && samples)
    {
      APPCORE_ERROR("Multisample textures are not writable directly", alignment);
      return nullptr;
    }

    auto texture = new Texture2D();
    texture->m_released = false;
    texture->m_context = this;
    texture->m_width = width;
    texture->m_height = height;
    texture->m_components = 1;
    texture->m_samples = samples;
    texture->m_data_type = from_dtype("f4", 2);
    texture->m_max_level = 0;
    texture->m_compare_func = Texture2D::Func::EQUAL;
    texture->m_anisotropy = 1.0f;
    texture->m_depth = true;
    texture->m_filter = { GL_LINEAR, GL_LINEAR };
    texture->m_repeat_x = false;
    texture->m_repeat_y = false;
    texture->m_texture_obj = 0;

    gl.ActiveTexture(GL_TEXTURE0 + m_default_texture_unit);
    gl.GenTextures(1, (GLuint*)&texture->m_texture_obj);

    if(!texture->m_texture_obj)
    {
      APPCORE_ERROR("cannot create texture");
      delete texture;
      return nullptr;
    }

    int texture_target = samples ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    int pixel_type = GL_FLOAT;

    gl.BindTexture(texture_target, texture->m_texture_obj);

    if(samples)
    {
      gl.TexImage2DMultisample(texture_target, samples, GL_DEPTH_COMPONENT24, width, height, true);
    }
    else
    {
      gl.TexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      gl.TexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
      gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
      gl.TexImage2D(texture_target, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, pixel_type, data);
      gl.TexParameteri(texture_target, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
      gl.TexParameteri(texture_target, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    }

    return AppCore::Ref<Texture2D>(texture);
  }

  AppCore::Ref<Texture3D>
  Context::texture3d(int width, int height, int depth, int components, const void* data, int alignment, const char* dtype)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    const GLMethods& gl = m_gl;

    if(components < 1 || components > 4)
    {
      APPCORE_ERROR("Components must be 1, 2, 3 or 4, got: {0}", components);
      return nullptr;
    }

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("The alignment must be 1, 2, 4 or 8, got: {0}", alignment);
      return nullptr;
    }

    auto data_type = from_dtype(dtype, strlen(dtype));

    if(!data_type)
    {
      APPCORE_ERROR("Invalid data type got: '{0}'", dtype);
      return nullptr;
    }

    auto texture = new Texture3D();
    texture->m_released = false;
    texture->m_context = this;
    texture->m_width = width;
    texture->m_height = height;
    texture->m_depth = depth;
    texture->m_components = components;
    texture->m_data_type = data_type;
    texture->m_max_level = 0;

    auto filter = data_type->float_type ? GL_LINEAR : GL_NEAREST;
    texture->m_filter = { filter, filter };

    texture->m_repeat_x = true;
    texture->m_repeat_y = true;
    texture->m_repeat_z = true;
    texture->m_texture_obj = 0;

    gl.ActiveTexture(GL_TEXTURE0 + m_default_texture_unit);
    gl.GenTextures(1, (GLuint*)&texture->m_texture_obj);

    if(!texture->m_texture_obj)
    {
      APPCORE_ERROR("cannot create texture");
      delete texture;
      return nullptr;
    }

    int pixel_type = data_type->gl_type;
    int base_format = data_type->base_format[components];
    int internal_format = data_type->internal_format[components];

    gl.BindTexture(GL_TEXTURE_3D, texture->m_texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexImage3D(GL_TEXTURE_3D, 0, internal_format, width, height, depth, 0, base_format, pixel_type, data);

    if(data_type->float_type)
    {
      gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
      gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      gl.TexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    return AppCore::Ref<Texture3D>(texture);
  }

  AppCore::Ref<TextureArray>
  Context::texture_array(int width, int height, int layers, int components, const void* data, int alignment, const char* dtype)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    const GLMethods& gl = m_gl;

    if(components < 1 || components > 4)
    {
      APPCORE_ERROR("Components must be 1, 2, 3 or 4, got: {0}", components);
      return nullptr;
    }

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("The alignment must be 1, 2, 4 or 8, got: {0}", alignment);
      return nullptr;
    }

    auto data_type = from_dtype(dtype, strlen(dtype));

    if(!data_type)
    {
      APPCORE_ERROR("Invalid data type got: '{0}'", dtype);
      return nullptr;
    }

    auto texture = new TextureArray();
    texture->m_released = false;
    texture->m_context = this;
    texture->m_width = width;
    texture->m_height = height;
    texture->m_layers = layers;
    texture->m_components = components;
    texture->m_data_type = data_type;
    texture->m_max_level = 0;

    auto filter = data_type->float_type ? GL_LINEAR : GL_NEAREST;
    texture->m_filter = { filter, filter };

    texture->m_repeat_x = true;
    texture->m_repeat_y = true;
    texture->m_texture_obj = 0;

    gl.ActiveTexture(GL_TEXTURE0 + m_default_texture_unit);
    gl.GenTextures(1, (GLuint*)&texture->m_texture_obj);

    if(!texture->m_texture_obj)
    {
      APPCORE_ERROR("cannot create texture");
      delete texture;
      return nullptr;
    }

    int pixel_type = data_type->gl_type;
    int base_format = data_type->base_format[components];
    int internal_format = data_type->internal_format[components];

    gl.BindTexture(GL_TEXTURE_2D_ARRAY, texture->m_texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexImage3D(GL_TEXTURE_2D_ARRAY, 0, internal_format, width, height, layers, 0, base_format, pixel_type, data);

    if(data_type->float_type)
    {
      gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
      gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      gl.TexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    return AppCore::Ref<TextureArray>(texture);
  }

  AppCore::Ref<TextureCube> Context::texture_cube(
      int width, int height, int components, const void* data, int alignment, const char* dtype, int internal_format_override)
  {

    APPCORE_ASSERT(!released(), "Context already released");
    const GLMethods& gl = m_gl;

    if(components < 1 || components > 4)
    {
      APPCORE_ERROR("Components must be 1, 2, 3 or 4, got: {0}", components);
      return nullptr;
    }

    if(alignment != 1 && alignment != 2 && alignment != 4 && alignment != 8)
    {
      APPCORE_ERROR("The alignment must be 1, 2, 4 or 8, got: {0}", alignment);
      return nullptr;
    }

    auto data_type = from_dtype(dtype, strlen(dtype));

    if(!data_type)
    {
      APPCORE_ERROR("Invalid data type got: '{0}'", dtype);
      return nullptr;
    }

    auto texture = new TextureCube();
    texture->m_released = false;
    texture->m_context = this;
    texture->m_width = width;
    texture->m_height = height;
    texture->m_components = components;
    texture->m_data_type = data_type;
    texture->m_max_level = 0;

    auto filter = data_type->float_type ? GL_LINEAR : GL_NEAREST;
    texture->m_filter = { filter, filter };

    texture->m_texture_obj = 0;

    gl.ActiveTexture(GL_TEXTURE0 + m_default_texture_unit);
    gl.GenTextures(1, (GLuint*)&texture->m_texture_obj);

    if(!texture->m_texture_obj)
    {
      APPCORE_ERROR("cannot create texture");
      delete texture;
      return nullptr;
    }

    int pixel_type = data_type->gl_type;
    int base_format = data_type->base_format[components];
    int internal_format = internal_format_override ? internal_format_override : data_type->internal_format[components];

    int expected_size = width * components * data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * height * 6;

    const char* ptr[6] = {
      (const char*)data + expected_size * 0 / 6, (const char*)data + expected_size * 1 / 6,
      (const char*)data + expected_size * 2 / 6, (const char*)data + expected_size * 3 / 6,
      (const char*)data + expected_size * 4 / 6, (const char*)data + expected_size * 5 / 6,
    };

    gl.BindTexture(GL_TEXTURE_CUBE_MAP, texture->m_texture_obj);

    gl.PixelStorei(GL_PACK_ALIGNMENT, alignment);
    gl.PixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    gl.TexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, internal_format, width, height, 0, base_format, pixel_type, ptr[0]);
    gl.TexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, internal_format, width, height, 0, base_format, pixel_type, ptr[1]);
    gl.TexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, internal_format, width, height, 0, base_format, pixel_type, ptr[2]);
    gl.TexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, internal_format, width, height, 0, base_format, pixel_type, ptr[3]);
    gl.TexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, internal_format, width, height, 0, base_format, pixel_type, ptr[4]);
    gl.TexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, internal_format, width, height, 0, base_format, pixel_type, ptr[5]);

    if(data_type->float_type)
    {
      gl.TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      gl.TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
      gl.TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      gl.TexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    return AppCore::Ref<TextureCube>(texture);
  }

  AppCore::Ref<VertexArray> Context::vertex_array(AppCore::Ref<Program> program,
                                                  AppGL::VertexDataArray vertex_data,
                                                  AppCore::Ref<Buffer> index_buffer,
                                                  int index_element_size,
                                                  bool skip_errors,
                                                  AppGL::RenderMode mode)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    const GLMethods& gl = m_gl;

    if(program->m_context != this)
    {
      APPCORE_ERROR("the program belongs to a different context");
      return nullptr;
    }

    if(index_buffer != nullptr && index_buffer->m_context != this)
    {
      APPCORE_ERROR("the index_buffer belongs to a different context");
      return nullptr;
    }

    int i = 0;
    for(auto&& v_data : vertex_data)
    {
      if(v_data.buffer == nullptr)
      {
        APPCORE_ERROR("vertex_data[{0}]: verempty vertex buffer", i);
        return nullptr;
      }

      if(v_data.buffer->m_context != this)
      {
        APPCORE_ERROR("vertex_data[{0}]: vertex buffer belongs to a different context", i);
        return nullptr;
      }

      FormatIterator it = FormatIterator(v_data.format);
      FormatInfo format_info = it.info();

      if(!format_info.valid)
      {
        APPCORE_ERROR("vertex_data[{0}]: invalid invalid format", i);
        return nullptr;
      }

      if(!v_data.attributes.size())
      {
        APPCORE_ERROR("vertex_data[{0}]: attributes must not be empty", i);
        return nullptr;
      }

      if((int)v_data.attributes.size() != format_info.nodes)
      {
        APPCORE_ERROR(
            "vertex_data[{0}]: format and attributes size mismatch {1} != {2}", i, format_info.nodes, v_data.attributes.size());
        return nullptr;
      }

      i++;
    }

    if(index_element_size != 1 && index_element_size != 2 && index_element_size != 4)
    {
      APPCORE_ERROR("index_element_size must be 1, 2, or 4, not %d", index_element_size);
      return nullptr;
    }

    auto array = new VertexArray();
    array->m_released = false;
    array->m_context = this;
    array->m_num_vertices = 0;
    array->m_num_instances = 1;
    array->m_program = program;
    array->m_index_buffer = index_buffer;
    array->m_index_element_size = index_element_size;
    const int element_types[5] = { 0, GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, 0, GL_UNSIGNED_INT };
    array->m_index_element_type = element_types[index_element_size];
    array->m_num_vertices = -1;

    array->m_vertex_array_obj = 0;
    gl.GenVertexArrays(1, (GLuint*)&array->m_vertex_array_obj);

    if(!array->m_vertex_array_obj)
    {
      APPCORE_ERROR("cannot create vertex array");
      delete array;
      return nullptr;
    }

    gl.BindVertexArray(array->m_vertex_array_obj);

    if(array->m_index_buffer != nullptr)
    {
      array->m_num_vertices = (int)(array->m_index_buffer->size() / index_element_size);
      gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, array->m_index_buffer->m_buffer_obj);
    }

    i = 0;
    for(auto&& v_data : vertex_data)
    {
      auto buffer = v_data.buffer;
      const char* format = v_data.format;

      FormatIterator it = FormatIterator(format);
      FormatInfo format_info = it.info();

      int buf_vertices = (int)(buffer->size() / format_info.size);

      if(!format_info.divisor && array->m_index_buffer == nullptr && (!i || array->m_num_vertices > buf_vertices))
      {
        array->m_num_vertices = buf_vertices;
      }

      gl.BindBuffer(GL_ARRAY_BUFFER, buffer->m_buffer_obj);

      char* ptr = 0;

      for(size_t j = 0; j < v_data.attributes.size(); ++j)
      {
        FormatNode* node = it.next();

        while(!node->type)
        {
          ptr += node->size;
          node = it.next();
        }

        auto attribute = array->m_program->attribute(v_data.attributes[j]);

        if(!attribute)
        {
          ptr += node->size;
          continue;
        }

        int attribute_location = attribute->m_location;
        int attribute_rows_length = attribute->m_data_type->rows_length;
        int attribute_scalar_type = attribute->m_data_type->scalar_type;

        for(int r = 0; r < attribute_rows_length; ++r)
        {
          int location = attribute_location + r;
          int count = node->count / attribute_rows_length;

          switch(attribute_scalar_type)
          {
            case GL_FLOAT: gl.VertexAttribPointer(location, count, node->type, node->normalize, format_info.size, ptr); break;
            case GL_DOUBLE: gl.VertexAttribLPointer(location, count, node->type, format_info.size, ptr); break;
            case GL_INT: gl.VertexAttribIPointer(location, count, node->type, format_info.size, ptr); break;
            case GL_UNSIGNED_INT: gl.VertexAttribIPointer(location, count, node->type, format_info.size, ptr); break;
          }

          gl.VertexAttribDivisor(location, format_info.divisor);

          gl.EnableVertexAttribArray(location);

          ptr += node->size / attribute_rows_length;
        }
      }
      i++;
    }

    array->m_num_subroutines = 0;
    array->m_subroutines = nullptr;
    array->m_num_subroutines += array->m_program->m_num_vertex_shader_subroutines;
    array->m_num_subroutines += array->m_program->m_num_fragment_shader_subroutines;
    array->m_num_subroutines += array->m_program->m_num_geometry_shader_subroutines;
    array->m_num_subroutines += array->m_program->m_num_tess_evaluation_shader_subroutines;
    array->m_num_subroutines += array->m_program->m_num_tess_control_shader_subroutines;

    if(array->m_num_subroutines)
    {
      array->m_subroutines = new unsigned[array->m_num_subroutines];
    }

    return AppCore::Ref<VertexArray>(array);
  }

  void Context::set_enable_flags(int flags)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    m_enable_flags = flags;

    if(flags & Context::EnableFlag::BLEND)
    {
      m_gl.Enable(GL_BLEND);
    }
    else
    {
      m_gl.Disable(GL_BLEND);
    }

    if(flags & Context::EnableFlag::DEPTH_TEST)
    {
      m_gl.Enable(GL_DEPTH_TEST);
    }
    else
    {
      m_gl.Disable(GL_DEPTH_TEST);
    }

    if(flags & Context::EnableFlag::CULL_FACE)
    {
      m_gl.Enable(GL_CULL_FACE);
    }
    else
    {
      m_gl.Disable(GL_CULL_FACE);
    }

    if(flags & Context::EnableFlag::RASTERIZER_DISCARD)
    {
      m_gl.Enable(GL_RASTERIZER_DISCARD);
    }
    else
    {
      m_gl.Disable(GL_RASTERIZER_DISCARD);
    }

    if(flags & Context::EnableFlag::PROGRAM_POINT_SIZE)
    {
      m_gl.Enable(GL_PROGRAM_POINT_SIZE);
    }
    else
    {
      m_gl.Disable(GL_PROGRAM_POINT_SIZE);
    }
  }

  void Context::enable(int flags)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    m_enable_flags |= flags;

    if(flags & Context::EnableFlag::BLEND)
    {
      m_gl.Enable(GL_BLEND);
    }

    if(flags & Context::EnableFlag::DEPTH_TEST)
    {
      m_gl.Enable(GL_DEPTH_TEST);
    }

    if(flags & Context::EnableFlag::CULL_FACE)
    {
      m_gl.Enable(GL_CULL_FACE);
    }

    if(flags & Context::EnableFlag::RASTERIZER_DISCARD)
    {
      m_gl.Enable(GL_RASTERIZER_DISCARD);
    }

    if(flags & Context::EnableFlag::PROGRAM_POINT_SIZE)
    {
      m_gl.Enable(GL_PROGRAM_POINT_SIZE);
    }
  }

  void Context::disable(int flags)
  {
    APPCORE_ASSERT(!released(), "Context already released");

    m_enable_flags &= ~flags;

    if(flags & Context::EnableFlag::BLEND)
    {
      m_gl.Disable(GL_BLEND);
    }

    if(flags & Context::EnableFlag::DEPTH_TEST)
    {
      m_gl.Disable(GL_DEPTH_TEST);
    }

    if(flags & Context::EnableFlag::CULL_FACE)
    {
      m_gl.Disable(GL_CULL_FACE);
    }

    if(flags & Context::EnableFlag::RASTERIZER_DISCARD)
    {
      m_gl.Disable(GL_RASTERIZER_DISCARD);
    }

    if(flags & Context::EnableFlag::PROGRAM_POINT_SIZE)
    {
      m_gl.Disable(GL_PROGRAM_POINT_SIZE);
    }
  }

  void Context::copy_buffer(
      const AppCore::Ref<Buffer>& src, const AppCore::Ref<Buffer>& dst, size_t size, size_t read_offset, size_t write_offset)
  {
    APPCORE_ASSERT(read_offset >= 0 && write_offset >= 0, "buffer underflow");
    APPCORE_ASSERT(!released(), "Context already released");

    if(size < 0)
    {
      size = src->size() - read_offset;
    }

    APPCORE_ASSERT((read_offset + size <= src->size() && write_offset + size <= dst->size()), "buffer overflow");

    m_gl.BindBuffer(GL_COPY_READ_BUFFER, src->m_buffer_obj);
    m_gl.BindBuffer(GL_COPY_WRITE_BUFFER, dst->m_buffer_obj);
    m_gl.CopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, read_offset, write_offset, size);
  }

  void Context::enable_direct(int value)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    m_gl.Enable(value);
  }

  void Context::disable_direct(int value)
  {
    APPCORE_ASSERT(!released(), "Context already released");
    m_gl.Disable(value);
  }

  void Context::finish()
  {
    APPCORE_ASSERT(!released(), "Context already released");
    m_gl.Finish();
  }

} // namespace AppGL
