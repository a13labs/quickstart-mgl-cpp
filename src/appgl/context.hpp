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
    virtual GLFunction Load(const AppCore::String& method) = 0;
    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual void Release() = 0;
    virtual bool IsValid() = 0;

    inline ContextMode::Enum Mode() { return mMode; };
    inline const GLMethods& GL() const { return mGL; }
    inline bool Released() { return mReleased; };
    inline int VersionCode() { return mVersionCode; };
    inline int MaxSamples() { return mMaxSamples; }
    inline int MaxIntegerSamples() { return mMaxIntegerSamples; }
    inline int MaxColorAttachments() { return mMaxColorAttachments; }
    inline int MaxTextureUnits() { return mMaxTextureUnits; }
    inline int DefaultTextureUnit() { return mDefaultTextureUnit; }
    inline float MaxAnisotropy() { return mMaxAnisotropy; }
    inline const AppCore::StringList& Extensions() const { return mExtensions; }

    inline const AppCore::Ref<FrameBuffer> DefaultFrameBuffer() const
    {
      return mDefaultFrameBuffer;
    }

    inline int EnableFlags() { return mEnableFlags; }
    inline int FrontFace() { return mFrontFace; }
    inline int CullFace() { return mCullFace; }
    inline int DepthFunc() { return mDepthFunc; }
    inline int BlendFuncSrc() { return mBlendFuncSrc; }
    inline int BlendFuncDst() { return mBlendFuncDst; }
    inline bool Wireframe() { return mWireframe; }
    inline bool Multisample() { return mMultisample; }
    inline int ProvokingVertex() { return mProvokingVertex; }
    inline float PolygonOffsetFactor() { return mPolygonOffsetFactor; }
    inline float PolygonOffsetUnits() { return mPolygonOffsetUnits; }

    static AppCore::Ref<Context> New(ContextMode::Enum mode, int glversion);

    AppCore::Ref<Buffer> NewBuffer(const uint8_t* data, size_t length, bool dynamic);

    AppCore::Ref<FrameBuffer> NewFrameBuffer(const AppCore::List<Texture> color_attachments,
                                             const Texture& depth_attachment);
    AppCore::Ref<FrameBuffer> NewFrameBuffer(const AppCore::List<Texture> color_attachments,
                                             const RenderBuffer& depth_attachment);
    AppCore::Ref<FrameBuffer> NewFrameBuffer(const AppCore::List<RenderBuffer> color_attachments,
                                             const Texture& depth_attachment);
    AppCore::Ref<FrameBuffer> NewFrameBuffer(const AppCore::List<RenderBuffer> color_attachments,
                                             const RenderBuffer& depth_attachment);

    AppCore::Ref<RenderBuffer> NewRenderBuffer(uint32_t w,
                                               uint32_t h,
                                               uint8_t components,
                                               uint8_t samples,
                                               const char* dtype,
                                               size_t dtype_size);

private:
    void LoadFunctions();

protected:
    ContextMode::Enum mMode;
    bool mReleased;

private:
    friend class FrameBuffer;

    GLMethods mGL;
    int mVersionCode;
    int mMaxSamples;
    int mMaxIntegerSamples;
    int mMaxColorAttachments;
    int mMaxTextureUnits;
    int mDefaultTextureUnit;
    float mMaxAnisotropy;
    int mEnableFlags;
    int mFrontFace;
    int mCullFace;
    int mDepthFunc;
    int mBlendFuncSrc;
    int mBlendFuncDst;
    bool mWireframe;
    bool mMultisample;
    int mProvokingVertex;
    float mPolygonOffsetFactor;
    float mPolygonOffsetUnits;

    AppCore::StringList mExtensions;
    AppCore::Ref<FrameBuffer> mDefaultFrameBuffer;
    AppCore::Ref<FrameBuffer> mBoundFrameBuffer;
  };

#ifdef APPGL_EGL
  class ContextEGL : public Context
  {

public:
    ContextEGL(ContextMode::Enum mode, int glversion);
    virtual ~ContextEGL() override;

public:
    virtual GLFunction Load(const AppCore::String& method) override;
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void Release() override;
    virtual bool IsValid() override;

private:
    GLContext mContext;
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
    virtual GLFunction Load(const AppCore::String& method) override;
    virtual void Enter() override;
    virtual void Exit() override;
    virtual void Release() override;
    virtual bool IsValid() override;

private:
    GLContext mContext;
  };
#endif

#ifdef APPGL_WGL

#endif

} // namespace AppGL
