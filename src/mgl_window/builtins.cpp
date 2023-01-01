#include "builtins.hpp"
#include "mgl_window/context/window.hpp"

namespace mgl_window
{
  mgl_opengl::context_ref current_context()
  {
    return window::current().context();
  }

  window& current_window()
  {
    return window::current();
  }

} // namespace mgl_window