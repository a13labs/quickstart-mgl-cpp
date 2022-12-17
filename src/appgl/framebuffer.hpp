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
    ~FrameBuffer() { Release(); }

    void Clear(float r, float g, float b, float a, float depth, const Rect& rect);
    void Use();
    AppCore::Ref<uint8_t> Read(const Rect& v,
                               int c,
                               int att,
                               int align,
                               int clmp,
                               const char* dtype,
                               size_t dsize,
                               int w_offset);
    void ReadInto(AppCore::Ref<uint8_t> buffer,
                  const Rect& v,
                  int c,
                  int att,
                  int align,
                  int clmp,
                  const char* dtype,
                  size_t dsize,
                  int w_offset);
    void Release();

    inline const Rect& Viewport() { return mViewport; }
    inline void SetViewport(const Rect& r) { mViewport = r; }

    inline const Rect& Scissor() { return mScissor; }
    inline void SetScissor(const Rect& r) { mScissor = r; }

    inline void EnableScissor() { mScissorEnabled = true; }
    inline void DisableScissor() { mScissorEnabled = false; }

    inline void Clear(float r, float g, float b, float a, float depth)
    {
      Clear(r, g, b, a, depth, {0, 0, mWidth, mHeight});
    }

    inline void Clear(float r, float g, float b, float a, float depth, int w, int h)
    {
      Clear(r, g, b, a, depth, {0, 0, w, h});
    }

    inline void Clear(const glm::vec4& color, float depth)
    {
      Clear(color.r, color.g, color.b, color.a, depth, {0, 0, mWidth, mHeight});
    }

    inline void Clear(const glm::vec4& color, float depth, int w, int h)
    {
      Clear(color.r, color.g, color.b, color.a, depth, {0, 0, w, h});
    }

    inline void Clear(const glm::vec4& color, float depth, const Rect& rect)
    {
      Clear(color.r, color.g, color.b, color.a, depth, rect);
    }

private:
    friend class Context;
    FrameBuffer(){};

    Context* mContext;
    Rect mViewport;
    bool mScissorEnabled;
    Rect mScissor;
    bool* mColorMask;
    int mDrawBuffersLen;
    unsigned* mDrawBuffers;
    int mGLObject;

    // Flags this as a detected framebuffer we don't control the size of
    bool mDynamic;
    int mWidth;
    int mHeight;

    int mSamples;
    bool mDepthMask;
    bool mReleased;
  };
} // namespace AppGL