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
#include "attachment.hpp"
#include "builtins.hpp"

namespace mgl_opengl
{
  class renderbuffer : public attachment
  {
public:
    ~renderbuffer() = default;

    void release();
    bool released();

    virtual attachment::type attachment_type() override;
    int width();
    int height();
    int samples();
    bool depth();
    int components();

    int glo();

private:
    friend class context;
    renderbuffer(){};

    context* m_context;
    data_type* m_data_type;
    int m_renderbuffer_obj;
    int m_width;
    int m_height;
    int m_components;
    int m_samples;
    bool m_depth;
    bool m_released;
  };

  inline int renderbuffer::glo()
  {
    return m_renderbuffer_obj;
  }

  inline bool renderbuffer::released()
  {
    return m_released;
  }

  inline int renderbuffer::components()
  {
    return m_components;
  }

  inline int renderbuffer::width()
  {
    return m_width;
  }

  inline int renderbuffer::height()
  {
    return m_height;
  }

  inline int renderbuffer::samples()
  {
    return m_samples;
  }

  inline bool renderbuffer::depth()
  {
    return m_depth;
  }

} // namespace mgl_opengl