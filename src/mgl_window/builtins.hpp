#pragma once
#include "mgl_core/mgl_core.hpp"
#include "mgl_opengl/mgl_opengl.hpp"

namespace mgl_window
{
  class window;
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

  namespace render
  {
    struct buffer_element;
    using buffer_layout = mgl_core::list<buffer_element>;

    class vertex_buffer;
    using vertex_buffers_list = mgl_core::list<vertex_buffer>;
  } // namespace render

  using event_handler = std::function<void(event&)>;

  using shader_defines = mgl_core::dict<mgl_core::string, mgl_core::string>;

  mgl_opengl::context_ref current_context();
  window& current_window();

} // namespace mgl_window