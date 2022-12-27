#include "appgl/buffer.hpp"
#include "appgl/context.hpp"
#include "appgl/datatype.hpp"
#include "appgl/program.hpp"
#include "appgl/sdl/window.hpp"
#include "appgl/shaderssources.hpp"
#include "appgl/uniform.hpp"
#include "appgl/vertexarray.hpp"

class ExampleWindow : public AppGL::Window
{

  public:
  virtual void on_draw(float time, float frame_time) override;
  virtual void on_load() override;
  virtual void on_unload() override;

  private:
  AppCore::Ref<AppGL::Program> m_program;
  AppCore::Ref<AppGL::Buffer> m_vbo;
  AppCore::Ref<AppGL::VertexArray> m_vao;
  AppCore::Ref<AppGL::Uniform> m_scale, m_rotation;
  float m_time = 0;
};

void ExampleWindow::on_draw(float time, float frame_time)
{
  const auto ctx = context();
  ctx->clear(1.0, 1.0, 1.0);
  ctx->enable(AppGL::Context::EnableFlag::BLEND);
  m_rotation->set_value(time);
  m_vao->render(10);
}

void ExampleWindow::on_load()
{
  APPCORE_INFO("ExampleWindow::on_load");
  set_title("basic_alpha_blending");

  const auto ctx = context();
  m_program = ctx->program({
      R""""(
                #version 330

                in vec2 vert;

                in vec4 vert_color;
                out vec4 frag_color;

                uniform vec2 scale;
                uniform float rotation;

                void main() {
                    frag_color = vert_color;
                    float r = rotation * (0.5 + gl_InstanceID * 0.05);
                    mat2 rot = mat2(cos(r), sin(r), -sin(r), cos(r));
                    gl_Position = vec4((rot * vert)*scale, 0.0, 1.0);
                    // gl_Position = vec4(vert, 0.0, 1.0);
                }
              )"""",
      R""""(
                #version 330
                in vec4 frag_color;
                out vec4 color;
                void main() {
                    color = vec4(frag_color);
                }
            )"""",

  });

  m_scale = m_program->uniform("scale");
  m_rotation = m_program->uniform("rotation");

  m_scale->set_value({ 0.5, aspect_ratio() * 0.5 });

  float vertices[] = {
    1.0,  0.0, //
    1.0,  0.0,   0.0, 0.5, //

    -0.5, 0.86, //
    0.0,  1.0,   0.0, 0.5, //

    -0.5, -0.86, //
    0.0,  0.0,   1.0, 0.5, //
  };

  m_vbo = ctx->buffer(vertices);
  m_vao = ctx->vertex_array(m_program, { { m_vbo, "2f 4f", { "vert", "vert_color" } } });
}

void ExampleWindow::on_unload()
{
  APPCORE_INFO("ExampleWindow::on_unload");
  m_vao->release();
  m_vbo->release();
  m_program->release();
}

int main(int argc, char* argv[])
{
  ExampleWindow app;
  app.run();

  return 0;
}