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
#include "texture.hpp"

namespace AppGL
{
  class Texture2D : public Attachment, public Texture
  {
public:
    ~Texture2D() { release(); }

    void release();
    virtual Attachment::Type attachment_type() override;
    virtual Texture::Type texture_type() override;
    virtual int width() override;
    virtual int height() override;
    virtual int samples() override;
    virtual bool depth() override;

private:
    friend class Context;
    Texture2D();

    virtual void color_attachment(Framebuffer* fb, int index) override;
    virtual void depth_attachment() override;
    virtual const Context* context() const override;

    Context* m_context;
    DataType* m_data_type;
    int m_texture_obj;
    int m_width;
    int m_height;
    int m_components;
    int m_samples;
    int m_min_filter;
    int m_mag_filter;
    int m_max_level;
    int m_compare_func;
    float m_anisotropy;
    bool m_depth;
    bool m_repeat_x;
    bool m_repeat_y;
    bool m_external;
    bool m_released;
  };
} // namespace AppGL