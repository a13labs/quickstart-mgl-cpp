#include "mgl_window/mgl_window.hpp"

class example_window : public mgl_window::window
{

  public:
  virtual void on_draw(float time, float frame_time) override;
  virtual void on_load() override;
  virtual void on_unload() override;

  private:
  mgl_core::ref<mgl_opengl::program> m_program;
  mgl_core::ref<mgl_opengl::buffer> m_vbo;
  mgl_core::ref<mgl_opengl::vertex_array> m_vao;
};

void example_window::on_draw(float time, float frame_time)
{
  const auto ctx = context();
  ctx->clear(1.0, 1.0, 1.0);
  m_vao->render(mgl_opengl::render_mode::LINES, 65 * 4);
}

void example_window::on_load()
{
  set_title("Perspective Projection");

  const auto ctx = context();
  m_program = ctx->program({
      R"(
                #version 330

                in vec3 vert;

                uniform float z_near;
                uniform float z_far;
                uniform float fovy;
                uniform float ratio;

                uniform vec3 center;
                uniform vec3 eye;
                uniform vec3 up;

                mat4 perspective() {

                    float zmul = (-2.0 * z_near * z_far) / (z_far - z_near);
                    float ymul = 1.0 / tan(fovy * 3.14159265 / 360);
                    float xmul = ymul / ratio;

                    return mat4(
                        xmul, 0.0, 0.0, 0.0,
                        0.0, ymul, 0.0, 0.0,
                        0.0, 0.0, -1.0, -1.0,
                        0.0, 0.0, zmul, 0.0
                    );
                }

                mat4 lookat() {

                    vec3 forward = normalize(center - eye);
                    vec3 side = normalize(cross(forward, up));
                    vec3 upward = cross(side, forward);

                    return mat4(
                        side.x, upward.x, -forward.x, 0,
                        side.y, upward.y, -forward.y, 0,
                        side.z, upward.z, -forward.z, 0,
                        -dot(eye, side), -dot(eye, upward), dot(eye, forward), 1
                    );
                }

                void main() {
                    gl_Position = perspective() * lookat() * vec4(vert, 1.0);
                }
              )",
      R"(
                #version 330

                out vec4 color;

                void main() {
                    color = vec4(0.04, 0.04, 0.04, 1.0);

                }
            )",

  });

  m_program->uniform("z_near")->set_value(0.1f);
  m_program->uniform("z_far")->set_value(1000.0f);
  m_program->uniform("ratio")->set_value(aspect_ratio());
  m_program->uniform("fovy")->set_value(60.f);

  m_program->uniform("eye")->set_value({ 3.f, 3.f, 3.f });
  m_program->uniform("center")->set_value({ 0.f, 0.f, 0.f });
  m_program->uniform("up")->set_value({ 0.f, 0.f, 1.f });

  mgl_core::mem_buffer<float> grid;

  grid.reserve(65 * 12);

  for(int i = 0; i <= 65; i++)
  {
    std::array<float, 12> elements = {
      (float)(i - 32), -32.0f,          0.0f, (float)(i - 32), 32.0f,           0.0f, //
      -32.0f,          (float)(i - 32), 0.0f, 32.0f,           (float)(i - 32), 0.0f, //
    };

    grid.insert(grid.end(), std::make_move_iterator(elements.begin()), std::make_move_iterator(elements.end()));
  }

  m_vbo = ctx->buffer(grid);
  m_vao = ctx->vertex_array(m_program, { { m_vbo, "3f", { "vert" } } });
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
