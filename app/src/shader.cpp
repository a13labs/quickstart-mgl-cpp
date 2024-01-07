#include "shader.hpp"
#include "mgl_application/application.hpp"
#include "mgl_graphics/render.hpp"
#include "mgl_window/api.hpp"

static mgl::window::api::program_ref s_program;
static mgl::string_list s_attributes = { "in_vert", "in_color" };

custom_shader::custom_shader()
    : mgl::graphics::shader()
{

  if(s_program == nullptr)
  {

#include "shaders/fragment/fragment.hpp"
#include "shaders/vertex/vertex.hpp"

    auto& render = mgl::graphics::current_render();
    auto ctx = render.context();
    // Load vertex and fragment shaders from generated source
    mgl::opengl::shaders glsl = { reinterpret_cast<const char*>(vertex_shader_source),
                                  reinterpret_cast<const char*>(fragment_shader_source) };

    s_program = ctx->program(glsl);
  }
}

void custom_shader::release()
{
  if(s_program != nullptr)
  {
    s_program->release();
  }
}

mgl::window::api::program_ref& custom_shader::program()
{
  return s_program;
}

mgl::string_list& custom_shader::attributes()
{
  return s_attributes;
}

void custom_shader::prepare() { }