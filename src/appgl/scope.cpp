
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
#include "scope.hpp"
#include "appcore/log.hpp"
#include "context.hpp"
#include "framebuffer.hpp"
#include "sampler.hpp"

namespace mgl
{
  void Scope::release()
  {
    if(m_released)
    {
      return;
    }

    m_released = true;
    m_framebuffer = nullptr;
    m_old_framebuffer = nullptr;
    m_context = nullptr;
    m_samplers.clear();
    m_textures.clear();
    m_buffers.clear();
  }

  void Scope::begin()
  {
    MGL_CORE_ASSERT(!m_released, "Scope released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    const int& flags = m_enable_flags;

    m_old_enable_flags = m_context->enable_flags();
    m_context->set_enable_flags(m_enable_flags);

    m_framebuffer->use();

    for(auto&& texture : m_textures)
    {
      gl.ActiveTexture(texture.binding);
      gl.BindTexture(texture.type, texture.gl_object);
    }

    for(auto&& buffer : m_buffers)
    {
      gl.BindBufferBase(buffer.type, buffer.binding, buffer.gl_object);
    }

    for(auto&& sampler : m_samplers)
    {
      MGL_CORE_ASSERT(sampler.sampler, "invalid sampler");
      sampler.sampler->use(sampler.binding);
    }

    if(flags & Context::flag::BLEND)
    {
      gl.Enable(GL_BLEND);
    }
    else
    {
      gl.Disable(GL_BLEND);
    }

    if(flags & Context::flag::DEPTH_TEST)
    {
      gl.Enable(GL_DEPTH_TEST);
    }
    else
    {
      gl.Disable(GL_DEPTH_TEST);
    }

    if(flags & Context::flag::CULL_FACE)
    {
      gl.Enable(GL_CULL_FACE);
    }
    else
    {
      gl.Disable(GL_CULL_FACE);
    }

    if(flags & Context::flag::RASTERIZER_DISCARD)
    {
      gl.Enable(GL_RASTERIZER_DISCARD);
    }
    else
    {
      gl.Disable(GL_RASTERIZER_DISCARD);
    }

    if(flags & Context::flag::PROGRAM_POINT_SIZE)
    {
      gl.Enable(GL_PROGRAM_POINT_SIZE);
    }
    else
    {
      gl.Disable(GL_PROGRAM_POINT_SIZE);
    }
  }

  void Scope::end()
  {
    MGL_CORE_ASSERT(!m_released, "Scope released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();
    const int& flags = m_old_enable_flags;

    m_context->set_enable_flags(m_old_enable_flags);

    m_old_framebuffer->use();

    if(flags & Context::flag::BLEND)
    {
      gl.Enable(GL_BLEND);
    }
    else
    {
      gl.Disable(GL_BLEND);
    }

    if(flags & Context::flag::DEPTH_TEST)
    {
      gl.Enable(GL_DEPTH_TEST);
    }
    else
    {
      gl.Disable(GL_DEPTH_TEST);
    }

    if(flags & Context::flag::CULL_FACE)
    {
      gl.Enable(GL_CULL_FACE);
    }
    else
    {
      gl.Disable(GL_CULL_FACE);
    }

    if(flags & Context::flag::RASTERIZER_DISCARD)
    {
      gl.Enable(GL_RASTERIZER_DISCARD);
    }
    else
    {
      gl.Disable(GL_RASTERIZER_DISCARD);
    }

    if(flags & Context::flag::PROGRAM_POINT_SIZE)
    {
      gl.Enable(GL_PROGRAM_POINT_SIZE);
    }
    else
    {
      gl.Disable(GL_PROGRAM_POINT_SIZE);
    }
  }

} // namespace mgl
