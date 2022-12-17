
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
#include "datatype.hpp"
#include "framebuffer.hpp"
#include "renderbuffer.hpp"
#include "texture.hpp"

namespace AppGL
{
  void Context::LoadFunctions()
  {
    // Map OpenGL functions
    void** gl_function = (void**)&mGL;
    for(int i = 0; GL_FUNCTIONS[i]; ++i)
    {
      APPCORE_INFO("Loading GL function: {0}", GL_FUNCTIONS[i]);
      auto func = Load(GL_FUNCTIONS[i]);
      APPCORE_ASSERT(func != nullptr, "Loading GL function {0}", GL_FUNCTIONS[i]);
      gl_function[i] = func;
    }
  }

  AppCore::Ref<Context> Context::New(ContextMode::Enum mode, int glversion)
  {

    Context* ctx = nullptr;

#ifdef APPGL_EGL
    APPCORE_INFO("Trying EGL context!");
    ctx = new ContextEGL(mode, glversion);
    if(!ctx->IsValid())
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
      ctx = new ContextGLX(mode, glversion);
      if(!ctx->IsValid())
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
      ctx = new ContextWGL(mode, glversion);
      if(!ctx->IsValid())
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

    const GLMethods& gl = ctx->GL();

    ctx->mReleased = false;
    ctx->mWireframe = false;

    ctx->LoadFunctions();

    int major = 0;
    int minor = 0;

    gl.GetIntegerv(GL_MAJOR_VERSION, &major);
    gl.GetIntegerv(GL_MINOR_VERSION, &minor);

    APPCORE_INFO("GL Version: {0}.{1}", major, minor);

    ctx->mVersionCode = major * 100 + minor * 10;

    // Load extensions
    int num_extensions = 0;
    gl.GetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);

    for(int i = 0; i < num_extensions; i++)
    {
      const char* ext = (const char*)gl.GetStringi(GL_EXTENSIONS, i);
      APPCORE_INFO("Found GL extension: {0}", ext);
      ctx->mExtensions.push_front(ext);
    }

    gl.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    gl.Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    gl.Enable(GL_PRIMITIVE_RESTART);
    gl.PrimitiveRestartIndex(-1);

    ctx->mMaxSamples = 0;
    gl.GetIntegerv(GL_MAX_SAMPLES, (GLint*)&ctx->mMaxSamples);

    ctx->mMaxIntegerSamples = 0;
    gl.GetIntegerv(GL_MAX_INTEGER_SAMPLES, (GLint*)&ctx->mMaxIntegerSamples);

    ctx->mMaxColorAttachments = 0;
    gl.GetIntegerv(GL_MAX_COLOR_ATTACHMENTS, (GLint*)&ctx->mMaxColorAttachments);

    ctx->mMaxTextureUnits = 0;
    gl.GetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint*)&ctx->mMaxTextureUnits);
    ctx->mDefaultTextureUnit = ctx->mMaxTextureUnits - 1;

    ctx->mMaxAnisotropy = 0.0;
    gl.GetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, (GLfloat*)&ctx->mMaxAnisotropy);

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
      gl.FrameBufferRenderbuffer(
          GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
      bound_framebuffer = framebuffer;
    }
