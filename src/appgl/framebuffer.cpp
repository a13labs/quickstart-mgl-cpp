
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
#include "framebuffer.hpp"
#include "appcore/log.hpp"
#include "context.hpp"

namespace AppGL
{
  void FrameBuffer::Release()
  {
    const GLMethods& gl = mContext->GL();

    if(mReleased)
    {
      return;
    }

    mReleased = true;

    if(mGLObject)
    {
      gl.DeleteFramebuffers(1, (GLuint*)&mGLObject);
      delete[] mDrawBuffers;
      delete[] mColorMask;
    }
  }

  void FrameBuffer::Clear(float r, float g, float b, float a, float depth, const Rect& rect)
  {
    if(mReleased)
      return;

    const GLMethods& gl = mContext->GL();

    gl.BindFramebuffer(GL_FRAMEBUFFER, mGLObject);

    if(mGLObject)
    {
      gl.DrawBuffers(mDrawBuffersLen, mDrawBuffers);
    }

    gl.ClearColor(r, g, b, a);
    gl.ClearDepth(depth);

    for(int i = 0; i < mDrawBuffersLen; ++i)
    {
      gl.ColorMaski(i,
                    mColorMask[i * 4 + 0],
                    mColorMask[i * 4 + 1],
                    mColorMask[i * 4 + 2],
                    mColorMask[i * 4 + 3]);
    }

    gl.DepthMask(mDepthMask);

    bool viewport = !(rect.X == 0 && rect.Y == 0 && rect.W == mWidth && rect.H == mHeight);

    // Respect the passed in viewport even with scissor enabled
    if(viewport)
    {
      gl.Enable(GL_SCISSOR_TEST);
      gl.Scissor(rect.X, rect.Y, rect.W, rect.H);
      gl.Clear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

      // restore scissor if enabled
      if(mScissorEnabled)
      {
        gl.Scissor(mScissor.X, mScissor.Y, mScissor.W, mScissor.H);
      }
      else
      {
        gl.Disable(GL_SCISSOR_TEST);
      }
    }
    else
    {
      // clear with scissor if enabled
      if(mScissorEnabled)
      {
        gl.Enable(GL_SCISSOR_TEST);
        gl.Scissor(mScissor.X, mScissor.Y, mScissor.W, mScissor.H);
      }
      gl.Clear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }

    gl.BindFramebuffer(GL_FRAMEBUFFER, mContext->mBoundFrameBuffer->mGLObject);
  }

  void FrameBuffer::Use()
  {
    if(mReleased)
      return;

    const GLMethods& gl = mContext->GL();

    gl.BindFramebuffer(GL_FRAMEBUFFER, mGLObject);

    if(mGLObject)
    {
      gl.DrawBuffers(mDrawBuffersLen, mDrawBuffers);
    }

    gl.Viewport(mViewport.X, mViewport.Y, mViewport.W, mViewport.H);

    if(mScissorEnabled)
    {
      gl.Enable(GL_SCISSOR_TEST);
      gl.Scissor(mScissor.X, mScissor.Y, mScissor.W, mScissor.H);
    }
    else
    {
      gl.Disable(GL_SCISSOR_TEST);
    }

    for(int i = 0; i < mDrawBuffersLen; ++i)
    {
      gl.ColorMaski(i,
                    mColorMask[i * 4 + 0],
                    mColorMask[i * 4 + 1],
                    mColorMask[i * 4 + 2],
                    mColorMask[i * 4 + 3]);
    }

    gl.DepthMask(mDepthMask);

    mContext->mBoundFrameBuffer = MakeThisRef();
  }

  AppCore::Ref<uint8_t> FrameBuffer::Read(const Rect& v,
                                          int c,
                                          int att,
                                          int align,
                                          int clmp,
                                          const char* dtype,
                                          size_t dsize,
                                          int w_offset)
  {
    return nullptr;
  }

  void FrameBuffer::ReadInto(AppCore::Ref<uint8_t> buffer,
                             const Rect& v,
                             int c,
                             int att,
                             int align,
                             int clmp,
                             const char* dtype,
                             size_t dsize,
                             int w_offset)
  { }
} // namespace AppGL
