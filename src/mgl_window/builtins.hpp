#pragma once
#include "mgl/mgl.hpp"
#include "mgl_core/mgl_core.hpp"

namespace mgl_window
{
  class Event;
  class EventListener;
  class WindowCloseEvent;
  class WindowResizeEvent;
  class KeyPressedEvent;
  class KeyReleasedEvent;
  class KeyTypedEvent;
  class MouseMovedEvent;
  class MouseScrolledEvent;
  class MouseButtonPressedEvent;
  class MouseButtonReleasedEvent;

  using event_handler = std::function<void(Event&)>;

  using shader_defines = mgl_core::dict<mgl_core::string, mgl_core::string>;

} // namespace mgl_window