#pragma once
#include "builtins.hpp"

namespace mgl::core
{

  template <typename T, typename U>
  list<pair<T, U>> zip(const list<T>& a, const list<U>& b)
  {
    list<pair<T, U>> zipped;

    for(std::size_t i = 0; i < a.size(); ++i)
    {
      zipped.emplace_back(a[i], b[i]);
    }

    return zipped;
  }
} // namespace  mgl::core