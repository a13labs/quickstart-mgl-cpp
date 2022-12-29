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

namespace mgl_opengl
{
  class varying
  {
public:
    ~varying() = default;

private:
    friend class context;
    varying(const mgl_core::string& name, int number, size_t array_length, int m_dimension);

    mgl_core::string m_name;
    int m_number;
    size_t m_array_length;
    int m_dimension;
  };
} // namespace mgl_opengl
