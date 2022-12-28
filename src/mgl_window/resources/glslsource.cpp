#include "glslsource.hpp"
#include <regex>

namespace mgl_window
{
  namespace resources
  {
    static mgl_core::string_list s_shaders_text = {
      "VERTEX_SHADER", "FRAGMENT_SHADER", "GEOMETRY_SHADER", "TESS_CONTROL_SHADER", "TESS_EVALUATION_SHADER",
    };

    static const std::regex s_layout_regex("(layout\\(.+\\)\\))?(\\s+)?(out)(\\s+)(\\w+)(\\s+)(\\w+)");

    ShaderSource::ShaderSource(const mgl_core::string& source)
    {
      m_source = mgl_core::trim_copy(source);
    }

    const mgl_core::string ShaderSource::source(ShaderSource::type type, const shader_defines& defines)
    {
      if(!mgl_core::in(s_shaders_text[type], m_source))
        return "";

      mgl_core::string_list str_defines;

      for(const auto& [key, value] : defines)
      {
        str_defines.push_back(mgl_core::format("#define {} {}", key, value));
      }

      auto line = (int)str_defines.size() + 2;

      return mgl_core::format(
          "#define {}\n{}\n#line {}\n{}", s_shaders_text[type], mgl_core::join("\n", str_defines), line, m_source);
    }

    const mgl_core::string_list ShaderSource::outputs()
    {
      mgl_core::string_list outputs = {};

      std::regex_iterator<mgl_core::string::iterator> it(m_source.begin(), m_source.end(), s_layout_regex);
      std::regex_iterator<mgl_core::string::iterator> end;

      for(; it != end; ++it)
      {
        auto idx = it->size() - 1;
        auto m = *it;
        outputs.push_back(m[idx].str());
      }

      return outputs;
    }

    void ShaderSource::handle_includes(int depth, int source_id) { }

  } // namespace resources
} // namespace mgl_window