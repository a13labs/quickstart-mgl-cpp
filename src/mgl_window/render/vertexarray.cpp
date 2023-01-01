#include "vertexarray.hpp"
#include "buffer.hpp"

namespace mgl::window
{
  namespace render
  {
    vertex_array::vertex_array(const mgl::core::string& name, render_mode mode)
    {
      m_name = name;
      m_mode = mode;
      m_vertex_count = 0;
      m_index_buffer = nullptr;
      m_index_element_size = 0;
    }

    void vertex_array::render(const mgl::opengl::program_ref& program, render_mode mode, int vertices, int first, int instances)
    {
      auto vao = instance(program);

      if(mode == render_mode::NONE)
        mode = m_mode;

      vao->render(mgl::opengl::render_mode(mode), vertices, first, instances);
    }

    void vertex_array::render_indirect(
        const mgl::opengl::program_ref& program, const mgl::opengl::buffer_ref& buffer, render_mode mode, int count, int first)
    {
      auto vao = instance(program);

      if(mode == render_mode::NONE)
        mode = m_mode;

      vao->render_indirect(buffer, mgl::opengl::render_mode(mode), count, first);
    }

    void vertex_array::transform(const mgl::opengl::program_ref& program,
                                 const mgl::opengl::buffer_ref& buffer,
                                 render_mode mode,
                                 int vertices,
                                 int first,
                                 int instances)
    {
      auto vao = instance(program);

      if(mode == render_mode::NONE)
        mode = m_mode;

      vao->transform(buffer, mgl::opengl::render_mode(mode), vertices, first, instances);
    }

    mgl::opengl::vertex_array_ref vertex_array::instance(const mgl::opengl::program_ref& program)
    {

      if(m_vao_cache.find(program->glo()) != m_vao_cache.end())
      {
        return m_vao_cache.at(program->glo());
      }

      mgl::core::string_list program_attrs = program->attributes(false);

#ifdef MGL_DEBUG
      mgl::core::list<bool> has_attr_v;

      for(auto&& attr : program_attrs)
      {
        for(auto&& buffer : m_buffers)
        {
          has_attr_v.push_back(buffer.has_attribute(attr));
        }
      }

      auto has_attr = std::any_of(has_attr_v.begin(), has_attr_v.end(), [](bool b) { return b; });
      MGL_CORE_ASSERT(has_attr, "vertex_array doesn't have attribute for program.");
#endif

      mgl::opengl::vertex_buffer_list v_buffers;

      mgl::core::string_list layout;
      mgl::core::string_list attrs;

      for(auto&& buffer : m_buffers)
      {
        for(auto&& p : mgl::core::zip(buffer.layout(), buffer.attributes()))
        {
          auto attr = p.second;
          auto element = p.first;

          if(!mgl::core::in(attr, program_attrs))
          {
            layout.push_back(mgl::core::format("{}x{}", element.components, element.byte_per_component));
            continue;
          }

          layout.push_back(element.format_string);
          attrs.push_back(attr);

          std::remove(program_attrs.begin(), program_attrs.end(), attr);
        }

        mgl::core::string v_layout = mgl::core::format("{}{}", mgl::core::join(' ', layout), buffer.per_instance() ? "/i" : "");
        v_buffers.push_back({ buffer.buffer(), v_layout, attrs });
      }

      MGL_CORE_ASSERT(program_attrs.size() == 0, "Missing buffer mapping");

      mgl::opengl::vertex_array_ref vao = nullptr;
      vao = mgl::window::current_context()->vertex_array(program, v_buffers, m_index_buffer, m_index_element_size);

      m_vao_cache.insert({ program->glo(), vao });

      return vao;
    }

    void vertex_array::buffer(const mgl::opengl::buffer_ref& buffer,
                              const mgl::core::string& layout,
                              const mgl::core::string_list& attrs)
    {
#ifdef MGL_DEBUG
      auto elements = mgl::core::split(layout, ' ');
      MGL_CORE_ASSERT(elements.size() == attrs.size(), "layout does not match attributes");
#endif

      m_buffers.push_back(vertex_buffer(buffer, layout, attrs));
      m_vertex_count += m_buffers.end()->vertices();
    }

  } // namespace render
} // namespace  mgl::window