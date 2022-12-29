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
#include "mgl_opengl/mgl_opengl.hpp"
#include "gtest/gtest.h"

TEST(gml_opengl, glsl_source_test)
{
  auto shader = mgl_opengl::glsl_source(R"(
#version 330
#ifdef VERTEX_SHADER
  layout (std140) uniform Common {
    mat4 Mvp;
    vec3 Light;
  };

  in vec3 in_position;
  in vec3 in_normal;
  in vec2 in_texcoord_0;

  out vec3 v_vert;
  out vec3 v_norm;
  out vec2 v_text;

  void main() {
    gl_Position = Mvp * vec4(in_position, 1.0);
    v_vert = in_position;
    v_norm = in_normal;
    v_text = in_texcoord_0;
  }
#endif
#ifdef FRAGMENT_SHADER
  layout (std140) uniform Common {
    mat4 Mvp;
    vec3 Light;
  };

  uniform sampler2D Texture;

  in vec3 v_vert;
  in vec3 v_norm;
  in vec2 v_text;

  out vec4 f_color;

  void main() {
    float lum = clamp(dot(normalize(Light - v_vert), normalize(v_norm)), 0.0, 1.0) * 0.8 + 0.2;
    f_color = vec4(texture(Texture, v_text).rgb * lum, 1.0);
  }
#endif
)");

  auto vs_expected = mgl_core::string(R"(
#version 330
#define VERTEX_SHADER
#define A 1
#define B 2
#line 4
#ifdef VERTEX_SHADER
  layout (std140) uniform Common {
    mat4 Mvp;
    vec3 Light;
  };
  in vec3 in_position;
  in vec3 in_normal;
  in vec2 in_texcoord_0;
  out vec3 v_vert;
  out vec3 v_norm;
  out vec2 v_text;
  void main() {
    gl_Position = Mvp * vec4(in_position, 1.0);
    v_vert = in_position;
    v_norm = in_normal;
    v_text = in_texcoord_0;
  }
#endif
#ifdef FRAGMENT_SHADER
  layout (std140) uniform Common {
    mat4 Mvp;
    vec3 Light;
  };
  uniform sampler2D Texture;
  in vec3 v_vert;
  in vec3 v_norm;
  in vec2 v_text;
  out vec4 f_color;
  void main() {
    float lum = clamp(dot(normalize(Light - v_vert), normalize(v_norm)), 0.0, 1.0) * 0.8 + 0.2;
    f_color = vec4(texture(Texture, v_text).rgb * lum, 1.0);
  }
#endif
)");

  auto fs_expected = mgl_core::string(R"(
#version 330
#define FRAGMENT_SHADER
#define A 1
#define B 2
#line 4
#ifdef VERTEX_SHADER
  layout (std140) uniform Common {
    mat4 Mvp;
    vec3 Light;
  };
  in vec3 in_position;
  in vec3 in_normal;
  in vec2 in_texcoord_0;
  out vec3 v_vert;
  out vec3 v_norm;
  out vec2 v_text;
  void main() {
    gl_Position = Mvp * vec4(in_position, 1.0);
    v_vert = in_position;
    v_norm = in_normal;
    v_text = in_texcoord_0;
  }
#endif
#ifdef FRAGMENT_SHADER
  layout (std140) uniform Common {
    mat4 Mvp;
    vec3 Light;
  };
  uniform sampler2D Texture;
  in vec3 v_vert;
  in vec3 v_norm;
  in vec2 v_text;
  out vec4 f_color;
  void main() {
    float lum = clamp(dot(normalize(Light - v_vert), normalize(v_norm)), 0.0, 1.0) * 0.8 + 0.2;
    f_color = vec4(texture(Texture, v_text).rgb * lum, 1.0);
  }
#endif
)");

  vs_expected = mgl_core::trim(vs_expected);
  fs_expected = mgl_core::trim(fs_expected);

  auto vs_source = shader.vertex({ { "A", "1" }, { "B", "2" } });
  auto fs_source = shader.fragment({ { "A", "1" }, { "B", "2" } });
  auto outputs = shader.outputs();

  EXPECT_EQ(outputs.size(), 4);
  EXPECT_EQ(outputs[0], "v_vert");
  EXPECT_EQ(outputs[1], "v_norm");
  EXPECT_EQ(outputs[2], "v_text");
  EXPECT_EQ(outputs[3], "f_color");
  EXPECT_EQ(vs_source, vs_expected);
  EXPECT_EQ(fs_source, fs_expected);
}