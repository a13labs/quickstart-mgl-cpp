#pragma once
#include "mgl_graphics/shader.hpp"

class custom_shader : public mgl::graphics::shader
{
  public:
  custom_shader() = default;

  virtual void prepare() override final;
  virtual mgl::window::api::program_ref& native() override final;
  virtual mgl::string_list& attributes() override final;
  virtual void unload() override final;
  virtual void load() override final;

  private:
  mgl::window::api::program_ref m_program;
};
