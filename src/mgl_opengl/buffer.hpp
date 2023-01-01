/*
   Copyright 2022 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#pragma once
#include "builtins.hpp"

namespace mgl::opengl
{
  class buffer
  {
public:
    ~buffer() = default;

public:
    void release();
    bool released();

    size_t size();
    bool dynamic();

    bool read_into(mgl::core::mem_buffer<float>& dst, size_t size = 0, size_t offset = 0, size_t write_offset = 0);
    bool read_into(mgl::core::mem_buffer<uint32_t>& dst, size_t size = 0, size_t offset = 0, size_t write_offset = 0);
    bool read_into(mgl::core::mem_buffer<uint8_t>& dst, size_t size = 0, size_t offset = 0, size_t write_offset = 0);
    bool read_into(mgl::core::mem_buffer<int>& dst, size_t size = 0, size_t offset = 0, size_t write_offset = 0);

    bool write(const mgl::core::mem_buffer<float>& dst, size_t offset);
    bool write(const mgl::core::mem_buffer<uint32_t>& dst, size_t offset);
    bool write(const mgl::core::mem_buffer<uint8_t>& dst, size_t offset);
    bool write(const mgl::core::mem_buffer<int>& dst, size_t offset);

    void clear();
    void bind_to_uniform_block(int binding = 0, size_t size = 0, size_t offset = 0);
    void bind_to_storage_buffer(int binding = 0, size_t size = 0, size_t offset = 0);
    void orphan(size_t size);

    int glo();

private:
    friend class context;
    friend class framebuffer;
    friend class texture_2d;
    friend class texture_3d;
    friend class texture_array;
    friend class texture_cube;
    friend class vertex_array;

    buffer() = default;

    bool read_into(void* dst, size_t dst_size, size_t read_size, size_t read_offset, size_t write_offset);
    bool write(const void* src, size_t size, size_t offset);

    context* m_context;
    int m_buffer_obj;
    size_t m_size;
    bool m_dynamic;
    bool m_released;
  };

  inline int buffer::glo()
  {
    return m_buffer_obj;
  }

  inline bool buffer::write(const mgl::core::mem_buffer<float>& src, size_t offset)
  {
    return write(src.data(), src.size() * sizeof(float), offset);
  }

  inline bool buffer::write(const mgl::core::mem_buffer<uint32_t>& src, size_t offset)
  {
    return write(src.data(), src.size() * sizeof(uint32_t), offset);
  }

  inline bool buffer::write(const mgl::core::mem_buffer<uint8_t>& src, size_t offset)
  {
    return write(src.data(), src.size() * sizeof(uint8_t), offset);
  }

  inline bool buffer::write(const mgl::core::mem_buffer<int>& src, size_t offset)
  {
    return write(src.data(), src.size() * sizeof(int), offset);
  }

  inline bool buffer::read_into(mgl::core::mem_buffer<float>& dst, size_t size, size_t offset, size_t write_offset)
  {
    return read_into(dst.data(), dst.size(), size, offset, write_offset);
  }

  inline bool buffer::read_into(mgl::core::mem_buffer<uint32_t>& dst, size_t size, size_t offset, size_t write_offset)
  {
    return read_into(dst.data(), dst.size(), size, offset, write_offset);
  }

  inline bool buffer::read_into(mgl::core::mem_buffer<uint8_t>& dst, size_t size, size_t offset, size_t write_offset)
  {
    return read_into(dst.data(), dst.size(), size, offset, write_offset);
  }

  inline bool buffer::read_into(mgl::core::mem_buffer<int>& dst, size_t size, size_t offset, size_t write_offset)
  {
    return read_into(dst.data(), dst.size(), size, offset, write_offset);
  }

  inline size_t buffer::size()
  {
    return m_size;
  }

  inline bool buffer::dynamic()
  {
    return m_dynamic;
  }

  inline bool buffer::released()
  {
    return m_released;
  }

} // namespace  mgl::opengl
