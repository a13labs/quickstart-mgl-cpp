#pragma once
#include "mgl_window/builtins.hpp"

namespace mgl::window
{
  namespace render
  {
    enum render_mode
    {
      POINTS = mgl::opengl::render_mode::POINTS,
      LINES = mgl::opengl::render_mode::LINES,
      LINE_LOOP = mgl::opengl::render_mode::LINE_LOOP,
      LINE_STRIP = mgl::opengl::render_mode::LINE_STRIP,
      TRIANGLES = mgl::opengl::render_mode::TRIANGLES,
      TRIANGLE_STRIP = mgl::opengl::render_mode::TRIANGLE_STRIP,
      TRIANGLE_FAN = mgl::opengl::render_mode::TRIANGLE_FAN,
      LINES_ADJACENCY = mgl::opengl::render_mode::LINES_ADJACENCY,
      LINE_STRIP_ADJACENCY = mgl::opengl::render_mode::LINE_STRIP_ADJACENCY,
      TRIANGLES_ADJACENCY = mgl::opengl::render_mode::TRIANGLES_ADJACENCY,
      TRIANGLE_STRIP_ADJACENCY = mgl::opengl::render_mode::TRIANGLE_STRIP_ADJACENCY,
      PATCHES = mgl::opengl::render_mode::PATCHES,

      NONE
    };

    class vertex_array
    {
  public:
      vertex_array(const mgl::core::string& name, render_mode mode = render_mode::TRIANGLES);
      ~vertex_array() = default;

      void render(const mgl::opengl::program_ref& program,
                  render_mode mode = render_mode::NONE,
                  int vertices = -1,
                  int first = 0,
                  int instances = -1);

      void render_indirect(const mgl::opengl::program_ref& program,
                           const mgl::opengl::buffer_ref& buffer,
                           render_mode mode,
                           int count = -1,
                           int first = -1);

      void transform(const mgl::opengl::program_ref& program,
                     const mgl::opengl::buffer_ref& buffer,
                     render_mode mode,
                     int vertices = -1,
                     int first = 0,
                     int instances = -1);

      void index_buffer(const mgl::core::mem_buffer<float>& data, int index_element_size = 4);
      void index_buffer(const mgl::core::mem_buffer<uint32_t>& data, int index_element_size = 4);
      void index_buffer(const mgl::core::mem_buffer<uint16_t>& data, int index_element_size = 4);
      void index_buffer(const mgl::core::mem_buffer<uint8_t>& data, int index_element_size = 4);
      void index_buffer(const mgl::opengl::buffer_ref& buffer, int index_element_size = 4);

      void buffer(const mgl::core::mem_buffer<float>& data, const mgl::core::string& layout, const mgl::core::string_list& attrs);
      void
      buffer(const mgl::core::mem_buffer<uint32_t>& data, const mgl::core::string& layout, const mgl::core::string_list& attrs);
      void
      buffer(const mgl::core::mem_buffer<uint16_t>& data, const mgl::core::string& layout, const mgl::core::string_list& attrs);
      void
      buffer(const mgl::core::mem_buffer<uint8_t>& data, const mgl::core::string& layout, const mgl::core::string_list& attrs);
      void buffer(const mgl::opengl::buffer_ref& buffer, const mgl::core::string& layout, const mgl::core::string_list& attrs);

      mgl::opengl::vertex_array_ref instance(const mgl::opengl::program_ref& program);

      mgl::opengl::context_ref context();

  private:
      mgl::core::string m_name;
      render_mode m_mode;
      mgl::core::dict<int, mgl::opengl::vertex_array_ref> m_vao_cache;
      vertex_buffers_list m_buffers;
      int m_vertex_count;
      mgl::opengl::buffer_ref m_index_buffer;
      int m_index_element_size;
    };

    inline mgl::opengl::context_ref vertex_array::context()
    {
      return mgl::window::current_context();
    }

    inline void vertex_array::index_buffer(const mgl::core::mem_buffer<float>& data, int index_element_size)
    {
      auto b = mgl::window::current_context()->buffer(data);
      index_buffer(b, index_element_size);
    }

    inline void vertex_array::index_buffer(const mgl::core::mem_buffer<uint32_t>& data, int index_element_size)
    {
      auto b = mgl::window::current_context()->buffer(data);
      index_buffer(b, index_element_size);
    }

    inline void vertex_array::index_buffer(const mgl::core::mem_buffer<uint16_t>& data, int index_element_size)
    {
      auto b = mgl::window::current_context()->buffer(data);
      index_buffer(b, index_element_size);
    }

    inline void vertex_array::index_buffer(const mgl::core::mem_buffer<uint8_t>& data, int index_element_size)
    {
      auto b = mgl::window::current_context()->buffer(data);
      index_buffer(b, index_element_size);
    }

    inline void vertex_array::index_buffer(const mgl::opengl::buffer_ref& buffer, int index_element_size)
    {
      m_index_buffer = buffer;
      m_index_element_size = index_element_size;
    }

    inline void vertex_array::buffer(const mgl::core::mem_buffer<float>& data,
                                     const mgl::core::string& layout,
                                     const mgl::core::string_list& attrs)
    {
      auto b = mgl::window::current_context()->buffer(data);
      buffer(b, layout, attrs);
    }

    inline void vertex_array::buffer(const mgl::core::mem_buffer<uint32_t>& data,
                                     const mgl::core::string& layout,
                                     const mgl::core::string_list& attrs)
    {
      auto b = mgl::window::current_context()->buffer(data);
      buffer(b, layout, attrs);
    }

    inline void vertex_array::buffer(const mgl::core::mem_buffer<uint16_t>& data,
                                     const mgl::core::string& layout,
                                     const mgl::core::string_list& attrs)
    {
      auto b = mgl::window::current_context()->buffer(data);
      buffer(b, layout, attrs);
    }

    inline void vertex_array::buffer(const mgl::core::mem_buffer<uint8_t>& data,
                                     const mgl::core::string& layout,
                                     const mgl::core::string_list& attrs)
    {
      auto b = mgl::window::current_context()->buffer(data);
      buffer(b, layout, attrs);
    }

  } // namespace render
} // namespace  mgl::window
