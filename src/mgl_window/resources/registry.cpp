#include "registry.hpp"
#include "glslsource.hpp"
#include "stb/stb_image.hpp"

namespace mgl_window
{
  namespace resources
  {
    struct settings
    {
      mgl_core::path_list textures_dirs;
      mgl_core::path_list programs_dirs;
    };

    static settings s_settings;

    mipmap_levels null_mipmap_levels = { 0, 0 };
    texture_2d_load_opts texture_2d_load_defaults = { false, true, null_mipmap_levels, 1.0 };
    program_load_opts program_load_defaults = { true, {}, {} };
    data_load_opts data_load_defaults = { mgl_core::input_file::in | mgl_core::input_file::binary };

    bool append_unique_path(const mgl_core::string& value, mgl_core::path_list& list)
    {

      auto path = mgl_core::path(value);

      if(path.is_relative())
      {
        MGL_CORE_ERROR("resources: path is relative: {0}", value);
        return false;
      }

      if(!std::filesystem::exists(path))
      {
        MGL_CORE_ERROR("resources: path does not exist: {0}", value);
        return false;
      }

      if(!std::filesystem::is_directory(path))
      {
        MGL_CORE_ERROR("resources: path is not a directory: {0}", value);
        return false;
      }

      auto it = std::find(list.begin(), list.end(), path);

      if(it == list.end())
        return true;

      list.push_back(path);
      return true;
    }

    const mgl_core::path& find(const mgl_core::string& value, mgl_core::path_list& list)
    {
      for(auto&& base : list)
      {
        auto path = std::filesystem::path() / value;

        if(std::filesystem::exists(path))
        {
          return base;
        }
      }

      return mgl_core::null_path;
    }

    bool register_dir(const mgl_core::string& path)
    {
      auto result = register_program_dir(path);
      result &= register_texture_dir(path);
      result &= register_scene_dir(path);
      result &= register_data_dir(path);
      return result;
    }

    bool register_program_dir(const mgl_core::string& path)
    {
      return append_unique_path(path, s_settings.programs_dirs);
    }

    bool register_texture_dir(const mgl_core::string& path)
    {
      return append_unique_path(path, s_settings.textures_dirs);
    }

    bool register_scene_dir(const mgl_core::string& path)
    {
      return false;
    }

    bool register_data_dir(const mgl_core::string& path)
    {
      return false;
    }

    static void stbi_kyon_horizontal_flip(void* image, int w, int h, int components)
    {
      size_t line_bytes = (size_t)w * components;
      stbi_uc temp[line_bytes];
      stbi_uc* bytes = (stbi_uc*)image;
      int lpos, rpos;
      for(int col = 0; col < h; col++)
      {
        stbi_uc* line = bytes + col * line_bytes;
        memcpy(&temp, line, line_bytes);
        for(int row = 0; row < w; row++)
        {
          lpos = row * components;
          rpos = line_bytes - row * components - 1;
          line[lpos] = temp[rpos - 3];
          line[lpos + 1] = temp[rpos - 2];
          line[lpos + 2] = temp[rpos - 1];
          line[lpos + 3] = temp[rpos];
        }
      }
    }

    mgl_core::ref<mgl_opengl::Texture> load_texture_2d(const mgl_core::string& path, const texture_2d_load_opts& opts)
    {
      MGL_CORE_ASSERT(Window::current().context(), "No context!");
      const auto ctx = Window::current().context();

      auto base_path = find(path, s_settings.textures_dirs);

      if(base_path.empty())
      {
        MGL_CORE_ERROR("load_texture_2d: path not found: {0}", path);
        return nullptr;
      }

      auto full_path = base_path / path;

      int width, height, components;
      stbi_set_flip_vertically_on_load(opts.flip_y);
      unsigned char* data = stbi_load(full_path.c_str(), &width, &height, &components, 0);

      if(opts.flip_x)
      {
        stbi_kyon_horizontal_flip(data, width, height, components);
      }

      auto texture = ctx->texture2d(width, height, components, data);
      stbi_image_free(data);

      if(opts.mipmap_levels != null_mipmap_levels)
      {
        texture->build_mipmaps(opts.mipmap_levels.base, opts.mipmap_levels.max_level);
      }

      texture->set_anisotropy(opts.anisotropy);

      return texture;
    }

    bool load_data_file(const mgl_core::string& path, mgl_core::input_file& file, const data_load_opts& opts)
    {
      auto base_path = find(path, s_settings.textures_dirs);

      if(base_path.empty())
      {
        MGL_CORE_ERROR("load_data_file: path not found: {0}", path);
        return false;
      }

      auto full_path = base_path / path;

      file.open(full_path, opts.mode);
      return true;
    }

    mgl_core::ref<mgl_opengl::Program> load_program(const mgl_core::string& path, const program_load_opts& opts)
    {
      MGL_CORE_ASSERT(Window::current().context(), "No context!");
      const auto ctx = Window::current().context();

      auto base_path = find(path, s_settings.textures_dirs);

      if(base_path.empty())
      {
        MGL_CORE_ERROR("load_program: path not found: {0}", path);
        return nullptr;
      }

      auto full_path = base_path / path;

      mgl_core::input_file shader_file(full_path, mgl_core::input_file::in);
      mgl_core::string shader_text((std::istreambuf_iterator<char>(shader_file)), std::istreambuf_iterator<char>());

      ShaderSource shader_source(shader_text);

      mgl_opengl::glsl_sources glsl = {
        shader_source.source(ShaderSource::type::VERTEX_SHADER, opts.defines),
        shader_source.source(ShaderSource::type::FRAGMENT_SHADER, opts.defines),
        shader_source.source(ShaderSource::type::GEOMETRY_SHADER, opts.defines),
        shader_source.source(ShaderSource::type::TESS_CONTROL_SHADER, opts.defines),
        shader_source.source(ShaderSource::type::TESS_EVALUATION_SHADER, opts.defines),
      };

      mgl_opengl::shaders_outputs outputs = opts.outputs;

      if(outputs.size() == 0 && glsl.fragment().empty())
      {
        MGL_CORE_ASSERT(!glsl.vertex().empty() || !glsl.geometry().empty(), "Required vertex or geometry shader");
        outputs = shader_source.outputs();
      }

      return ctx->program(glsl, outputs);
    }

    mgl_core::ref<mgl_opengl::Program> load_program(mgl_core::string vertex_shader,
                                                    mgl_core::string fragment_shader,
                                                    mgl_core::string geometry_shader,
                                                    mgl_core::string tess_control_shader,
                                                    mgl_core::string tess_evaluation_shader,
                                                    const program_load_opts& opts)
    {
      MGL_CORE_ASSERT(Window::current().context(), "No context!");
      const auto ctx = Window::current().context();

      mgl_opengl::glsl_sources glsl = {
        vertex_shader, fragment_shader, geometry_shader, tess_control_shader, tess_evaluation_shader,
      };

      mgl_opengl::shaders_outputs outputs = opts.outputs;

      return ctx->program(glsl, outputs);
    }

  } // namespace resources
} // namespace mgl_window