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
#include "glm/glm.hpp"

namespace mgl
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

    void get_value(bool& value);
    void get_value(uint8_t& value);
    void get_value(uint16_t& value);
    void get_value(uint32_t& value);
    void get_value(int& value);
    void get_value(float& value);
    void get_value(const glm::vec1& value);
    void get_value(const glm::vec2& value);
    void get_value(const glm::vec3& value);
    void get_value(const glm::vec4& value);
    void get_value(const glm::mat2& value);
    void get_value(const glm::mat2x3& value);
    void get_value(const glm::mat2x4& value);
    void get_value(const glm::mat3& value);
    void get_value(const glm::mat3x2& value);
    void get_value(const glm::mat3x4& value);
    void get_value(const glm::mat4& value);
    void get_value(const glm::mat4x2& value);
    void get_value(const glm::mat4x3& value);
    void get_value(uint8_t* value, size_t size);
    void get_value(uint16_t* value, size_t size);
    void get_value(uint32_t* value, size_t size);
    void get_value(int* value, size_t size);
    void get_value(float* value, size_t size);

    void set_value(bool value);
    void set_value(uint8_t value);
    void set_value(uint16_t value);
    void set_value(uint32_t value);
    void set_value(int value);
    void set_value(float value);
    void set_value(const glm::vec1 value);
    void set_value(const glm::vec2 value);
    void set_value(const glm::vec3 value);
    void set_value(const glm::vec4 value);
    void set_value(const glm::mat2 value);
    void set_value(const glm::mat2x3 value);
    void set_value(const glm::mat2x4 value);
    void set_value(const glm::mat3 value);
    void set_value(const glm::mat3x2 value);
    void set_value(const glm::mat3x4 value);
    void set_value(const glm::mat4 value);
    void set_value(const glm::mat4x2 value);
    void set_value(const glm::mat4x3 value);
    void set_value(uint8_t* value, size_t size);
    void set_value(uint16_t* value, size_t size);
    void set_value(uint32_t* value, size_t size);
    void set_value(int* value, size_t size);
    void set_value(float* value, size_t size);