#endif
    {
      auto fb = new FrameBuffer();

      fb->mReleased = false;
      fb->mGLObject = 0;

      fb->mDrawBuffersLen = 1;
      fb->mDrawBuffers = new unsigned[fb->mDrawBuffersLen];

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
      gl.GetIntegerv(GL_DRAW_BUFFER, (int*)&fb->mDrawBuffers[0]);
      gl.BindFramebuffer(GL_FRAMEBUFFER, bound_framebuffer);

      fb->mColorMask = new bool[4];
      fb->mColorMask[0] = true;
      fb->mColorMask[1] = true;
      fb->mColorMask[2] = true;
      fb->mColorMask[3] = true;

      fb->mDepthMask = true;
      fb->mContext = ctx;

      int scissor_box[4] = {};
      gl.GetIntegerv(GL_SCISSOR_BOX, scissor_box);

      Rect r = {scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3]};
      fb->mScissorEnabled = false;
      fb->mViewport = r;
      fb->mScissor = r;

      fb->mWidth = r.W;
      fb->mHeight = r.H;
      fb->mDynamic = true;

      ctx->mDefaultFrameBuffer = AppCore::Ref<FrameBuffer>(fb);
    }

    ctx->mBoundFrameBuffer = ctx->mDefaultFrameBuffer;

    ctx->mEnableFlags = 0;
    ctx->mFrontFace = GL_CCW;

    ctx->mDepthFunc = GL_LEQUAL;
    ctx->mBlendFuncSrc = GL_SRC_ALPHA;
    ctx->mBlendFuncDst = GL_ONE_MINUS_SRC_ALPHA;

    ctx->mWireframe = false;
    ctx->mMultisample = true;

    ctx->mProvokingVertex = GL_LAST_VERTEX_CONVENTION;

    ctx->mPolygonOffsetFactor = 0.0f;
    ctx->mPolygonOffsetUnits = 0.0f;

    gl.GetError(); // clear errors

    return AppCore::Ref<Context>(ctx);
  }

  AppCore::Ref<Buffer> Context::NewBuffer(const uint8_t* data, size_t length, bool dynamic)
  {

    if(!length)
    {
      APPCORE_ERROR("Missing data length");
      return nullptr;
    }

    Buffer* buffer = new Buffer();
    buffer->m_released = false;

    buffer->m_size = length;
    buffer->m_dynamic = dynamic;

    const GLMethods& gl = mGL;

    buffer->m_buffer_obj = 0;
    gl.GenBuffers(1, (GLuint*)&buffer->m_buffer_obj);

    if(!buffer->m_buffer_obj)
    {
      APPCORE_ERROR("Cannot create buffer");
      delete(buffer);
      return nullptr;
    }

    gl.BindBuffer(GL_ARRAY_BUFFER, buffer->m_buffer_obj);
    gl.BufferData(GL_ARRAY_BUFFER, length, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

    buffer->m_context = this;

    return AppCore::Ref<Buffer>(buffer);
  }

  AppCore::Ref<FrameBuffer> Context::NewFrameBuffer(const AppCore::List<Texture> color_attachments,
                                                    const Texture& depth_attachment)
  {
    return nullptr;
  }

  AppCore::Ref<FrameBuffer> Context::NewFrameBuffer(const AppCore::List<Texture> color_attachments,
                                                    const RenderBuffer& depth_attachment)
  {
    return nullptr;
  }

  AppCore::Ref<FrameBuffer>
  Context::NewFrameBuffer(const AppCore::List<RenderBuffer> color_attachments,
                          const Texture& depth_attachment)
  {
    return nullptr;
  }

  AppCore::Ref<FrameBuffer>
  Context::NewFrameBuffer(const AppCore::List<RenderBuffer> color_attachments,
                          const RenderBuffer& depth_attachment)
  {
    return nullptr;
  }

  AppCore::Ref<RenderBuffer> Context::NewRenderBuffer(uint32_t w,
                                                      uint32_t h,
                                                      uint8_t components,
                                                      uint8_t samples,
                                                      const char* dtype,
                                                      size_t dtype_size)
  {

    if(components < 1 || components > 4)
    {
      APPCORE_ERROR("Components must be 1, 2, 3 or 4, got: {0}", components);
      return nullptr;
    }

    if((samples & (samples - 1)) || samples > mMaxSamples)
    {
      APPCORE_ERROR("The number of samples is invalid: {0}", samples);
      return nullptr;
    }

    DataType* dataType = FromDType(dtype, dtype_size);

    if(!dataType)
    {
      APPCORE_ERROR("Invalid data type: '{0}'", dtype);
      return nullptr;
    }

    int format = dataType->InternalFormat[components];

    const GLMethods& gl = mGL;

    RenderBuffer* renderbuffer = new RenderBuffer();
    renderbuffer->mReleased = false;

    renderbuffer->mGLObject = 0;
    gl.GenRenderbuffers(1, (GLuint*)&renderbuffer->mGLObject);

    if(!renderbuffer->mGLObject)
    {
      APPCORE_ERROR("Cannot create RenderBuffer");
      delete(renderbuffer);
      return nullptr;
    }

    gl.BindRenderbuffer(GL_RENDERBUFFER, renderbuffer->mGLObject);

    if(samples == 0)
    {
      gl.RenderbufferStorage(GL_RENDERBUFFER, format, w, h);
    }
    else
    {
      gl.RenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, w, h);
    }

    renderbuffer->mWidth = w;
    renderbuffer->mHeight = h;
    renderbuffer->mComponents = components;
    renderbuffer->mSamples = samples;
    renderbuffer->mDataType = dataType;
    renderbuffer->mDepth = false;

    renderbuffer->mContext = this;

    return AppCore::Ref<RenderBuffer>(renderbuffer);
  }

} // namespace AppGL
