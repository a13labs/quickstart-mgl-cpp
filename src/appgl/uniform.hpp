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
#include "glm/glm.hpp"

namespace AppGL
{
  class Uniform
  {
public:
    struct DataType
    {
      bool matrix;
      int dimension;
      int element_size;
    };

    ~Uniform();

    void read(bool& value);
    void read(uint8_t& value);
    void read(uint16_t& value);
    void read(uint32_t& value);
    void read(int& value);
    void read(float& value);
    void read(const glm::vec1& value);
    void read(const glm::vec2& value);
    void read(const glm::vec3& value);
    void read(const glm::vec4& value);
    void read(const glm::mat2& value);
    void read(const glm::mat2x3& value);
    void read(const glm::mat2x4& value);
    void read(const glm::mat3& value);
    void read(const glm::mat3x2& value);
    void read(const glm::mat3x4& value);
    void read(const glm::mat4& value);
    void read(const glm::mat4x2& value);
    void read(const glm::mat4x3& value);
    void read(uint8_t* value, size_t size);
    void read(uint16_t* value, size_t size);
    void read(uint32_t* value, size_t size);
    void read(int* value, size_t size);
    void read(float* value, size_t size);

    void write(bool value);
    void write(uint8_t value);
    void write(uint16_t value);
    void write(uint32_t value);
    void write(int value);
    void write(float value);
    void write(const glm::vec1 value);
    void write(const glm::vec2 value);
    void write(const glm::vec3 value);
    void write(const glm::vec4 value);
    void write(const glm::mat2 value);
    void write(const glm::mat2x3 value);
    void write(const glm::mat2x4 value);
    void write(const glm::mat3 value);
    void write(const glm::mat3x2 value);
    void write(const glm::mat3x4 value);
    void write(const glm::mat4 value);
    void write(const glm::mat4x2 value);
    void write(const glm::mat4x3 value);
    void write(uint8_t* value, size_t size);
    void write(uint16_t* value, size_t size);
    void write(uint32_t* value, size_t size);
    void write(int* value, size_t size);
    void write(float* value, size_t size);

private:
    friend class Context;
    Uniform(const AppCore::String& name, int gl_type, int program_obj, int location, size_t size, Context* ctx);

    void write(void* data, size_t size);
    void read(void* data, size_t size);

    Context* m_context;
    AppCore::String m_name;
    int m_program_obj;
    int m_gl_type;
    int m_location;
    int m_size;
    DataType* m_data_type;
    uint8_t* m_data;
  };

  inline void Uniform::read(uint8_t& value)
  {
    read((void*)&value, sizeof(uint8_t));
  }

  inline void Uniform::read(uint16_t& value)
  {
    read((void*)&value, sizeof(uint16_t));
  }

  inline void Uniform::read(uint32_t& value)
  {
    read((void*)&value, sizeof(uint32_t));
  }

  inline void Uniform::read(int& value)
  {
    read((void*)&value, sizeof(int));
  }

  inline void Uniform::read(float& value)
  {
    read((void*)&value, sizeof(float));
  }

  inline void Uniform::read(const glm::vec1& value)
  {
    read((void*)&value, sizeof(glm::vec1));
  }

  inline void Uniform::read(const glm::vec2& value)
  {
    read((void*)&value, sizeof(glm::vec2));
  }

  inline void Uniform::read(const glm::vec3& value)
  {
    read((void*)&value, sizeof(glm::vec3));
  }

  inline void Uniform::read(const glm::vec4& value)
  {
    read((void*)&value, sizeof(glm::vec4));
  }

  inline void Uniform::read(const glm::mat2& value)
  {
    read((void*)&value, sizeof(glm::mat2));
  }

  inline void Uniform::read(const glm::mat2x3& value)
  {
    read((void*)&value, sizeof(glm::mat2x3));
  }

  inline void Uniform::read(const glm::mat2x4& value)
  {
    read((void*)&value, sizeof(glm::mat2x4));
  }

  inline void Uniform::read(const glm::mat3& value)
  {
    read((void*)&value, sizeof(glm::mat3));
  }

  inline void Uniform::read(const glm::mat3x2& value)
  {
    read((void*)&value, sizeof(glm::mat3x2));
  }

  inline void Uniform::read(const glm::mat3x4& value)
  {
    read((void*)&value, sizeof(glm::mat3x4));
  }

  inline void Uniform::read(const glm::mat4& value)
  {
    read((void*)&value, sizeof(glm::mat4));
  }

  inline void Uniform::read(const glm::mat4x2& value)
  {
    read((void*)&value, sizeof(glm::mat4x2));
  }

  inline void Uniform::read(const glm::mat4x3& value)
  {
    read((void*)&value, sizeof(glm::mat4x3));
  }

  inline void Uniform::read(uint8_t* value, size_t size)
  {
    read((void*)value, size * sizeof(uint8_t));
  }

  inline void Uniform::read(uint16_t* value, size_t size)
  {
    read((void*)value, size * sizeof(uint16_t));
  }

  inline void Uniform::read(uint32_t* value, size_t size)
  {
    read((void*)value, size * sizeof(uint32_t));
  }

  inline void Uniform::read(int* value, size_t size)
  {
    read((void*)value, size * sizeof(int));
  }

  inline void Uniform::read(float* value, size_t size)
  {
    read((void*)value, size * sizeof(float));
  }

  inline void Uniform::write(bool value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(bool));
  }

  inline void Uniform::write(uint8_t value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(uint8_t));
  }

  inline void Uniform::write(uint16_t value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(uint16_t));
  }

  inline void Uniform::write(uint32_t value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(uint32_t));
  }

  inline void Uniform::write(int value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(int));
  }

  inline void Uniform::write(float value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(float));
  }

  inline void Uniform::write(const glm::vec1 value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(glm::vec1));
  }

  inline void Uniform::write(const glm::vec2 value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(glm::vec2));
  }

  inline void Uniform::write(const glm::vec3 value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(glm::vec3));
  }

  inline void Uniform::write(const glm::vec4 value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(glm::vec4));
  }

  inline void Uniform::write(const glm::mat2 value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(glm::mat2));
  }

  inline void Uniform::write(const glm::mat2x3 value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(glm::mat2x3));
  }

  inline void Uniform::write(const glm::mat2x4 value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(glm::mat2x4));
  }

  inline void Uniform::write(const glm::mat3 value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(glm::mat3));
  }

  inline void Uniform::write(const glm::mat3x2 value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(glm::mat3x2));
  }

  inline void Uniform::write(const glm::mat3x4 value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(glm::mat3x4));
  }

  inline void Uniform::write(const glm::mat4 value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(glm::mat4));
  }

  inline void Uniform::write(const glm::mat4x2 value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(glm::mat4x2));
  }

  inline void Uniform::write(const glm::mat4x3 value)
  {
    auto tmp = value;
    write((void*)&tmp, sizeof(glm::mat4x3));
  }

  inline void Uniform::write(uint8_t* value, size_t size)
  {
    write((void*)&value, size * sizeof(uint8_t));
  }

  inline void Uniform::write(uint16_t* value, size_t size)
  {
    write((void*)&value, size * sizeof(uint16_t));
  }

  inline void Uniform::write(uint32_t* value, size_t size)
  {
    write((void*)&value, size * sizeof(uint32_t));
  }

  inline void Uniform::write(int* value, size_t size)
  {
    write((void*)&value, size * sizeof(int));
  }

  inline void Uniform::write(float* value, size_t size)
  {
    write((void*)&value, size * sizeof(float));
  }

} // namespace AppGL
