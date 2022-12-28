#include "string.hpp"

namespace mgl_core
{
  void split(const string& str, const string& delimiter, string_list& list)
  {
    auto tmp = str;
    size_t pos = 0;
    mgl_core::string token;
    while((pos = tmp.find(delimiter)) != mgl_core::string::npos)
    {
      auto line = tmp.substr(0, pos);
      list.push_back(line);
      tmp.erase(0, pos + delimiter.length());
    }
  }

  string join(const string& delimiter, const string_list& list)
  {
    std::string ret;
    for(const auto& s : list)
    {
      if(!ret.empty())
        ret += delimiter;
      ret += s;
    }
    return ret;
  }

  template <typename t>
  inline string join_impl(const string& delimiter, const list<t>& list)
  {
    std::string ret;
    for(const auto& s : list)
    {
      if(!ret.empty())
        ret += delimiter;
      ret += std::to_string(s);
    }
    return ret;
  }

  string join(const string& delimiter, const list<int32_t>& list)
  {
    return join_impl<int32_t>(delimiter, list);
  }

  string join(const string& delimiter, const list<int16_t>& list)
  {
    return join_impl<int16_t>(delimiter, list);
  }

  string join(const string& delimiter, const list<int8_t>& list)
  {
    return join_impl<int8_t>(delimiter, list);
  }

  string join(const string& delimiter, const list<uint32_t>& list)
  {
    return join_impl<uint32_t>(delimiter, list);
  }

  string join(const string& delimiter, const list<uint16_t>& list)
  {
    return join_impl<uint16_t>(delimiter, list);
  }

  string join(const string& delimiter, const list<uint8_t>& list)
  {
    return join_impl<uint8_t>(delimiter, list);
  }

  string join(const string& delimiter, const list<float>& list)
  {
    return join_impl<float>(delimiter, list);
  }
} // namespace mgl_core
