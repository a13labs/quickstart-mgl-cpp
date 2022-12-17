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
  class Program
  {
public:
    ~Program() { release(); }

    void release();

private:
    friend class Context;
    Program();

    Context* m_context;
    int m_geometry_input;
    int m_geometry_output;
    int m_program_obj;
    int m_num_vertex_shader_subroutines;
    int m_num_fragment_shader_subroutines;
    int m_num_geometry_shader_subroutines;
    int m_num_tess_evaluation_shader_subroutines;
    int m_num_tess_control_shader_subroutines;
    int m_geometry_vertices;
    int m_num_varyings;
    bool m_released;
  };
} // namespace AppGL
