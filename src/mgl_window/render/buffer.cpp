#include "buffer.hpp"
#include <cctype>

namespace mgl_window
{

  namespace render
  {
    const buffer_format null_buffer_format = { "", 0, 0 };
    const static mgl_core::string_list s_valid_divisors = { "v", "i", "r" };
    const static mgl_core::dict<const mgl_core::string, const buffer_format> s_buffer_formats = {
      { "f", buffer_format{ "f", 1, 4, false } },
      { "f1", buffer_format{ "f1", 1, 1, false } },
      { "f2", buffer_format{ "f2", 1, 2, false } },
      { "f4", buffer_format{ "f4", 1, 4, false } },
      { "f8", buffer_format{ "f8", 1, 8, false } },
      // Unsigned Integers
      { "u", buffer_format{ "u", 1, 4, false } },
      { "u1", buffer_format{ "u1", 1, 1, false } },
      { "u2", buffer_format{ "u2", 1, 2, false } },
      { "u4", buffer_format{ "u4", 1, 4, false } },
      // Signed Integer
      { "i", buffer_format{ "i", 1, 4, false } },
      { "i1", buffer_format{ "i1", 1, 1, false } },
      { "i2", buffer_format{ "i2", 1, 2, false } },
      { "i4", buffer_format{ "i4", 1, 4, false } },
      // Normalized float
      { "nf", buffer_format{ "nf", 1, 4, false } },
      { "nf1", buffer_format{ "nf1", 1, 1, false } },
      { "nf2", buffer_format{ "nf2", 1, 2, false } },
      { "nf4", buffer_format{ "nf4", 1, 4, false } },
      // Normalized uint
      { "nu", buffer_format{ "nu", 1, 4, false } },
      { "nu1", buffer_format{ "nu1", 1, 1, false } },
      { "nu2", buffer_format{ "nu2", 1, 2, false } },
      { "nu4", buffer_format{ "nu4", 1, 4, false } },
      // Normalized int
      { "ni", buffer_format{ "ni", 1, 4, false } },
      { "ni1", buffer_format{ "ni1", 1, 1, false } },
      { "ni2", buffer_format{ "ni2", 1, 2, false } },
      { "ni4", buffer_format{ "ni4", 1, 4, false } }
    };

    const buffer_format& get_buffer_format(const mgl_core::string& str)
    {
      if(s_buffer_formats.find(str) == s_buffer_formats.end())
      {
        return null_buffer_format;
      }
      return s_buffer_formats.at(str);
    }

    buffer_format attribute_format(const mgl_core::string& str)
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

      auto fmt_info = get_buffer_format(bformat);
      MGL_CORE_ASSERT(!fmt_info.format_string.empty(), "invalid format {0}", bformat);
      return { str, components, fmt_info.byte_per_component, divisor == "i" };
    }

    buffer_formats parse_attribute_formats(const mgl_core::string& str)
    {
      buffer_formats r;

      auto formats = mgl_core::split(str, ' ');

      for(auto&& f : formats)
      {
        r.push_back(attribute_format(f));
      }

      return r;
    }

    buffer_info::buffer_info(mgl_opengl::buffer_ref buffer,
                             const mgl_core::string& format,
                             const mgl_core::string_list& attrs,
                             bool per_instance)
    {
      m_buffer = buffer;

      m_format = format;
      m_formats = parse_attribute_formats(m_format);
      MGL_CORE_ASSERT(attrs.size() == m_formats.size(), "invalid attributes or format");

      m_attrs = attrs;
      m_per_instance = per_instance;
    }

    int buffer_info::vertex_size()
    {
      int r = 0;
      for(auto&& f : m_formats)
      {
        r += f.bytes_total();
      }
      return r;
    }

  } // namespace render
} // namespace mgl_window