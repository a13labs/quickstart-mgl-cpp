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
#include "attachment.hpp"

namespace AppGL
{
  class Renderbuffer : public Attachment
  {
public:
    ~Renderbuffer() { release(); }

    void release();
    virtual Attachment::Type attachment_type() override;
    virtual int width() override;
    virtual int height() override;
    virtual int samples() override;
    virtual bool depth() override;

private:
    friend class Context;
    Renderbuffer(){};

    virtual void color_attachment(Framebuffer* fb, int index) override;
    virtual void depth_attachment() override;
    virtual const Context* context() const override;

    Context* m_context;
    DataType* m_data_type;
    int m_renderbuffer_obj;
    int m_width;
    int m_height;
    int m_components;
    int m_samples;
    bool m_depth;
    bool m_released;
  };
} // namespace AppGL