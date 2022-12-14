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
#include "appcore/appcore.hpp"
#include "appgl.hpp"
#include "glm/glm.hpp"

namespace AppGL
{
  class FrameBuffer : public AppCore::RefFromThis<FrameBuffer>
  {
public:
    FrameBuffer(const AppCore::Ref<Context>& ctx);

    FrameBuffer(const AppCore::Ref<Context>& ctx,
                const AppCore::List<Texture> color_attachments,
                const Texture& depth_attachment);

    FrameBuffer(const AppCore::Ref<Context>& ctx,
                const AppCore::List<Texture> color_attachments,
                const RenderBuffer& depth_attachment);

    FrameBuffer(const AppCore::Ref<Context>& ctx,
                const AppCore::List<RenderBuffer> color_attachments,
                const Texture& depth_attachment);

    FrameBuffer(const AppCore::Ref<Context>& ctx,
                const AppCore::List<RenderBuffer> color_attachments,
                const RenderBuffer& depth_attachment);

    ~FrameBuffer();

    inline void SetViewport(int x, int y, int w, int h)
    {
      mViewportX = x;
      mViewportY = y;
      mViewportWidth = w;
      mViewportHeight = h;
    }

    inline void SetScissor(int x, int y, int w, int h)
    {
      mScissorX = x;
      mScissorY = y;
      mScissorWidth = w;
      mScissorHeight = h;
    }

    inline void EnableScissor() { mScissorEnabled = true; }
    inline void DisableScissor() { mScissorEnabled = false; }

    inline int ViewportX() { return mViewportX; }
    inline int ViewportY() { return mViewportY; }
    inline int ViewportWidth() { return mViewportWidth; }
    inline int ViewportHeight() { return mViewportHeight; }

    inline int ScissorX() { return mScissorX; }
    inline int ScissorY() { return mScissorY; }
    inline int ScissorWidth() { return mScissorWidth; }
    inline int ScissorHeight() { return mScissorHeight; }

    void Release();

    inline void Clear(float r, float g, float b, float a, float depth)
    {
      Clear(r, g, b, a, depth, 0, 0, mWidth, mHeight);
    }

    inline void Clear(float r, float g, float b, float a, float depth, int w, int h)
    {
      Clear(r, g, b, a, depth, 0, 0, w, h);
    }

    inline void Clear(const glm::vec4& color, float depth)
    {
      Clear(color.r, color.g, color.b, color.a, depth, 0, 0, mWidth, mHeight);
    }

    inline void Clear(const glm::vec4& color, float depth, int w, int h)
    {
      Clear(color.r, color.g, color.b, color.a, depth, 0, 0, w, h);
    }

    inline void Clear(const glm::vec4& color, float depth, int x, int y, int w, int h)
    {
      Clear(color.r, color.g, color.b, color.a, depth, x, y, w, h);
    }

    void Clear(float r, float g, float b, float a, float depth, int x, int y, int w, int h);
    void Use();

private:
    AppCore::Ref<Context> mContext;

    int mViewportX;
    int mViewportY;
    int mViewportWidth;
    int mViewportHeight;

    bool mScissorEnabled;
    int mScissorX;
    int mScissorY;
    int mScissorWidth;
    int mScissorHeight;

    bool* mColorMask;

    int mDrawBuffersLen;
    unsigned* mDrawBuffers;

    int mFrameBufferObj;

    // Flags this as a detected framebuffer we don't control the size of
    bool mDynamic;
    int mWidth;
    int mHeight;

    int mSamples;
    bool mDepthMask;
    bool mReleased;
  };
} // namespace AppGL