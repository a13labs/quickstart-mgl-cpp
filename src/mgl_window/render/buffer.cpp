#include "buffer.hpp"
#include <cctype>

namespace mgl_window
{

  namespace render
  {
    const buffer_element null_buffer_format = { "", 0, 0 };
    const static mgl_core::string_list s_valid_divisors = { "v", "i", "r" };
    const static mgl_core::dict<const mgl_core::string, const buffer_element> s_buffer_formats = {
      { "f", buffer_element{ "f", 1, 4, false } },
      { "f1", buffer_element{ "f1", 1, 1, false } },
      { "f2", buffer_element{ "f2", 1, 2, false } },
      { "f4", buffer_element{ "f4", 1, 4, false } },
      { "f8", buffer_element{ "f8", 1, 8, false } },
      // Unsigned Integers
      { "u", buffer_element{ "u", 1, 4, false } },
      { "u1", buffer_element{ "u1", 1, 1, false } },
      { "u2", buffer_element{ "u2", 1, 2, false } },
      { "u4", buffer_element{ "u4", 1, 4, false } },
      // Signed Integer
      { "i", buffer_element{ "i", 1, 4, false } },
      { "i1", buffer_element{ "i1", 1, 1, false } },
      { "i2", buffer_element{ "i2", 1, 2, false } },
      { "i4", buffer_element{ "i4", 1, 4, false } },
      // Normalized float
      { "nf", buffer_element{ "nf", 1, 4, false } },
      { "nf1", buffer_element{ "nf1", 1, 1, false } },
      { "nf2", buffer_element{ "nf2", 1, 2, false } },
      { "nf4", buffer_element{ "nf4", 1, 4, false } },
      // Normalized uint
      { "nu", buffer_element{ "nu", 1, 4, false } },
      { "nu1", buffer_element{ "nu1", 1, 1, false } },
      { "nu2", buffer_element{ "nu2", 1, 2, false } },
      { "nu4", buffer_element{ "nu4", 1, 4, false } },
      // Normalized int
      { "ni", buffer_element{ "ni", 1, 4, false } },
      { "ni1", buffer_element{ "ni1", 1, 1, false } },
      { "ni2", buffer_element{ "ni2", 1, 2, false } },
      { "ni4", buffer_element{ "ni4", 1, 4, false } }
    };

    const buffer_element& to_buffer_element(const mgl_core::string& str)
    {
      if(s_buffer_formats.find(str) == s_buffer_formats.end())
      {
        return null_buffer_format;
      }
      return s_buffer_formats.at(str);
    }

    buffer_element attribute_format(const mgl_core::string& str)
    {
      auto parts = mgl_core::split(str, '/');

      auto format = parts[0];
      mgl_core::string divisor;

      if(parts.size() > 1)
      {
        MGL_CORE_ASSERT(parts.size() == 2, "invalid divisor");
        MGL_CORE_ASSERT(mgl_core::in(parts[1], s_valid_divisors), "invalid divisor");
        divisor = parts[1];
      }

      int components = 1;
      mgl_core::string bformat = format;
      if(!isalpha(format[0]))
      {
        parts = mgl_core::re_split(format, "([fiudn])");
        components = mgl_core::to_int(parts[0]);
        bformat = format.substr(parts[0].size());
      }

      auto fmt_info = to_buffer_element(bformat);
      MGL_CORE_ASSERT(!fmt_info.format_string.empty(), "invalid format {0}", bformat);
      return { str, components, fmt_info.byte_per_component, divisor == "i" };
    }

    buffer_layout parse_layout(const mgl_core::string& str)
    {
      buffer_layout r;

      auto formats = mgl_core::split(str, ' ');

      for(auto&& f : formats)
      {
        r.push_back(attribute_format(f));
      }

      return r;
    }

    vertex_buffer::vertex_buffer(mgl_opengl::buffer_ref buffer,
                                 const mgl_core::string& format,
                                 const mgl_core::string_list& attrs,
                                 bool per_instance)
    {
      MGL_CORE_ASSERT(m_buffer != nullptr, "invalid buffer");
      MGL_CORE_ASSERT(!m_buffer->released(), "buffer already released");
      m_buffer = buffer;

      m_layout_str = format;
      m_layout = parse_layout(m_layout_str);
      MGL_CORE_ASSERT(attrs.size() == m_layout.size(), "invalid attributes or format");

      // Sanity check byte size
      MGL_CORE_ASSERT(m_buffer->size() % size() == 0, "Buffer size not aligned with layout");
      m_vertices = m_buffer->size() / size();

      m_attrs = attrs;
      m_per_instance = per_instance;
    }

    size_t vertex_buffer::size()
    {
      size_t r = 0;
      for(auto&& f : m_layout)
      {
        r += f.size();
      }
      return r;
    }

  } // namespace render
} // namespace mgl_window