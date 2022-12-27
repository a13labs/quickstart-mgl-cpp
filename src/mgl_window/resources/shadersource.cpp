#include "shadersource.hpp"
#include <regex>

namespace mgl_window
{
  namespace resources
  {
    static mgl_core::string_list s_shaders_text = {
      "VERTEX_SHADER",
      "GEOMETRY_SHADER",
      "FRAGMENT_SHADER"
      "TESS_CONTROL_SHADER",
      "TESS_EVALUATION_SHADER",
    };

    static const std::regex s_layout_regex("(layout\\(.+\\)\\))?(\\s+)?(out)(\\s+)(\\w+)(\\s+)(\\w+)");

    ShaderSource::ShaderSource(const mgl_core::string& source)
    {
      m_source = source;
    }

    const mgl_core::string ShaderSource::source(ShaderSource::type type, const shader_defines& defines)
    {
      return "#include " + s_shaders_text[type] + " 1\n#line 2\n" + m_source;
    }

    const mgl_core::string_list ShaderSource::attributes()
    {
      mgl_core::string_list attributes = {};

      std::regex_iterator<mgl_core::string::iterator> it(m_source.begin(), m_source.end(), s_layout_regex);
      std::regex_iterator<mgl_core::string::iterator> end;

      for(; it != end; ++it)
      {
        auto idx = it->size() - 1;
        auto m = *it;
        attributes.push_back(m[idx].str());
      }

      return attributes;
    }

    void ShaderSource::handle_includes(int depth, int source_id) { }

  } // namespace resources
} // namespace mgl_window