#include "layers.hpp"
#include "mgl_application/application.hpp"

class example : public mgl::application::application
{
  public:
  example()
      : mgl::application::application()
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