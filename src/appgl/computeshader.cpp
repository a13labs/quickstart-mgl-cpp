
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
#include "computeshader.hpp"
#include "context.hpp"

namespace AppGL
{
  void ComputeShader::release()
  {
    if(m_released)
      return;

    m_released = true;

    const GLMethods& gl = m_context->gl();
    gl.DeleteShader(m_shader_obj);
    gl.DeleteProgram(m_program_obj);
  }

  void ComputeShader::run(int x, int y, int z)
  {
    if(m_released)
      return;

    const GLMethods& gl = m_context->gl();

    gl.UseProgram(m_program_obj);
    gl.DispatchCompute(x, y, z);
  }

} // namespace AppGL
