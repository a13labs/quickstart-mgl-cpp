
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
  FrameBuffer::FrameBuffer(const AppCore::Ref<Context>& ctx)
  {

    const GLMethods& gl = ctx->GL();

    mContext = ctx;
    mReleased = false;

    int bound_framebuffer = 0;
    gl.GetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &bound_framebuffer);

#ifdef APP_OSX

    if(ctx->Mode() == ContextMode::Standalone)
    {
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

    mFrameBufferObj = 0;

    mDrawBuffersLen = 1;
    mDrawBuffers = new unsigned[mDrawBuffersLen];

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
    gl.GetIntegerv(GL_DRAW_BUFFER, (int*)&mDrawBuffers[0]);
    gl.BindFramebuffer(GL_FRAMEBUFFER, bound_framebuffer);

    mColorMask = new bool[4];
    mColorMask[0] = true;
    mColorMask[1] = true;
    mColorMask[2] = true;
    mColorMask[3] = true;

    mDepthMask = true;

    int scissor_box[4] = {};
    gl.GetIntegerv(GL_SCISSOR_BOX, scissor_box);

    DisableScissor();
    SetViewport(scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3]);
    SetScissor(scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3]);

    mWidth = scissor_box[2];
    mHeight = scissor_box[3];
    mDynamic = true;
  }

  FrameBuffer::FrameBuffer(const AppCore::Ref<Context>& ctx,
                           const AppCore::List<Texture> color_attachments,
                           const Texture& depth_attachment)
  { }

  FrameBuffer::FrameBuffer(const AppCore::Ref<Context>& ctx,
                           const AppCore::List<Texture> color_attachments,
                           const RenderBuffer& depth_attachment)
  { }

  FrameBuffer::FrameBuffer(const AppCore::Ref<Context>& ctx,
                           const AppCore::List<RenderBuffer> color_attachments,
                           const Texture& depth_attachment)
  { }

  FrameBuffer::FrameBuffer(const AppCore::Ref<Context>& ctx,
                           const AppCore::List<RenderBuffer> color_attachments,
                           const RenderBuffer& depth_attachment)
  { }

  FrameBuffer::~FrameBuffer()
  {
    if(!mReleased)
      Release();
  }

  void FrameBuffer::Release()
  {
    const GLMethods& gl = mContext->GL();

    if(mReleased)
    {
      return;
    }

    mReleased = true;

    if(mFrameBufferObj)
    {
      gl.DeleteFramebuffers(1, (GLuint*)&mFrameBufferObj);
      delete[] mDrawBuffers;
      delete[] mColorMask;
    }
  }

  void
  FrameBuffer::Clear(float r, float g, float b, float a, float depth, int x, int y, int w, int h)
  {
    if(mReleased)
      return;

    const GLMethods& gl = mContext->GL();

    gl.BindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObj);

    if(mFrameBufferObj)
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

    bool viewport = !(x == 0 && y == 0 && w == mWidth && h == mHeight);

    // Respect the passed in viewport even with scissor enabled
    if(viewport)
    {
      gl.Enable(GL_SCISSOR_TEST);
      gl.Scissor(x, y, w, h);
      gl.Clear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

      // restore scissor if enabled
      if(mScissorEnabled)
      {
        gl.Scissor(mScissorX, mScissorY, mScissorWidth, mScissorHeight);
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
        gl.Scissor(mScissorX, mScissorY, mScissorWidth, mScissorHeight);
      }
      gl.Clear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }

    gl.BindFramebuffer(GL_FRAMEBUFFER, mContext->BoundFrameBuffer()->mFrameBufferObj);
  }

  void FrameBuffer::Use()
  {
    if(mReleased)
      return;

    const GLMethods& gl = mContext->GL();

    gl.BindFramebuffer(GL_FRAMEBUFFER, mFrameBufferObj);

    if(mFrameBufferObj)
    {
      gl.DrawBuffers(mDrawBuffersLen, mDrawBuffers);
    }

    gl.Viewport(mViewportX, mViewportY, mViewportWidth, mViewportHeight);

    if(mScissorEnabled)
    {
      gl.Enable(GL_SCISSOR_TEST);
      gl.Scissor(mScissorX, mScissorY, mScissorWidth, mScissorHeight);
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

    mContext->SetBoundFrameBuffer(MakeThisRef());
  }
} // namespace AppGL
