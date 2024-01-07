#include "layers.hpp"
#include "shader.hpp"

#include "imgui/imgui.h"
#include "mgl_application/application.hpp"
#include "mgl_graphics/buffers.hpp"
#include "mgl_graphics/render.hpp"

static mgl::graphics::vertex_buffer_ref s_vbo = nullptr;
static mgl::graphics::shader_ref shader = nullptr;

void render_layer::render_prepare()
{
  auto& render = mgl::graphics::current_render();
  render.clear(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  render.enable_shader(shader);
  render.draw(s_vbo, nullptr, mgl::graphics::render::draw_mode::TRIANGLES);
  render.disable_shader();
}

void render_layer::on_attach()
{
  mgl::buffer<float> vertices = {
    // x, y, red, green, blue
    0.0,  0.8,  1.0, 0.0, 0.0, //
    -0.6, -0.8, 0.0, 1.0, 0.0, //
    0.6,  -0.8, 0.0, 0.0, 1.0, //
  };

  s_vbo = mgl::create_ref<mgl::graphics::vertex_buffer>(vertices, "2f 3f");
  shader = mgl::create_ref<custom_shader>();
}

void render_layer::on_detach()
{
  shader->release();
  s_vbo->release();
}
