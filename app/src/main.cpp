#include "layers.hpp"
#include "mgl_graphics/application.hpp"

class example : public mgl::graphics::application
{
  public:
  example()
      : mgl::graphics::application()
  {
    config().gui_layer = mgl::create_ref<gui_layer>();
    config().render_layer = mgl::create_ref<render_layer>();
  }
};

int main(int argc, char* argv[])
{
  example app;
  app.run();

  return 0;
}