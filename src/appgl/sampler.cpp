
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
#include "sampler.hpp"
#include "appcore/log.hpp"
#include "context.hpp"

namespace mgl
{
  void Sampler::release()
  {
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    if(m_released)
      return;

    m_released = true;
    gl.DeleteSamplers(1, (GLuint*)&m_sampler_obj);
  }

  void Sampler::use(int index)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();
    gl.BindSampler(index, m_sampler_obj);
  }

  void Sampler::clear(int index)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();
    gl.BindSampler(index, 0);
  }

  void Sampler::set_repeat_x(bool value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();
    m_repeat_x = value;

    if(value)
    {
      gl.SamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_S, GL_REPEAT);
      return;
    }

    gl.SamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  }

  void Sampler::set_repeat_y(bool value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();
    m_repeat_y = value;

    if(value)
    {
      gl.SamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_T, GL_REPEAT);
      return;
    }

    gl.SamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  void Sampler::set_repeat_z(bool value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();
    m_repeat_z = value;

    if(value)
    {
      gl.SamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_R, GL_REPEAT);
      return;
    }

    gl.SamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  }

  void Sampler::set_filter(const Sampler::Filter& value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();
    m_filter = value;
    gl.SamplerParameteri(m_sampler_obj, GL_TEXTURE_MIN_FILTER, m_filter.min_filter);
    gl.SamplerParameteri(m_sampler_obj, GL_TEXTURE_MAG_FILTER, m_filter.mag_filter);
  }

  void Sampler::set_compare_func(Sampler::func value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();
    m_compare_func = value;
    if(m_compare_func == Sampler::func::NONE)
    {
      gl.SamplerParameteri(m_sampler_obj, GL_TEXTURE_COMPARE_MODE, GL_NONE);
      return;
    }

    gl.SamplerParameteri(m_sampler_obj, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    gl.SamplerParameteri(m_sampler_obj, GL_TEXTURE_COMPARE_FUNC, m_compare_func);
  }

  void Sampler::set_anisotropy(float value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    if(m_context->max_anisotropy() == 0)
      return;

    m_anisotropy = (float)MGL_MIN(MGL_MAX(value, 1.0), m_context->max_anisotropy());
    gl.SamplerParameterf(m_sampler_obj, GL_TEXTURE_MAX_ANISOTROPY, m_anisotropy);
  }

  void Sampler::set_border_color(const glm::vec4& value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();
    m_border_color = value;
    gl.SamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    gl.SamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    gl.SamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    gl.SamplerParameterfv(m_sampler_obj, GL_TEXTURE_BORDER_COLOR, (GLfloat*)&m_border_color);
  }

  void Sampler::set_min_lod(float value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();
    m_min_lod = value;
    gl.SamplerParameterf(m_sampler_obj, GL_TEXTURE_MIN_LOD, m_min_lod);
  }

  void Sampler::set_max_lod(float value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();
    m_max_lod = value;
    gl.SamplerParameterf(m_sampler_obj, GL_TEXTURE_MAX_LOD, m_max_lod);
  }

} // namespace mgl
