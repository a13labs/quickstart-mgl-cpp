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

namespace AppGL
{
  class Sampler
  {
public:
    ~Sampler() { release(); }

public:
    void release();

private:
    friend class Context;
    Sampler();

    Context* m_context;
    int m_sampler_obj;
    int m_min_filter;
    int m_mag_filter;
    float m_anisotropy;
    int m_compare_func;
    bool m_repeat_x;
    bool m_repeat_y;
    bool m_repeat_z;
    float m_border_color;
    float m_min_lod;
    float m_max_lod;
    bool m_released;
  };
} // namespace AppGL
