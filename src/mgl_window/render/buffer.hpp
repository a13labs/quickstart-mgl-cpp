#pragma once
#include "mgl_window/builtins.hpp"

namespace mgl_window
{
  namespace render
  {
    struct buffer_format
    {
      const mgl_core::string format_string;
      const int components;
      const int byte_per_component;
      const bool per_instance;

      int bytes_total();
    };

    extern const buffer_format null_buffer_format;
    using buffer_formats = mgl_core::list<buffer_format>;

    buffer_formats parse_attribute_formats(const mgl_core::string& str);
    const buffer_format& get_buffer_format(const mgl_core::string& str);

    class buffer_info
    {

  public:
      buffer_info(mgl_opengl::buffer_ref buffer,
                  const mgl_core::string& format,
                  const mgl_core::string_list& attrs = {},
                  bool per_instance = false);
      ~buffer_info();

      int vertex_size();
      mgl_opengl::vertex_buffer vertex_data();
      const buffer_formats& formats() const;
      bool has_attribute(const mgl_core::string& name);

  private:
      mgl_opengl::buffer_ref m_buffer;
      mgl_core::string m_format;
      buffer_formats m_formats;
      mgl_core::string_list m_attrs;
      bool m_per_instance;
    };

    inline int buffer_format::bytes_total()
    {
      return components * byte_per_component;
    }

    inline mgl_opengl::vertex_buffer buffer_info::vertex_data()
    {
      return { m_buffer, m_format, m_attrs };
    }

    inline const buffer_formats& buffer_info::formats() const
    {
      return m_formats;
    }

    inline bool buffer_info::has_attribute(const mgl_core::string& name)
    {
      return mgl_core::in(name, m_attrs);
    }

  } // namespace render
} // namespace mgl_window
