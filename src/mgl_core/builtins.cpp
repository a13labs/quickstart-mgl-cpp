#include "builtins.hpp"

namespace mgl_core
{
  const viewport_2d null_viewport_2d = { 0, 0, 0, 0 };
  const viewport_3d null_viewport_3d = { 0, 0, 0, 0, 0, 0 };
  const size null_size = { 0, 0 };
  const path null_path = path();

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

} // namespace mgl_core