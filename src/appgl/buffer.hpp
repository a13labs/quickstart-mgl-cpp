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
#include "appgl.hpp"

namespace AppGL
{
  class Buffer
  {
public:
    ~Buffer() { release(); }

public:
    void release();
    bool released();

    size_t size();
    bool dynamic();

    void read_into(AppCore::MemoryBuffer<float>&, size_t size, size_t offset, size_t write_offset);
    void read_into(AppCore::MemoryBuffer<uint32_t>&, size_t size, size_t offset, size_t write_offset);
    void read_into(AppCore::MemoryBuffer<uint8_t>& dst, size_t size, size_t offset, size_t write_offset);
    void read_into(AppCore::MemoryBuffer<int>& dst, size_t size, size_t offset, size_t write_offset);

    void write(const AppCore::MemoryBuffer<float>& dst, size_t offset);
    void write(const AppCore::MemoryBuffer<uint32_t>& dst, size_t offset);
    void write(const AppCore::MemoryBuffer<uint8_t>& dst, size_t offset);
    void write(const AppCore::MemoryBuffer<int>& dst, size_t offset);

    void clear();
    void bind_to_uniform_block(int binding = 0, size_t size = 0, size_t offset = -1);
    void bind_to_storage_buffer(int binding = 0, size_t size = 0, size_t offset = -1);
    void orphan(size_t size = -1);

private:
    friend class Context;
    friend class Framebuffer;
    friend class Texture2D;
    friend class Texture3D;
    friend class TextureArray;
    friend class TextureCube;

    Buffer() = default;

    void read_into(void* dst, size_t dst_size, size_t read_size, size_t read_offset, size_t write_offset);
    void write(const void* src, size_t size, size_t offset);

    Context* m_context;
    int m_buffer_obj;
    size_t m_size;
    bool m_dynamic;
    bool m_released;
  };

  inline void Buffer::write(const AppCore::MemoryBuffer<float>& src, size_t offset)
  {
    write(src.data(), src.size_bytes(), offset);
  }

  inline void Buffer::write(const AppCore::MemoryBuffer<uint32_t>& src, size_t offset)
  {
    write(src.data(), src.size_bytes(), offset);
  }

  inline void Buffer::write(const AppCore::MemoryBuffer<uint8_t>& src, size_t offset)
  {
    write(src.data(), src.size_bytes(), offset);
  }

  inline void Buffer::write(const AppCore::MemoryBuffer<int>& src, size_t offset)
  {
    write(src.data(), src.size_bytes(), offset);
  }

  inline void Buffer::read_into(AppCore::MemoryBuffer<float>& dst, size_t size, size_t offset, size_t write_offset)
  {
    read_into(dst.data(), dst.size(), size, offset, write_offset);
  }

  inline void Buffer::read_into(AppCore::MemoryBuffer<uint32_t>& dst, size_t size, size_t offset, size_t write_offset)
  {
    read_into(dst.data(), dst.size(), size, offset, write_offset);
  }

  inline void Buffer::read_into(AppCore::MemoryBuffer<uint8_t>& dst, size_t size, size_t offset, size_t write_offset)
  {
    read_into(dst.data(), dst.size(), size, offset, write_offset);
  }

  inline void Buffer::read_into(AppCore::MemoryBuffer<int>& dst, size_t size, size_t offset, size_t write_offset)
  {
    read_into(dst.data(), dst.size(), size, offset, write_offset);
  }

  inline size_t Buffer::size()
  {
    return m_size;
  }

  inline bool Buffer::dynamic()
  {
    return m_dynamic;
  }

  inline bool Buffer::released()
  {
    return m_released;
  }

} // namespace AppGL
