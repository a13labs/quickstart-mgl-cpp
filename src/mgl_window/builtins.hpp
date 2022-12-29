#pragma once
#include "mgl_core/mgl_core.hpp"
#include "mgl_opengl/mgl_opengl.hpp"

namespace mgl_window
{
  class event;
  class window_close_event;
  class window_resize_event;
  class key_pressed_event;
  class key_released_event;
  class key_typed_event;
  class mouse_moved_event;
  class mouse_scrolled_event;
  class mouse_button_pressed_event;
  class mouse_button_released_event;

  using event_handler = std::function<void(event&)>;

  using shader_defines = mgl_core::dict<mgl_core::string, mgl_core::string>;

} // namespace mgl_window