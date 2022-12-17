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
    void release();
    void write(DataPtr data, size_t size, size_t offset);
    void read(size_t size, size_t offset);
    void read_info(DataPtr data, size_t size, size_t offset);
    void write_chunks(DataPtr data, size_t start, size_t step, size_t count);
    void read_chunks(size_t size, size_t start, size_t step, size_t count);
    void read_chunks_info(
        DataPtr data, size_t size, size_t start, size_t step, size_t count, size_t w_offset);

    void clear(size_t size, size_t offset);
    void orphan(size_t size);
    void bind_to_uniform_block(int binding, size_t offset, size_t size);
    void bind_to_storage_buffer(int binding, size_t offset, size_t size);

    inline size_t size() { return m_size; }
    inline bool dynamic() { return m_dynamic; }
    inline bool released() { return m_released; }

private:
    friend class Context;
    Buffer(){};

    Context* m_context;
    int m_buffer_obj;
    size_t m_size;
    bool m_dynamic;
    bool m_released;
  };
} // namespace AppGL
