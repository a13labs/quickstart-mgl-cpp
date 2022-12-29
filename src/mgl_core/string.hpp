#pragma once
#include "builtins.hpp"
#include <spdlog/fmt/fmt.h>

namespace mgl_core
{
  static const size_t npos = std::string::npos;

  template <typename... T>
  inline auto format(fmt::format_string<T...> fmt, T&&... args) -> string
  {
    return fmt::vformat(fmt, fmt::make_format_args(args...));
  }

  string_list split(const string& s, char delimiter);

  string trim(string s);
  string trim(string s, const std::function<bool(char)>& predicate);

  std::string
  join(char delimiter, const std::vector<std::string>& vec, std::size_t start_index = 0, std::size_t end_index = npos);

  template <typename T>
  string join(char delimiter, const list<T>& vec, std::size_t start_index = 0, std::size_t end_index = npos)
  {
    std::ostringstream result;

    if(end_index < start_index)
      return "";

    if(end_index == npos)
      end_index = vec.size() - 1;

    if(start_index < vec.size() && end_index < vec.size())
    {
      std::copy(vec.begin() + start_index, vec.begin() + end_index, std::ostream_iterator<T>(result, &delimiter));
      result << vec[end_index];
    }
    return result.str();
  }

  inline bool in(const string& e, const string& str)
  {
    return str.find(e) != npos;
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
    return std::all_of(lst.begin(), lst.end(), [](const string& str) { return str.empty(); });
  }
} // namespace mgl_core