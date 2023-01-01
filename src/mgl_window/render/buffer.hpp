#pragma once
#include "mgl_window/builtins.hpp"

namespace mgl::window
{
  namespace render
  {
    struct buffer_element
    {
      const mgl::core::string format_string;
      const int components;
      const int byte_per_component;
      const bool per_instance;

      size_t size();
    };

    extern const buffer_element null_buffer_format;

    buffer_layout parse_layout(const mgl::core::string& str);
    const buffer_element& to_buffer_element(const mgl::core::string& str);

    class vertex_buffer
    {

  public:
      vertex_buffer(mgl::opengl::buffer_ref buffer,
                    const mgl::core::string& format,
                    const mgl::core::string_list& attrs = {},
                    bool per_instance = false);
      ~vertex_buffer() = default;

      size_t size();
      const buffer_layout& layout() const;
      const mgl::core::string_list& attributes() const;
      bool has_attribute(const mgl::core::string& name);
      const mgl::opengl::buffer_ref& buffer() const;
      bool per_instance();
      int vertices();

      operator mgl::opengl::vertex_buffer();

  private:
      mgl::opengl::buffer_ref m_buffer;
      mgl::core::string m_layout_str;
      buffer_layout m_layout;
      mgl::core::string_list m_attrs;
      bool m_per_instance;
      int m_vertices;
    };

    inline size_t buffer_element::size()
    {
      return components * byte_per_component;
    }

    inline const buffer_layout& vertex_buffer::layout() const
    {
      return m_layout;
    }

    inline const mgl::core::string_list& vertex_buffer::attributes() const
    {
      return m_attrs;
    }

    inline bool vertex_buffer::has_attribute(const mgl::core::string& name)
    {
      return mgl::core::in(name, m_attrs);
    }

    inline const mgl::opengl::buffer_ref& vertex_buffer::buffer() const
    {
      return m_buffer;
    }

    inline vertex_buffer::operator mgl::opengl::vertex_buffer()
    {
      return { m_buffer, m_layout_str, m_attrs };
    }

    inline bool vertex_buffer::per_instance()
    {
      return m_per_instance;
    }

    inline int vertex_buffer::vertices()
    {
      return m_vertices;
    }
  } // namespace render
} // namespace  mgl::window
