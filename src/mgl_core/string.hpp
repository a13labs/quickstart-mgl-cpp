#pragma once
#include "builtins.hpp"
#include <spdlog/fmt/fmt.h>

namespace mgl_core
{
  template <typename... T>
  inline auto format(fmt::format_string<T...> fmt, T&&... args) -> string
  {
    return fmt::vformat(fmt, fmt::make_format_args(args...));
  }

  void split(string& str, const string& delimiter, string_list& list);
  string join(const string& delimiter, const string_list& list);
  string join(const string& delimiter, const list<int32_t>& list);
  string join(const string& delimiter, const list<int16_t>& list);
  string join(const string& delimiter, const list<int8_t>& list);
  string join(const string& delimiter, const list<uint32_t>& list);
  string join(const string& delimiter, const list<uint16_t>& list);
  string join(const string& delimiter, const list<uint8_t>& list);
  string join(const string& delimiter, const list<float>& list);

} // namespace mgl_core