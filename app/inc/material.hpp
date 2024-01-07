#pragma once
#include "mgl_graphics/material.hpp"

class custom_material : public mgl::graphics::material
{
  public:
  custom_material();

  virtual void prepare() override final;
  virtual mgl::window::api::program_ref& program() override final;
  virtual mgl::string_list& attributes() override final;
  virtual void release() override final;
};
