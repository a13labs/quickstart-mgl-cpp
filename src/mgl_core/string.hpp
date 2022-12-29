#pragma once
#include "builtins.hpp"
#include <spdlog/fmt/fmt.h>

namespace mgl_core
{
  static const size_t npos = -1;

  template <typename... T>
  inline auto format(fmt::format_string<T...> fmt, T&&... args) -> string
  {
    return fmt::vformat(fmt, fmt::make_format_args(args...));
  }

  string_list split(const string& s, char delimiter);
  string trim(string s);
  string trim(string s, const std::function<bool(char)>& predicate);
  string join(char delimiter, const string_list& list, size_t start = 0, size_t end = npos);

  string join(const string& delimiter, const list<int32_t>& list, size_t start = 0, size_t end = npos);
  string join(const string& delimiter, const list<int16_t>& list, size_t start = 0, size_t end = npos);
  string join(const string& delimiter, const list<int8_t>& list, size_t start = 0, size_t end = npos);
  string join(const string& delimiter, const list<uint32_t>& list, size_t start = 0, size_t end = npos);
  string join(const string& delimiter, const list<uint16_t>& list, size_t start = 0, size_t end = npos);
  string join(const string& delimiter, const list<uint8_t>& list, size_t start = 0, size_t end = npos);
  string join(const string& delimiter, const list<float>& list, size_t start = 0, size_t end = npos);

  inline bool in(const string& e, const string& str)
  {
    return str.find(e);
  }

  inline bool starts_with(const string& str, string e)
  {
    return str.rfind(e, 0) == 0;
  }

  inline int to_int(const string& str)
  {
    return std::stoi(str);
  };

  inline bool all_empty(const string_list& lst)
  {
    return std::all_of(lst.begin(), lst.end(), [](const std::string& str) { return str.empty(); });
  }
} // namespace mgl_core