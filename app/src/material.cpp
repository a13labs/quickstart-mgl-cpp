#include "material.hpp"
#include "mgl_graphics/application.hpp"
#include "mgl_graphics/render.hpp"
#include "mgl_window/api.hpp"

static mgl::window::api::program_ref s_program;
static mgl::string_list s_attributes = { "in_vert", "in_color" };

custom_material::custom_material()
    : mgl::graphics::material()
{
  if(s_program == nullptr)
  {
    auto& render = mgl::graphics::current_render();
    auto ctx = render.context();
    s_program = ctx->program({
        R"(
                    #version 330

                    in vec2 in_vert;
                    in vec3 in_color;
                    out vec3 v_color;    // Goes to the fragment shader

                    void main() {
                        gl_Position = vec4(in_vert, 0.0, 1.0);
                        v_color = in_color;
                    }
                  )",
        R"(
                    #version 330

                    in vec3 v_color;
                    out vec4 f_color;

                    void main() {
                        // We're not interested in changing the alpha value
                        f_color = vec4(v_color, 1.0);
                    }
                )",

    });
  }
}

void custom_material::release()
{
  if(s_program != nullptr)
  {
    s_program->release();
  }
}

mgl::window::api::program_ref& custom_material::program()
{
  return s_program;
}

mgl::string_list& custom_material::attributes()
{
  return s_attributes;
}

void custom_material::prepare() { }