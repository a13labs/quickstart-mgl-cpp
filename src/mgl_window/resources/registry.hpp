#pragma once
#include "mgl_window/mgl_window.hpp"

namespace mgl_window
{
  namespace resources
  {
    struct mipmap_levels
    {
      int base;
      int max_level;
    };

    extern mipmap_levels null_mipmap_levels;

    inline bool operator==(const mipmap_levels& lhs, const mipmap_levels& rhs)
    {
      return lhs.base == rhs.base && lhs.max_level == rhs.max_level;
    }

    inline bool operator!=(const mipmap_levels& lhs, const mipmap_levels& rhs)
    {
      return !(lhs == rhs);
    }

    struct texture_2d_load_opts
    {
      bool flip_x;
      bool flip_y;
      mgl_window::resources::mipmap_levels mipmap_levels;
      float anisotropy;
    };

    extern texture_2d_load_opts texture_2d_load_defaults;

    struct program_load_opts
    {
      bool reloadable;
      shader_defines defines;
      mgl_opengl::shaders_outputs outputs;
    };

    extern program_load_opts program_load_defaults;

    struct data_load_opts
    {
      mgl_core::openmode mode;
    };

    extern data_load_opts data_load_defaults;

    bool register_dir(const mgl_core::string& path);
    bool register_program_dir(const mgl_core::string& path);
    bool register_texture_dir(const mgl_core::string& path);
    bool register_scene_dir(const mgl_core::string& path);
    bool register_data_dir(const mgl_core::string& path);

    mgl_core::ref<mgl_opengl::Texture> load_texture_2d(const mgl_core::string& path,
                                                       const texture_2d_load_opts& opts = texture_2d_load_defaults);
    bool
    load_data_file(const mgl_core::string& path, mgl_core::input_file& file, const data_load_opts& opts = data_load_defaults);
    mgl_core::ref<mgl_opengl::Program> load_program(const mgl_core::string& path,
                                                    const program_load_opts& opts = program_load_defaults);
    mgl_core::ref<mgl_opengl::Program> load_program(mgl_core::string vertex_shader,
                                                    mgl_core::string fragment_shader = "",
                                                    mgl_core::string geometry_shader = "",
                                                    mgl_core::string tess_control_shader = "",
                                                    mgl_core::string tess_evaluation_shader = "",
                                                    const program_load_opts& opts = program_load_defaults);

  } // namespace resources
} // namespace mgl_window