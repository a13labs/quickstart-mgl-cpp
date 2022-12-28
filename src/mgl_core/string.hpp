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

  inline void ltrim(string& s)
  {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
  }

  inline void rtrim(string& s)
  {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
  }

  inline void trim(string& s)
  {
    rtrim(s);
    ltrim(s);
  }

  inline string ltrim_copy(string s)
  {
    ltrim(s);
    return s;
  }

  inline string rtrim_copy(string s)
  {
    rtrim(s);
    return s;
  }

  inline string trim_copy(string s)
  {
    trim(s);
    return s;
  }

  inline bool in(const string& e, const string& str)
  {
    return str.find(e);
  }
} // namespace mgl_core