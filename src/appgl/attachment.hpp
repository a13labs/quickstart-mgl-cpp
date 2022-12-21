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

namespace AppGL
{
  class Attachment
  {
public:
    enum Type
    {
      TEXTURE,
      RENDERBUFFER
    };

    virtual ~Attachment() = default;
    virtual Attachment::Type attachment_type() = 0;
    virtual int width() = 0;
    virtual int height() = 0;
    virtual int samples() = 0;
    virtual bool depth() = 0;

private:
    friend class Context;
    virtual void color_attachment(Framebuffer* fb, int index) = 0;
    virtual void depth_attachment() = 0;
    virtual const Context* context() const = 0;
  };
} // namespace AppGL