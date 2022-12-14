
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

  AppCore::Ref<Context> Context::Create(ContextMode::Enum mode, int glversion)
  {

    AppCore::Ref<Context> ctx = nullptr;

#ifdef APPGL_EGL
    APPCORE_INFO("Trying EGL context!");
    ctx = AppCore::CreateRef<ContextEGL>(mode, glversion);
    if(!ctx->IsValid())
    {
      APPCORE_INFO("EGL not supported!");
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
      ctx = AppCore::CreateRef<ContextGLX>(mode, glversion);
      if(!ctx->IsValid())
      {
        APPCORE_INFO("GLX not supported!");
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
      ctx = AppCore::CreateRef<ContextWGL>(mode, glversion);
      if(!ctx->IsValid())
      {
        APPCORE_INFO("WGL not supported!");
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

    ctx->LoadFunctions();

    int major = 0;
    int minor = 0;

    ctx->GL().GetIntegerv(GL_MAJOR_VERSION, &major);
    ctx->GL().GetIntegerv(GL_MINOR_VERSION, &minor);

    APPCORE_INFO("GL Version: {0}.{1}", major, minor);

    ctx->mVersionCode = major * 100 + minor * 10;

    // Load extensions
    int num_extensions = 0;
    ctx->GL().GetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);

    for(int i = 0; i < num_extensions; i++)
    {
      const char* ext = (const char*)ctx->GL().GetStringi(GL_EXTENSIONS, i);
      APPCORE_INFO("Found GL extension: {0}", ext);
      ctx->mExtensions.push_front(ext);
    }

    ctx->GL().BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ctx->GL().Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    ctx->GL().Enable(GL_PRIMITIVE_RESTART);
    ctx->GL().PrimitiveRestartIndex(-1);

    ctx->mMaxSamples = 0;
    ctx->GL().GetIntegerv(GL_MAX_SAMPLES, (GLint*)&ctx->mMaxSamples);

    ctx->mMaxIntegerSamples = 0;
    ctx->GL().GetIntegerv(GL_MAX_INTEGER_SAMPLES, (GLint*)&ctx->mMaxIntegerSamples);

    ctx->mMaxColorAttachments = 0;
    ctx->GL().GetIntegerv(GL_MAX_COLOR_ATTACHMENTS, (GLint*)&ctx->mMaxColorAttachments);

    ctx->mMaxTextureUnits = 0;
    ctx->GL().GetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint*)&ctx->mMaxTextureUnits);
    ctx->mDefaultTextureUnit = ctx->mMaxTextureUnits - 1;

    ctx->mMaxAnisotropy = 0.0;
    ctx->GL().GetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, (GLfloat*)&ctx->mMaxAnisotropy);

    ctx->mDefaultFrameBuffer = AppCore::CreateRef<FrameBuffer>(ctx);

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

    ctx->GL().GetError(); // clear errors

    return ctx;
  }

  AppCore::Ref<FrameBuffer>
  Context::CreateFramebuffer(const AppCore::List<Texture> color_attachments,
                             const Texture& depth_attachment)
  {
    return nullptr;
  }

  AppCore::Ref<FrameBuffer>
  Context::CreateFramebuffer(const AppCore::List<Texture> color_attachments,
                             const RenderBuffer& depth_attachment)
  {
    return nullptr;
  }

  AppCore::Ref<FrameBuffer>
  Context::CreateFramebuffer(const AppCore::List<RenderBuffer> color_attachments,
                             const Texture& depth_attachment)
  {
    return nullptr;
  }

  AppCore::Ref<FrameBuffer>
  Context::CreateFramebuffer(const AppCore::List<RenderBuffer> color_attachments,
                             const RenderBuffer& depth_attachment)
  {
    return nullptr;
  }

} // namespace AppGL
