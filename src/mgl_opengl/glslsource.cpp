#include "glslsource.hpp"
#include <regex>

namespace mgl_opengl
{
  static mgl_core::string_list s_shaders_text = {
    "VERTEX_SHADER", "FRAGMENT_SHADER", "GEOMETRY_SHADER", "TESS_CONTROL_SHADER", "TESS_EVALUATION_SHADER",
  };

  static const std::regex s_layout_regex("(layout\\(.+\\)\\))?(\\s+)?(out)(\\s+)(\\w+)(\\s+)(\\w+)");

  glsl_source::glsl_source(const mgl_core::string& source, glsl_source::type type)
  {
    auto src = mgl_core::trim(source);
    auto lines = mgl_core::split(src, '\n');
    MGL_CORE_ASSERT(mgl_core::starts_with(lines[0], "#version"), "#version must appear on the first line");

    m_type = type;
    m_version_code = mgl_core::to_int(lines[0].substr(sizeof("#version")));
    m_source = mgl_core::join('\n', lines, 1);
  }

  const mgl_core::string glsl_source::source(glsl_source::type type, const shader_defines& defines)
  {
    if(!mgl_core::in(s_shaders_text[type], m_source))
    {
      if(type == m_type)
        return source(defines);

      return "";
    }

    mgl_core::string_list str_defines;

    for(const auto& [key, value] : defines)
    {
      str_defines.push_back(mgl_core::format("#define {} {}", key, value));
    }

    auto line = (int)str_defines.size() + 2;

    return mgl_core::format("#version {}\n#define {}\n{}\n#line {}\n{}",
                            m_version_code,
                            s_shaders_text[type],
                            mgl_core::join('\n', str_defines),
                            line,
                            m_source);
  }

  const mgl_core::string glsl_source::source(const shader_defines& defines)
  {
    mgl_core::string_list str_defines;

    for(const auto& [key, value] : defines)
    {
      str_defines.push_back(mgl_core::format("#define {} {}", key, value));
    }

    auto line = (int)str_defines.size() + 1;

    return mgl_core::format("#version {}\n{}\n#line {}\n{}", m_version_code, mgl_core::join('\n', str_defines), line, m_source);
  }

  const mgl_core::string_list glsl_source::outputs()
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

  void glsl_source::handle_includes(int depth, int source_id) { }

} // namespace mgl_opengl