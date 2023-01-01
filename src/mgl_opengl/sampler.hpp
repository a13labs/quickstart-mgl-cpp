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
#pragma once
#include "builtins.hpp"
#include "glm/vec4.hpp"

namespace mgl_opengl
{
  class sampler
  {
public:
    struct Filter
    {
      int min_filter;
      int mag_filter;
    };

    ~sampler() = default;

public:
    void use(int index = 0);
    void clear(int index = 0);

    void release();
    bool released();

    bool repeat_x();
    void set_repeat_x(bool value);

    bool repeat_y();
    void set_repeat_y(bool value);

    bool repeat_z();
    void set_repeat_z(bool value);

    const sampler::Filter& filter() const;
    void set_filter(const sampler::Filter& value);

    mgl_opengl::compare_func compare_func();
    void set_compare_func(mgl_opengl::compare_func value);

    float anisotropy();
    void set_anisotropy(float value);

    const glm::vec4& border_color() const;
    void set_border_color(const glm::vec4& value);
    void set_border_color(float r, float g, float b, float a);

    float min_lod();
    void set_min_lod(float value);

    float max_lod();
    void set_max_lod(float value);

    int glo();

private:
    friend class context;
    sampler() = default;

    context* m_context;
    int m_sampler_obj;
    float m_anisotropy;
    mgl_opengl::compare_func m_compare_func;
    bool m_repeat_x;
    bool m_repeat_y;
    bool m_repeat_z;
    glm::vec4 m_border_color;
    float m_min_lod;
    float m_max_lod;
    bool m_released;
    sampler::Filter m_filter;
  };

  inline int sampler::glo()
  {
    return m_sampler_obj;
  }

  inline bool sampler::released()
  {
    return m_released;
  }

  inline bool sampler::repeat_x()
  {
    return m_repeat_x;
  }

  inline bool sampler::repeat_y()
  {
    return m_repeat_y;
  }

  inline bool sampler::repeat_z()
  {
    return m_repeat_z;
  }

  inline float sampler::anisotropy()
  {
    return m_anisotropy;
  }

  inline float sampler::min_lod()
  {
    return m_min_lod;
  }

  inline float sampler::max_lod()
  {
    return m_max_lod;
  }

  inline const glm::vec4& sampler::border_color() const
  {
    return m_border_color;
  }

  inline const sampler::Filter& sampler::filter() const
  {
    return m_filter;
  }

  inline mgl_opengl::compare_func sampler::compare_func()
  {
    return m_compare_func;
  }

  inline void sampler::set_border_color(float r, float g, float b, float a)
  {
    set_border_color({ r, g, b, a });
  }

} // namespace mgl_opengl
