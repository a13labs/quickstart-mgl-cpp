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
  class uniform_block
  {
public:
    ~uniform_block() = default;

    int binding();
    void set_binding(int value);

private:
    friend class context;
    uniform_block(const mgl::core::string& name, int program_obj, int index, size_t size, context* ctx);

    context* m_context;
    mgl::core::string m_name;
    int m_program_obj;
    int m_index;
    int m_size;
  };
} // namespace  mgl::opengl
