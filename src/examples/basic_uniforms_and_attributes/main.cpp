#include "mgl_window/mgl_window.hpp"
#include <cmath>

class example_window : public mgl::window::window
{

  public:
  virtual void on_draw(float time, float frame_time) override;
  virtual void on_load() override;
  virtual void on_unload() override;

  private:
  mgl::core::ref<mgl::opengl::program> m_program;
  mgl::core::ref<mgl::opengl::buffer> m_vbo;
  mgl::core::ref<mgl::opengl::vertex_array> m_vao;
  mgl::core::ref<mgl::opengl::uniform> m_scale, m_rotation;
  float m_time = 0;
};

void example_window::on_draw(float time, float frame_time)
{
  const auto ctx = context();

  float sin_scale = static_cast<float>(sin(mgl::core::deg2rad(time * 60)));

  ctx->clear(1.0, 1.0, 1.0);
  ctx->enable(mgl::opengl::enable_flag::BLEND);
  m_vao->render();

  m_rotation->set_value(time);
  m_scale->set_value({ sin_scale * 0.75, 0.75 });
}

void example_window::on_load()
{
  set_title("Alpha Blending");

  const auto ctx = context();
  m_program = ctx->program({
      R"(
                #version 330
                in vec2 vert;

                uniform vec2 scale;
                uniform float rotation;

                void main() {

                    mat2 rot = mat2(
                        cos(rotation), sin(rotation),
                        -sin(rotation), cos(rotation)
                    );
                    gl_Position = vec4((rot * vert) * scale, 0.0, 1.0);
                }
              )",
      R"(
                #version 330

                out vec4 color;
                void main() {
                    color = vec4(0.3, 0.5, 1.0, 1.0);
                }
            )",

  });

  m_scale = m_program->uniform("scale");
  m_rotation = m_program->uniform("rotation");

  m_scale->set_value({ width() / height() * 0.75, 0.25 });

  mgl::core::mem_buffer<float> vertices = {
    1.0,  0.0, //
    -0.5, 0.86, //
    -0.5, -0.86 //
  };

  m_vbo = ctx->buffer(vertices);
  m_vao = ctx->vertex_array(m_program, { { m_vbo, "2f", { "vert" } } });
}

void example_window::on_unload()
{
  m_vao->release();
  m_vbo->release();
  m_program->release();
}

int main(int argc, char* argv[])
{
  example_window app;
  app.run();

  return 0;
}