/*
   Copyright 2020 Alexandre Pires (c.alexandre.pires@gmail.com)

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
    void write(const float* src, size_t size, size_t offset);
    void write(const uint32_t* src, size_t size, size_t offset);
    void write(const uint16_t* src, size_t size, size_t offset);
    void write(const uint8_t* src, size_t size, size_t offset);
    void write(const int* src, size_t size, size_t offset);
    void read(const float* dst, size_t size, size_t offset);
    void read(const uint32_t* dst, size_t size, size_t offset);
    void read(const uint16_t* dst, size_t size, size_t offset);
    void read(const uint8_t* dst, size_t size, size_t offset);
    void read(const int* dst, size_t size, size_t offset);

    size_t size();
    bool dynamic();
    bool released();
    const Context* context() const;

    void clear();
    void bind_to_uniform_block(int binding, size_t size, size_t offset);
    void bind_to_storage_buffer(int binding, size_t size, size_t offset);
    void orphan(size_t size);
    void release();

private:
    friend class Context;

    Buffer() = default;

    void write(void* src, size_t size, size_t offset);
    void read(void* dst, size_t size, size_t offset);

    Context* m_context;
    int m_buffer_obj;
    size_t m_size;
    bool m_dynamic;
    bool m_released;
  };

  inline void Buffer::write(const float* src, size_t size, size_t offset)
  {
    write((void*)src, size * sizeof(float), offset);
  }

  inline void Buffer::write(const uint32_t* src, size_t size, size_t offset)
  {
    write((void*)src, size * sizeof(uint32_t), offset);
  }

  inline void Buffer::write(const uint16_t* src, size_t size, size_t offset)
  {
    write((void*)src, size * sizeof(uint16_t), offset);
  }

  inline void Buffer::write(const uint8_t* src, size_t size, size_t offset)
  {
    write((void*)src, size * sizeof(uint8_t), offset);
  }

  inline void Buffer::write(const int* src, size_t size, size_t offset)
  {
    write((void*)src, size * sizeof(int), offset);
  }

  inline void Buffer::read(const float* dst, size_t size, size_t offset)
  {
    read((void*)dst, size * sizeof(float), offset);
  }

  inline void Buffer::read(const uint32_t* dst, size_t size, size_t offset)
  {
    read((void*)dst, size * sizeof(uint32_t), offset);
  }

  inline void Buffer::read(const uint16_t* dst, size_t size, size_t offset)
  {
    read((void*)dst, size * sizeof(uint16_t), offset);
  }

  inline void Buffer::read(const uint8_t* dst, size_t size, size_t offset)
  {
    read((void*)dst, size * sizeof(uint8_t), offset);
  }

  inline void Buffer::read(const int* dst, size_t size, size_t offset)
  {
    read((void*)dst, size * sizeof(int), offset);
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

  inline const Context* Buffer::context() const
  {
    return m_context;
  }

} // namespace AppGL