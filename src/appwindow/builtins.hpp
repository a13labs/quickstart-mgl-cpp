#pragma once
#include "appcore/appcore.hpp"
#include "appgl/appgl.hpp"

namespace AppWindow
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

  using EventHandler = std::function<void(Event&)>;
} // namespace AppWindow