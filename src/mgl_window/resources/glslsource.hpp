#pragma once
#include "mgl_window/builtins.hpp"

namespace mgl_window
{
  namespace resources
  {
    class ShaderSource
    {
  public:
      enum type
      {
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        GEOMETRY_SHADER,
        TESS_EVALUATION_SHADER,
        TESS_CONTROL_SHADER,
      };

      ShaderSource(const mgl_core::string& source);
      ~ShaderSource() = default;

      const mgl_core::string source(ShaderSource::type type, const shader_defines& defines = {});
      const mgl_core::string_list outputs();

  private:
      void handle_includes(int depth = 0, int source_id = 0);

      mgl_core::string m_source;
      mgl_core::string_list m_attributes;
    };
  } // namespace resources
} // namespace mgl_window