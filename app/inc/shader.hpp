#pragma once
#include "mgl_graphics/shader.hpp"

class custom_shader : public mgl::graphics::shader
{
  public:
  custom_shader() = default;

  virtual void prepare() override final;
  virtual mgl::string_list& attributes() override final;
  virtual void load() override final;
};
