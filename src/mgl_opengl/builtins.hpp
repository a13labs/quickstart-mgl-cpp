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
#include "mgl_core/mgl_core.hpp"
#include "opengl.hpp"

namespace mgl_opengl
{
  using gl_function = void*;
  using gl_context = void*;

  enum render_mode
  {
    POINTS = 0x0000,
    LINES = 0x0001,
    LINE_LOOP = 0x0002,
    LINE_STRIP = 0x0003,
    TRIANGLES = 0x0004,
    TRIANGLE_STRIP = 0x0005,
    TRIANGLE_FAN = 0x0006,
    LINES_ADJACENCY = 0x000A,
    LINE_STRIP_ADJACENCY = 0x000B,
    TRIANGLES_ADJACENCY = 0x000C,
    TRIANGLE_STRIP_ADJACENCY = 0x0000D,
    PATCHES = 0x000E,
  };

  enum enable_flag
  {
    NOTHING = 0,
    BLEND = BIT(1),
    DEPTH_TEST = BIT(2),
    CULL_FACE = BIT(3),
    RASTERIZER_DISCARD = BIT(4),
    PROGRAM_POINT_SIZE = BIT(5),
    INVALID = 0x40000000
  };

  enum compare_func
  {
    NONE = 0x0000,
    NEVER = GL_NEVER,
    LESS = GL_LESS,
    EQUAL = GL_EQUAL,
    LESS_EQUAL = GL_LEQUAL,
    GREATER = GL_GREATER,
    NOT_EQUAL = GL_NOTEQUAL,
    GREATER_EQUAL = GL_GEQUAL,
    ALWAYS = GL_ALWAYS,
  };

  struct color_mask
  {
    bool r, g, b, a;
  };

  struct data_type;

  struct glsl_sources;
  class glsl_source;

  class attribute;
  using attribute_ref = mgl_core::ref<mgl_opengl::attribute>;

  class Attachment;
  using attachment_ref = mgl_core::ref<mgl_opengl::Attachment>;

  class Buffer;
  using buffer_ref = mgl_core::ref<mgl_opengl::Buffer>;

  class Context;
  using context_ref = mgl_core::ref<mgl_opengl::Context>;

  class ConditionalRender;
  using conditional_render_ref = mgl_core::ref<mgl_opengl::ConditionalRender>;

  class ComputeShader;
  using compute_shader_ref = mgl_core::ref<mgl_opengl::ComputeShader>;

  class Framebuffer;
  using framebuffer_ref = mgl_core::ref<mgl_opengl::Framebuffer>;

  class Program;
  using program_ref = mgl_core::ref<mgl_opengl::Program>;

  class Query;
  using query_ref = mgl_core::ref<mgl_opengl::Query>;

  class Renderbuffer;
  using renderbuffer_ref = mgl_core::ref<mgl_opengl::Renderbuffer>;

  class Sampler;
  using sampler_ref = mgl_core::ref<mgl_opengl::Sampler>;

  class Scope;
  using scope_ref = mgl_core::ref<mgl_opengl::Scope>;

  class Subroutine;
  using subroutine_ref = mgl_core::ref<mgl_opengl::Subroutine>;

  class Texture;
  using texture_ref = mgl_core::ref<mgl_opengl::Texture>;

  class Texture2D;
  using texture_2d_ref = mgl_core::ref<mgl_opengl::Texture2D>;

  class TextureArray;
  using texture_array_ref = mgl_core::ref<mgl_opengl::TextureArray>;

  class Texture3D;
  using texture_3d_ref = mgl_core::ref<mgl_opengl::Texture3D>;

  class TextureCube;
  using texture_cube_ref = mgl_core::ref<mgl_opengl::TextureCube>;

  class Uniform;
  using uniform_ref = mgl_core::ref<mgl_opengl::Uniform>;

  class UniformBlock;
  using uniform_block_ref = mgl_core::ref<mgl_opengl::UniformBlock>;

  class Varying;
  using varying_ref = mgl_core::ref<mgl_opengl::Varying>;

  class VertexArray;
  using vertex_array_ref = mgl_core::ref<mgl_opengl::VertexArray>;

  struct texture_binding
  {
    texture_ref texture;
    int binding;
  };

  struct buffer_binding
  {
    buffer_ref buffer;
    int binding;
  };

  struct sampler_binding
  {
    sampler_ref sampler;
    int binding;
  };

  using shader_defines = mgl_core::dict<mgl_core::string, mgl_core::string>;
  using attributes_dict = mgl_core::dict<mgl_core::string, attribute_ref>;
  using subroutines_dict = mgl_core::dict<mgl_core::string, subroutine_ref>;
  using uniforms_dict = mgl_core::dict<mgl_core::string, uniform_ref>;
  using uniform_blocks_dict = mgl_core::dict<mgl_core::string, uniform_block_ref>;
  using varyings_dict = mgl_core::dict<mgl_core::string, varying_ref>;
  using attachments_ref = mgl_core::ref_list<Attachment>;
  using shaders_outputs = mgl_core::string_list;
  using fragment_outputs = mgl_core::dict<mgl_core::string, int>;
  using color_masks = mgl_core::list<color_mask>;
  using textures = mgl_core::ref_list<Texture>;
  using buffers = mgl_core::ref_list<Buffer>;
  using samplers = mgl_core::ref_list<Sampler>;
  using texture_bindings = mgl_core::list<texture_binding>;
  using buffer_bindings = mgl_core::list<buffer_binding>;
  using sampler_bindings = mgl_core::list<sampler_binding>;

  extern const std::string no_shader;

  struct vertex_data
  {
    buffer_ref buffer;
    const char* format;
    mgl_core::string_list attributes;
  };

  using vertex_data_list = mgl_core::list<vertex_data>;
} // namespace mgl_opengl
