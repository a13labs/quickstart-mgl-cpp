#pragma once
#include "builtins.hpp"

namespace mgl_core
{
  static constexpr double pi = 3.14159265358979323846;

  inline double deg2rad(double degrees)
  {
    return degrees * pi / 180.0;
  }
} // namespace mgl_core
