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
} // namespace mgl_window