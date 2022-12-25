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
#include "opengl.hpp"

#define APPGL_MAX(a, b) (((a) > (b)) ? (a) : (b))
#define APPGL_MIN(a, b) (((a) < (b)) ? (a) : (b))

namespace AppGL
{
  using GLFunction = void*;
  using GLContext = void*;

  struct ColorMask
  {
    bool r, g, b, a;
  };

  struct DataType;

  struct Viewport2D
  {
    int x;
    int y;
    int width;
    int height;

    Viewport2D(int x, int y, int w, int h)
        : x(x)
        , y(y)
        , width(w)
        , height(h)
    { }

    Viewport2D(int w, int h)
        : x(0)
        , y(0)
        , width(w)
        , height(h)
    { }

    Viewport2D()
        : x(0)
        , y(0)
        , width(0)
        , height(0)
    { }
  };

  struct Viewport3D
  {
    int x;
    int y;
    int z;
    int width;
    int height;
    int depth;

    Viewport3D(int x, int y, int z, int w, int h, int d)
        : x(x)
        , y(y)
        , z(z)
        , width(w)
        , height(h)
        , depth(d)
    { }

    Viewport3D(int w, int h, int d)
        : x(0)
        , y(0)
        , z(0)
        , width(w)
        , height(h)
        , depth(d)
    { }

    Viewport3D()
        : x(0)
        , y(0)
        , z(0)
        , width(0)
        , height(0)
        , depth(0)
    { }
  };

  struct Size
  {
    int width;
    int height;

    Size(int w, int h)
        : width(w)
        , height(h)
    { }

    Size()
        : width(0)
        , height(0)
    { }
  };

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
  class Texture;
  class Texture2D;
  class TextureArray;
  class Texture3D;
  class TextureCube;
  class Uniform;
  class UniformBlock;
  class Varying;
  class VertexArray;
  class Window;

  struct TextureBinding
  {
    AppCore::Ref<Texture> texture;
    int binding;
  };

  struct BufferBinding
  {
    AppCore::Ref<Buffer> buffer;
    int binding;
  };

  struct SamplerBinding
  {
    AppCore::Ref<Sampler> sampler;
    int binding;
  };

  using AttributesMap = AppCore::Dict<AppCore::String, AppCore::Ref<Attribute>>;
  using SubroutinesMap = AppCore::Dict<AppCore::String, AppCore::Ref<Subroutine>>;
  using UniformsMap = AppCore::Dict<AppCore::String, AppCore::Ref<Uniform>>;
  using UniformBlocksMap = AppCore::Dict<AppCore::String, AppCore::Ref<UniformBlock>>;
  using VaryingsMap = AppCore::Dict<AppCore::String, AppCore::Ref<Varying>>;
  using AttachmentsRef = AppCore::VectorRef<Attachment>;
  using ShadersOutputs = AppCore::StringList;
  using FragmentOutputs = AppCore::Dict<AppCore::String, int>;
  using ColorMasks = AppCore::List<ColorMask>;
  using Textures = AppCore::VectorRef<Texture>;
  using Buffers = AppCore::VectorRef<Buffer>;
  using Samplers = AppCore::VectorRef<Sampler>;
  using TextureBindings = AppCore::List<TextureBinding>;
  using BufferBindings = AppCore::List<BufferBinding>;
  using SamplerBindings = AppCore::List<SamplerBinding>;

  extern const std::string NoShader;
} // namespace AppGL
