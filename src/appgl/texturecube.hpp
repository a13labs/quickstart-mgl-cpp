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
#include "appgl.hpp"

namespace AppGL
{
  class TextureCube
  {
public:
    ~TextureCube() { release(); }

    void release();

private:
    friend class Context;
    TextureCube();

    Context* m_context;
    DataType* m_data_type;
    int m_texture_obj;
    int m_width;
    int m_height;
    int m_depth;
    int m_components;
    int m_min_filter;
    int m_mag_filter;
    int m_max_level;
    float m_anisotropy;
    bool m_released;
  };
} // namespace AppGL