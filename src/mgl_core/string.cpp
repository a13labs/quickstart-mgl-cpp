#include "string.hpp"
#include "log.hpp"

namespace mgl_core
{
  string_list split(const string& s, char delimiter)
  {
    string_list tokens;
    string token;
    std::istringstream token_stream(s);
    while(std::getline(token_stream, token, delimiter))
    {
      if(!token.empty())
      {
        tokens.push_back(token);
      }
    }
    if(!s.empty() && s.back() == delimiter)
    {
      tokens.push_back("");
    }
    return tokens;
  }

  string trim(string s)
  {
    // Remove leading whitespace
    s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), isspace));
    // Remove trailing whitespace
    s.erase(std::find_if_not(s.rbegin(), s.rend(), isspace).base(), s.end());
    return s;
  }

  string trim(string s, const std::function<bool(char)>& predicate)
  {
    s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), predicate));
    s.erase(std::find_if_not(s.rbegin(), s.rend(), predicate).base(), s.end());
    return s;
  }

  string join(char delimiter, const string_list& list, size_t start, size_t end)
  {
    MGL_CORE_ASSERT(start >= 0 && start < list.size(), "out of bounds");
    MGL_CORE_ASSERT(end == npos || end < list.size(), "out of bounds");

    if(end == npos)
      end = list.size();

    string ret;
    for(auto it = list.begin() + start; it != list.begin() + end; it++)
    {
      if(!ret.empty())
        ret += delimiter;
      ret += *it;
    }
    return ret;
  }

  template <typename t>
  inline string join_impl(const string& delimiter, const list<t>& list, size_t start, size_t end)
  {
    MGL_CORE_ASSERT(start >= 0 && start < list.size(), "out of bounds");
    MGL_CORE_ASSERT(end == npos || end < list.size(), "out of bounds");

    if(end == npos)
      end = list.size();

    string ret;
    for(auto it = list.begin() + start; it != list.begin() + end; it++)
    {
      if(!ret.empty())
        ret += delimiter;
      ret += std::to_string(*it);
    }
    return ret;
  }

  string join(const string& delimiter, const list<int32_t>& list, size_t start, size_t end)
  {
    return join_impl<int32_t>(delimiter, list, start, end);
  }

  string join(const string& delimiter, const list<int16_t>& list, size_t start, size_t end)
  {
    return join_impl<int16_t>(delimiter, list, start, end);
  }

  string join(const string& delimiter, const list<int8_t>& list, size_t start, size_t end)
  {
    return join_impl<int8_t>(delimiter, list, start, end);
  }

  string join(const string& delimiter, const list<uint32_t>& list, size_t start, size_t end)
  {
    return join_impl<uint32_t>(delimiter, list, start, end);
  }

  string join(const string& delimiter, const list<uint16_t>& list, size_t start, size_t end)
  {
    return join_impl<uint16_t>(delimiter, list, start, end);
  }

  string join(const string& delimiter, const list<uint8_t>& list, size_t start, size_t end)
  {
    return join_impl<uint8_t>(delimiter, list, start, end);
  }

  string join(const string& delimiter, const list<float>& list, size_t start, size_t end)
  {
    return join_impl<float>(delimiter, list, start, end);
  }
} // namespace mgl_core
