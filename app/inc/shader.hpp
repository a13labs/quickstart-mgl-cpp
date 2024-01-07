#pragma once
#include "mgl_graphics/shader.hpp"

class custom_shader : public mgl::graphics::shader
{
  public:
  custom_shader();

  virtual void prepare() override final;
  virtual mgl::window::api::program_ref& program() override final;
  virtual mgl::string_list& attributes() override final;
  virtual void release() override final;
};
