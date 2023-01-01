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
#include "glm/glm.hpp"

namespace mgl::opengl
{
  class subroutine
  {
    enum type
    {
      VERTEX_SHADER = GL_VERTEX_SHADER,
      FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
      GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
      TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
      TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
    };

public:
    ~subroutine() = default;

private:
    friend class context;
    subroutine(const mgl::core::string& name, int index, subroutine::type type);

    mgl::core::string m_name;
    int m_index;
    subroutine::type m_type;
  };
} // namespace  mgl::opengl
