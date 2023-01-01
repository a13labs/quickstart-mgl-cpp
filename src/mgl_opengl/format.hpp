#pragma once
namespace mgl::opengl
{
  struct format_node
  {
    int size;
    int count;
    int type;
    bool normalize;
  };

  struct format_info
  {
    int size;
    int nodes;
    int divisor;
    bool valid;

    static format_info invalid()
    {
      format_info invalid;
      invalid.size = 0;
      invalid.nodes = 0;
      invalid.divisor = 0;
      invalid.valid = false;
      return invalid;
    }
  };

  struct format_iterator
  {
    const char* ptr;
    format_node node;

    format_iterator(const char* str);

    format_info info();
    format_node* next();
  };
} // namespace  mgl::opengl
