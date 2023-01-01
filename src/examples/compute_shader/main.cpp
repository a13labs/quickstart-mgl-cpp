#include "NumCpp/Functions/frombuffer.hpp"
#include "NumCpp/Functions/multiply.hpp"
#include "NumCpp/Functions/ones.hpp"
#include "NumCpp/Functions/zeros.hpp"
#include "NumCpp/Random/uniform.hpp"
#include "gif.hpp"
#include "mgl_opengl/mgl_opengl.hpp"

int main(int argc, char* argv[])
{
  mgl::core::log::init();

  // author: minu jeong
  auto code = mgl::opengl::glsl_source(R"(
#version 440
layout(local_size_x=X, local_size_y=Y, local_size_z=Z) in;
layout (std430, binding=0) buffer in_0
{
    vec4 inxs[1];
};

layout (std430, binding=1) buffer out_0
{
    vec4 outxs[1];
};

layout (std430, binding=2) buffer uv_0
{
    vec2 uvs[1];
};

#define win_width 5
#define win_height 5
#define win_wh 25
vec4 window[win_wh] = {
    // should manually initialize this
    vec4(0), vec4(0), vec4(0), vec4(0), vec4(0),
    vec4(0), vec4(0), vec4(0), vec4(0), vec4(0),
    vec4(0), vec4(0), vec4(0), vec4(0), vec4(0),
    vec4(0), vec4(0), vec4(0), vec4(0), vec4(0),
    vec4(0), vec4(0), vec4(0), vec4(0), vec4(0)
};

void main()
{
    // define consts
    const int x = int(gl_LocalInvocationID.x);
    const int y = int(gl_WorkGroupID.x);
    const int frag_i = x + y * W;

    int ignored = 0;
    // read window
    for (int win_x = 0; win_x < win_width; win_x++)
    {
        for (int win_y = 0; win_y < win_height; win_y++)
        {
            int win_i = win_y * win_width + win_x;
            int wox = win_x - win_width / 2;
            int woy = win_y - win_height / 2;
            int src_i = x + wox + (y + woy) * W;
            if (src_i < 0 || src_i > W * H)
            {
                window[win_i] = vec4(0, 0, 0, 0);
                ignored++;
                continue;
            }

            window[win_i] = inxs[src_i];
        }
    }

    // simple bubble sort to find median
    while(true)
    {
        bool is_swapped = false;
        for (int win_ii = win_wh - 1; win_ii > 1; win_ii--)
        {
            vec4 now = window[win_ii];
            if (now.w == 0.0) { continue; }
            if (length(window[win_ii - 1]) > length(now))
            {
                // swap
                window[win_ii] = window[win_ii - 1];
                window[win_ii - 1] = now;
                is_swapped = true;
            }
        }

        if (!is_swapped)
        {
            break;
        }
    }
    int median_i = win_wh / 2 + ignored / 2;
    vec4 median = window[median_i];

    // write to buffer
    outxs[frag_i] = vec4(median.xyz, 1.0);
}    
  )");

  uint32_t W = 512;
  uint32_t H = 256;
  uint32_t X = W + 1;
  uint32_t Y = 1;
  uint32_t Z = 1;
  int FRAMES = 50;

  auto ctx = mgl::opengl::create_context(mgl::opengl::context_mode::STANDALONE);

  auto compute_shader = ctx->compute_shader(code.source({
      { "W", std::to_string(W) },
      { "H", std::to_string(H) },
      { "X", std::to_string(X) },
      { "Y", std::to_string(Y) },
      { "Z", std::to_string(Z) },
  }));

  if(!compute_shader)
  {
    MGL_CORE_ERROR("No compute shader");
    return 1;
  }

  auto buffer_a = ctx->buffer(nc::random::uniform<float>({ H, W * 4 }, 0.0, 2.0).toStlVector());
  auto buffer_b = ctx->buffer(nc::zeros<float>({ H, W * 4 }).toStlVector());

  GifWriter writer = {};
  GifBegin(&writer, "debug.gif", (int)W, (int)H, 2, 8, true);

  auto out_size = H * W * 4 * sizeof(float);
  mgl::core::mem_buffer<uint8_t> out(out_size);
  auto last_buffer = buffer_b;

  for(int i = 0; i < FRAMES; i++)
  {
    auto toogle = (bool)(i % 2);
    buffer_a->bind_to_storage_buffer(toogle ? 1 : 0);
    buffer_b->bind_to_storage_buffer(toogle ? 0 : 1);

    // toggle 2 buffers as input and output
    last_buffer = toogle ? buffer_a : buffer_b;

    // local invocation id x -> pixel x
    // work group_id x -> pixel y
    // eg) buffer[x, y] = gl_LocalInvocationID.x + gl_WorkGroupID.x * W
    compute_shader->run(H, 1);

    // print out
    last_buffer->read_into(out);
    auto output = nc::frombuffer<float>((char*)out.data(), out_size).astype<uint8_t>();
    output = nc::multiply<uint8_t>(output, 255);

    GifWriteFrame(&writer, output.data(), W, H, 2, 8, true);
  }
  GifEnd(&writer);
  return 0;
}