private:
    friend class Context;
    Uniform(const mgl_core::string& name, int gl_type, int program_obj, int location, size_t size, Context* ctx);

    void set_value(void* data, size_t size);
    void get_value(void* data, size_t size);

    Context* m_context;
    mgl_core::string m_name;
    int m_program_obj;
    int m_gl_type;
    int m_location;
    int m_size;
    DataType* m_data_type;
    uint8_t* m_data;
  };

  inline void Uniform::get_value(uint8_t& value)
  {
    get_value((void*)&value, sizeof(uint8_t));
  }

  inline void Uniform::get_value(uint16_t& value)
  {
    get_value((void*)&value, sizeof(uint16_t));
  }

  inline void Uniform::get_value(uint32_t& value)
  {
    get_value((void*)&value, sizeof(uint32_t));
  }

  inline void Uniform::get_value(int& value)
  {
    get_value((void*)&value, sizeof(int));
  }

  inline void Uniform::get_value(float& value)
  {
    get_value((void*)&value, sizeof(float));
  }

  inline void Uniform::get_value(const glm::vec1& value)
  {
    get_value((void*)&value, sizeof(glm::vec1));
  }

  inline void Uniform::get_value(const glm::vec2& value)
  {
    get_value((void*)&value, sizeof(glm::vec2));
  }

  inline void Uniform::get_value(const glm::vec3& value)
  {
    get_value((void*)&value, sizeof(glm::vec3));
  }

  inline void Uniform::get_value(const glm::vec4& value)
  {
    get_value((void*)&value, sizeof(glm::vec4));
  }

  inline void Uniform::get_value(const glm::mat2& value)
  {
    get_value((void*)&value, sizeof(glm::mat2));
  }

  inline void Uniform::get_value(const glm::mat2x3& value)
  {
    get_value((void*)&value, sizeof(glm::mat2x3));
  }

  inline void Uniform::get_value(const glm::mat2x4& value)
  {
    get_value((void*)&value, sizeof(glm::mat2x4));
  }

  inline void Uniform::get_value(const glm::mat3& value)
  {
    get_value((void*)&value, sizeof(glm::mat3));
  }

  inline void Uniform::get_value(const glm::mat3x2& value)
  {
    get_value((void*)&value, sizeof(glm::mat3x2));
  }

  inline void Uniform::get_value(const glm::mat3x4& value)
  {
    get_value((void*)&value, sizeof(glm::mat3x4));
  }

  inline void Uniform::get_value(const glm::mat4& value)
  {
    get_value((void*)&value, sizeof(glm::mat4));
  }

  inline void Uniform::get_value(const glm::mat4x2& value)
  {
    get_value((void*)&value, sizeof(glm::mat4x2));
  }

  inline void Uniform::get_value(const glm::mat4x3& value)
  {
    get_value((void*)&value, sizeof(glm::mat4x3));
  }

  inline void Uniform::get_value(uint8_t* value, size_t size)
  {
    get_value((void*)value, size * sizeof(uint8_t));
  }

  inline void Uniform::get_value(uint16_t* value, size_t size)
  {
    get_value((void*)value, size * sizeof(uint16_t));
  }

  inline void Uniform::get_value(uint32_t* value, size_t size)
  {
    get_value((void*)value, size * sizeof(uint32_t));
  }

  inline void Uniform::get_value(int* value, size_t size)
  {
    get_value((void*)value, size * sizeof(int));
  }

  inline void Uniform::get_value(float* value, size_t size)
  {
    get_value((void*)value, size * sizeof(float));
  }

  inline void Uniform::set_value(bool value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(bool));
  }

  inline void Uniform::set_value(uint8_t value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(uint8_t));
  }

  inline void Uniform::set_value(uint16_t value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(uint16_t));
  }

  inline void Uniform::set_value(uint32_t value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(uint32_t));
  }

  inline void Uniform::set_value(int value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(int));
  }

  inline void Uniform::set_value(float value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(float));
  }

  inline void Uniform::set_value(const glm::vec1 value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(glm::vec1));
  }

  inline void Uniform::set_value(const glm::vec2 value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(glm::vec2));
  }

  inline void Uniform::set_value(const glm::vec3 value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(glm::vec3));
  }

  inline void Uniform::set_value(const glm::vec4 value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(glm::vec4));
  }

  inline void Uniform::set_value(const glm::mat2 value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(glm::mat2));
  }

  inline void Uniform::set_value(const glm::mat2x3 value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(glm::mat2x3));
  }

  inline void Uniform::set_value(const glm::mat2x4 value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(glm::mat2x4));
  }

  inline void Uniform::set_value(const glm::mat3 value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(glm::mat3));
  }

  inline void Uniform::set_value(const glm::mat3x2 value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(glm::mat3x2));
  }

  inline void Uniform::set_value(const glm::mat3x4 value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(glm::mat3x4));
  }

  inline void Uniform::set_value(const glm::mat4 value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(glm::mat4));
  }

  inline void Uniform::set_value(const glm::mat4x2 value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(glm::mat4x2));
  }

  inline void Uniform::set_value(const glm::mat4x3 value)
  {
    auto tmp = value;
    set_value((void*)&tmp, sizeof(glm::mat4x3));
  }

  inline void Uniform::set_value(uint8_t* value, size_t size)
  {
    set_value((void*)&value, size * sizeof(uint8_t));
  }

  inline void Uniform::set_value(uint16_t* value, size_t size)
  {
    set_value((void*)&value, size * sizeof(uint16_t));
  }

  inline void Uniform::set_value(uint32_t* value, size_t size)
  {
    set_value((void*)&value, size * sizeof(uint32_t));
  }

  inline void Uniform::set_value(int* value, size_t size)
  {
    set_value((void*)&value, size * sizeof(int));
  }

  inline void Uniform::set_value(float* value, size_t size)
  {
    set_value((void*)&value, size * sizeof(float));
  }

} // namespace mgl
