
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
#ifdef MGL_OPENGL_GLX

#  include "mgl_core/log.hpp"
#  include "mgl_opengl/context.hpp"
#  include <dlfcn.h>

namespace mgl_opengl
{
  ContextGLX::ContextGLX(context_mode::Enum mode, int required)
  {
    m_mode = mode;
    m_context = nullptr;
  }

  ContextGLX::~ContextGLX() { }

  gl_function ContextGLX::load(const mgl_core::string& method)
  {
    return nullptr;
  }

  void ContextGLX::enter() { }

  void ContextGLX::exit() { }

  void ContextGLX::release() { }

  bool ContextGLX::is_valid()
  {
    return false;
  }
} // namespace mgl_opengl
#endif