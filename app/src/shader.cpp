#include "shader.hpp"
#include "mgl_application/application.hpp"
#include "mgl_graphics/render.hpp"

#include "shaders/fragment/fragment.hpp"
#include "shaders/vertex/vertex.hpp"

static mgl::string_list s_attributes = { "in_vert", "in_color" };

void custom_shader::load()
{
  auto& render = mgl::graphics::current_render();
  auto ctx = render.context();
  // Load vertex and fragment shaders from generated source
  mgl::opengl::shaders glsl = { vertex_shader_source, fragment_shader_source };

  m_program = ctx->program(glsl);
}

void custom_shader::unload()
{
  m_program->release();
}

mgl::window::api::program_ref& custom_shader::native()
{
  return m_program;
}

mgl::string_list& custom_shader::attributes()
{
  return s_attributes;
}

void custom_shader::prepare() { }