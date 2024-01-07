#pragma once
#include "mgl_graphics/layers/gui.hpp"
#include "mgl_graphics/layers/render.hpp"

struct ImVec4;

extern ImVec4 clear_color;

class render_layer : public mgl::graphics::layers::render_layer
{
  public:
  render_layer()
      : mgl::graphics::layers::render_layer("Example Render Layer")
  { }

  virtual void on_attach() override;

  virtual void on_detach() override;

  virtual void render_prepare() override;
};

class gui_layer : public mgl::graphics::layers::gui_layer
{
  public:
  gui_layer()
      : mgl::graphics::layers::gui_layer("Example GUI Layer")
  { }
  virtual void draw_ui(float time, float frame_time) override;
};
