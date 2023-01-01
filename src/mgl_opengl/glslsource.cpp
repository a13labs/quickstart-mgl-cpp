#include "glslsource.hpp"
#include <regex>

namespace mgl::opengl
{
  static mgl::core::string_list s_shaders_text = {
    "VERTEX_SHADER", "FRAGMENT_SHADER", "GEOMETRY_SHADER", "TESS_CONTROL_SHADER", "TESS_EVALUATION_SHADER",
  };

  static const std::regex s_layout_regex("(layout\\(.+\\)\\))?(\\s+)?(out)(\\s+)(\\w+)(\\s+)(\\w+)");

  glsl_source::glsl_source(const mgl::core::string& source, glsl_source::type type)
  {
    auto src = mgl::core::trim(source);
    auto lines = mgl::core::split(src, '\n');
    MGL_CORE_ASSERT(mgl::core::starts_with(lines[0], "#version"), "#version must appear on the first line");

    m_type = type;
    m_version_code = mgl::core::to_int(lines[0].substr(sizeof("#version")));
    m_source = mgl::core::join('\n', lines, 1);
  }

  const mgl::core::string glsl_source::source(glsl_source::type type, const shader_defines& defines)
  {
    if(!mgl::core::in(s_shaders_text[type], m_source))
    {
      if(type == m_type)
        return source(defines);

      return "";
    }

    mgl::core::string_list str_defines;

    for(const auto& [key, value] : defines)
    {
      str_defines.push_back(mgl::core::format("#define {} {}", key, value));
    }

    auto line = (int)str_defines.size() + 2;

    return mgl::core::format("#version {}\n#define {}\n{}\n#line {}\n{}",
                             m_version_code,
                             s_shaders_text[type],
                             mgl::core::join('\n', str_defines),
                             line,
                             m_source);
  }

  const mgl::core::string glsl_source::source(const shader_defines& defines)
  {
    mgl::core::string_list str_defines;

    for(const auto& [key, value] : defines)
    {
      str_defines.push_back(mgl::core::format("#define {} {}", key, value));
    }

    auto line = (int)str_defines.size() + 1;

    return mgl::core::format("#version {}\n{}\n#line {}\n{}", m_version_code, mgl::core::join('\n', str_defines), line, m_source);
  }

  const mgl::core::string_list glsl_source::outputs()
  {
    mgl::core::string_list outputs = {};

    std::regex_iterator<mgl::core::string::iterator> it(m_source.begin(), m_source.end(), s_layout_regex);
    std::regex_iterator<mgl::core::string::iterator> end;

    for(; it != end; ++it)
    {
      auto idx = it->size() - 1;
      auto m = *it;
      outputs.push_back(m[idx].str());
    }

    return outputs;
  }

  void glsl_source::handle_includes(int depth, int source_id) { }

} // namespace  mgl::opengl