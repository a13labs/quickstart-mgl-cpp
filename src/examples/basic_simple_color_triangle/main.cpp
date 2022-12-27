#include "appgl/buffer.hpp"
#include "appgl/context.hpp"
#include "appgl/datatype.hpp"
#include "appgl/program.hpp"
#include "appgl/shaderssources.hpp"
#include "appgl/vertexarray.hpp"
#include "appwindow/context/window.hpp"

class ExampleWindow : public AppWindow::Window
{

  public:
  virtual void on_draw(float time, float frame_time) override;
  virtual void on_load() override;
  virtual void on_unload() override;

  private:
  AppCore::Ref<AppGL::Program> m_program;
  AppCore::Ref<AppGL::Buffer> m_vbo;
  AppCore::Ref<AppGL::VertexArray> m_vao;
};

void ExampleWindow::on_draw(float time, float frame_time)
{
  const auto ctx = context();
  ctx->clear(1.0, 1.0, 1.0);
  m_vao->render();
}

void ExampleWindow::on_load()
{
  APPCORE_INFO("ExampleWindow::on_load");
  set_title("basic_simple_color_triangle");

  const auto ctx = context();
  m_program = ctx->program({
      R""""(
                #version 330

                in vec2 in_vert;

                in vec3 in_color;
                out vec3 v_color;    // Goes to the fragment shader

                void main() {
                    gl_Position = vec4(in_vert, 0.0, 1.0);
                    v_color = in_color;
                }
              )"""",
      R""""(
                #version 330

                in vec3 v_color;
                out vec4 f_color;

                void main() {
                    // We're not interested in changing the alpha value
                    f_color = vec4(v_color, 1.0);
                }
            )"""",

  });

  float vertices[] = {
    // x, y, red, green, blue
    0.0,  0.8,  1.0, 0.0, 0.0, //
    -0.6, -0.8, 0.0, 1.0, 0.0, //
    0.6,  -0.8, 0.0, 0.0, 1.0, //
  };

  m_vbo = ctx->buffer(vertices);
  m_vao = ctx->vertex_array(m_program, { { m_vbo, "2f 3f", { "in_vert", "in_color" } } });
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