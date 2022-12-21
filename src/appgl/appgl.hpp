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
#include "appcore/appcore.hpp"

#define MGL_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MGL_MIN(a, b) (((a) < (b)) ? (a) : (b))

namespace AppGL
{
  struct Rect
  {
    int X;
    int Y;
    int W;
    int H;

    Rect(int x, int y, int w, int h)
        : X(x)
        , Y(y)
        , W(w)
        , H(h)
    { }

    Rect(int w, int h)
        : X(0)
        , Y(0)
        , W(w)
        , H(h)
    { }

    Rect()
        : X(0)
        , Y(0)
        , W(0)
        , H(0)
    { }
  };

  struct Size
  {
    int W;
    int H;

    Size(int w, int h)
        : W(w)
        , H(h)
    { }

    Size()
        : W(0)
        , H(0)
    { }
  };

  using GLFunction = void*;
  using GLContext = void*;

  struct DataType;
  struct ShadersSources;

  class Attribute;
  class Attachment;
  class Buffer;
  class Context;
  class ConditionalRender;
  class ComputeShader;
  class Framebuffer;
  class Program;
  class Query;
  class Renderbuffer;
  class Sampler;
  class Scope;
  class Subroutine;
  class Texture2D;
  class TextureArray;
  class Texture3D;
  class TextureCube;
  class Uniform;
  class UniformBlock;
  class Varying;
  class VertexArray;
  class Window;

  using AttributesMap = AppCore::Dict<AppCore::String, AppCore::Ref<Attribute>>;
  using SubroutinesMap = AppCore::Dict<AppCore::String, AppCore::Ref<Subroutine>>;
  using UniformsMap = AppCore::Dict<AppCore::String, AppCore::Ref<Uniform>>;
  using UniformBlocksMap = AppCore::Dict<AppCore::String, AppCore::Ref<UniformBlock>>;
  using VaryingsMap = AppCore::Dict<AppCore::String, AppCore::Ref<Varying>>;

  using AttachmentsRef = AppCore::VectorRef<Attachment>;

  using ColorMask = AppCore::List<bool>;

  using ShadersOutputs = AppCore::StringList;
  using FragmentOutputs = AppCore::Dict<AppCore::String, int>;

  extern const std::string NoShader;
} // namespace AppGL
