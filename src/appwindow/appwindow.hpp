#pragma once
#include "appcore/appcore.hpp"

namespace AppWindow
{
  class Event;
  using EventHandler = std::function<void(Event&)>;
} // namespace AppWindow