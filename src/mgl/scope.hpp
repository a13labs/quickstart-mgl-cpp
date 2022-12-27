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

namespace mgl
{
  class Scope
  {
public:
    struct BindingData
    {
      int binding;
      int type;
      int gl_object;
    };

    ~Scope() = default;

    void release();
    bool released();

    void begin();
    void end();

private:
    friend class Context;
    Scope() = default;

    Context* m_context;
    mgl_core::ref<Framebuffer> m_framebuffer;
    mgl_core::ref<Framebuffer> m_old_framebuffer;
    sampler_bindings m_samplers;
    mgl_core::list<Scope::BindingData> m_textures;
    mgl_core::list<Scope::BindingData> m_buffers;
    int m_enable_flags;
    int m_old_enable_flags;
    bool m_released;
  };

  inline bool Scope::released()
  {
    return m_released;
  }
} // namespace mgl