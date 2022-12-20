
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
#include "uniform.hpp"
#include "appcore/log.hpp"
#include "context.hpp"

namespace AppGL
{
  static UniformDataType gl_bool = {false, 1, 4};
  static UniformDataType gl_bool_vec2 = {false, 2, 8};
  static UniformDataType gl_bool_vec3 = {false, 3, 12};
  static UniformDataType gl_bool_vec4 = {false, 4, 16};
  static UniformDataType gl_int = {false, 1, 4};
  static UniformDataType gl_int_vec2 = {false, 2, 8};
  static UniformDataType gl_int_vec3 = {false, 3, 12};
  static UniformDataType gl_int_vec4 = {false, 4, 16};
  static UniformDataType gl_unsigned_int = {false, 1, 4};
  static UniformDataType gl_unsigned_int_vec2 = {false, 2, 8};
  static UniformDataType gl_unsigned_int_vec3 = {false, 3, 12};
  static UniformDataType gl_unsigned_int_vec4 = {false, 4, 16};
  static UniformDataType gl_float = {false, 1, 4};
  static UniformDataType gl_float_vec2 = {false, 2, 8};
  static UniformDataType gl_float_vec3 = {false, 3, 12};
  static UniformDataType gl_float_vec4 = {false, 4, 16};
  static UniformDataType gl_double = {false, 1, 8};
  static UniformDataType gl_double_vec2 = {false, 2, 16};
  static UniformDataType gl_double_vec3 = {false, 3, 24};
  static UniformDataType gl_double_vec4 = {false, 4, 32};
  static UniformDataType gl_sampler_1_d = {false, 1, 4};
  static UniformDataType gl_sampler_1D_array = {false, 1, 4};
  static UniformDataType gl_int_sampler_1D = {false, 1, 4};
  static UniformDataType gl_int_sampler_1D_array = {false, 1, 4};
  static UniformDataType gl_sampler_2D = {false, 1, 4};
  static UniformDataType gl_int_sampler_2D = {false, 1, 4};
  static UniformDataType gl_unsigned_int_sampler_2D = {false, 1, 4};
  static UniformDataType gl_sampler_2D_array = {false, 1, 4};
  static UniformDataType gl_int_sampler_2D_array = {false, 1, 4};
  static UniformDataType gl_unsigned_int_sampler_2D_array = {false, 1, 4};
  static UniformDataType gl_sampler_3D = {false, 1, 4};
  static UniformDataType gl_int_sampler_3D = {false, 1, 4};
  static UniformDataType gl_unsigned_int_sampler_3D = {false, 1, 4};
  static UniformDataType gl_sampler_2D_shadow = {false, 1, 4};
  static UniformDataType gl_sampler_2D_multisample = {false, 1, 4};
  static UniformDataType gl_int_sampler_2D_multisample = {false, 1, 4};
  static UniformDataType gl_unsigned_int_sampler_2D_multisample = {false, 1, 4};
  static UniformDataType glSampler_2D_multisample_array = {false, 1, 4};
  static UniformDataType gl_int_sampler_2D_multisample_array = {false, 1, 4};
  static UniformDataType gl_unsigned_int_sampler_2D_multisample_array = {false, 1, 4};
  static UniformDataType gl_sampler_cube = {false, 1, 4};
  static UniformDataType gl_int_sampler_cube = {false, 1, 4};
  static UniformDataType gl_unsigned_int_sampler_cube = {false, 1, 4};
  static UniformDataType gl_image_2D = {false, 1, 4};
  static UniformDataType gl_float_mat_2 = {true, 4, 16};
  static UniformDataType gl_float_mat_2x3 = {true, 6, 24};
  static UniformDataType gl_float_mat_2x4 = {true, 8, 32};
  static UniformDataType gl_float_mat_3x2 = {true, 6, 24};
  static UniformDataType gl_float_mat_3 = {true, 9, 36};
  static UniformDataType gl_float_mat_3x4 = {true, 12, 48};
  static UniformDataType gl_float_mat_4x2 = {true, 8, 32};
  static UniformDataType gl_float_mat_4x3 = {true, 12, 48};
  static UniformDataType gl_float_mat_4 = {true, 16, 64};
  static UniformDataType gl_double_mat_2 = {true, 4, 32};
  static UniformDataType gl_double_mat_2x3 = {true, 6, 48};
  static UniformDataType gl_double_mat_2x4 = {true, 8, 64};
  static UniformDataType gl_double_mat_3x2 = {true, 6, 48};
  static UniformDataType gl_double_mat_3 = {true, 9, 72};
  static UniformDataType gl_double_mat_3x4 = {true, 12, 96};
  static UniformDataType gl_double_mat_4x2 = {true, 8, 64};
  static UniformDataType gl_double_mat_4x3 = {true, 12, 96};
  static UniformDataType gl_double_mat_4 = {true, 16, 128};

  UniformDataType* uniform_lookup_table(int gl_type)
  {

    switch(gl_type)
    {
      case GL_BOOL: return &gl_bool;
      case GL_BOOL_VEC2: return &gl_bool_vec2;
      case GL_BOOL_VEC3: return &gl_bool_vec3;
      case GL_BOOL_VEC4: return &gl_bool_vec4;
      case GL_INT: return &gl_int;
      case GL_INT_VEC2: return &gl_int_vec2;
      case GL_INT_VEC3: return &gl_int_vec3;
      case GL_INT_VEC4: return &gl_int_vec4;
      case GL_UNSIGNED_INT: return &gl_unsigned_int;
      case GL_UNSIGNED_INT_VEC2: return &gl_unsigned_int_vec2;
      case GL_UNSIGNED_INT_VEC3: return &gl_unsigned_int_vec3;
      case GL_UNSIGNED_INT_VEC4: return &gl_unsigned_int_vec4;
      case GL_FLOAT: return &gl_float;
      case GL_FLOAT_VEC2: return &gl_float_vec2;
      case GL_FLOAT_VEC3: return &gl_float_vec3;
      case GL_FLOAT_VEC4: return &gl_float_vec4;
      case GL_DOUBLE: return &gl_double;
      case GL_DOUBLE_VEC2: return &gl_double_vec2;
      case GL_DOUBLE_VEC3: return &gl_double_vec3;
      case GL_DOUBLE_VEC4: return &gl_double_vec4;
      case GL_SAMPLER_1D: return &gl_sampler_1_d;
      case GL_SAMPLER_1D_ARRAY: return &gl_sampler_1D_array;
      case GL_INT_SAMPLER_1D: return &gl_int_sampler_1D;
      case GL_INT_SAMPLER_1D_ARRAY: return &gl_int_sampler_1D_array;
      case GL_SAMPLER_2D: return &gl_sampler_2D;
      case GL_INT_SAMPLER_2D: return &gl_int_sampler_2D;
      case GL_UNSIGNED_INT_SAMPLER_2D: return &gl_unsigned_int_sampler_2D;
      case GL_SAMPLER_2D_ARRAY: return &gl_sampler_2D_array;
      case GL_INT_SAMPLER_2D_ARRAY: return &gl_int_sampler_2D_array;
      case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: return &gl_unsigned_int_sampler_2D_array;
      case GL_SAMPLER_3D: return &gl_sampler_3D;
      case GL_INT_SAMPLER_3D: return &gl_int_sampler_3D;
      case GL_UNSIGNED_INT_SAMPLER_3D: return &gl_unsigned_int_sampler_3D;
      case GL_SAMPLER_2D_SHADOW: return &gl_sampler_2D_shadow;
      case GL_SAMPLER_2D_MULTISAMPLE: return &gl_sampler_2D_multisample;
      case GL_INT_SAMPLER_2D_MULTISAMPLE: return &gl_int_sampler_2D_multisample;
      case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: return &gl_unsigned_int_sampler_2D_multisample;
      case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: return &glSampler_2D_multisample_array;
      case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return &gl_int_sampler_2D_multisample_array;
      case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return &gl_unsigned_int_sampler_2D_multisample_array;
      case GL_SAMPLER_CUBE: return &gl_sampler_cube;
      case GL_INT_SAMPLER_CUBE: return &gl_int_sampler_cube;
      case GL_UNSIGNED_INT_SAMPLER_CUBE: return &gl_unsigned_int_sampler_cube;
      case GL_IMAGE_2D: return &gl_image_2D;
      case GL_FLOAT_MAT2: return &gl_float_mat_2;
      case GL_FLOAT_MAT2x3: return &gl_float_mat_2x3;
      case GL_FLOAT_MAT2x4: return &gl_float_mat_2x4;
      case GL_FLOAT_MAT3x2: return &gl_float_mat_3x2;
      case GL_FLOAT_MAT3: return &gl_float_mat_3;
      case GL_FLOAT_MAT3x4: return &gl_float_mat_3x4;
      case GL_FLOAT_MAT4x2: return &gl_float_mat_4x2;
      case GL_FLOAT_MAT4x3: return &gl_float_mat_4x3;
      case GL_FLOAT_MAT4: return &gl_float_mat_4;
      case GL_DOUBLE_MAT2: return &gl_double_mat_2;
      case GL_DOUBLE_MAT2x3: return &gl_double_mat_2x3;
      case GL_DOUBLE_MAT2x4: return &gl_double_mat_2x4;
      case GL_DOUBLE_MAT3x2: return &gl_double_mat_3x2;
      case GL_DOUBLE_MAT3: return &gl_double_mat_3;
      case GL_DOUBLE_MAT3x4: return &gl_double_mat_3x4;
      case GL_DOUBLE_MAT4x2: return &gl_double_mat_4x2;
      case GL_DOUBLE_MAT4x3: return &gl_double_mat_4x3;
      case GL_DOUBLE_MAT4: return &gl_double_mat_4;
      default: APPCORE_ASSERT(false, "invalid gl type"); return nullptr;
    };
  }

  Uniform::Uniform(const AppCore::String& name, int gl_type, int m_program_obj, int m_location, size_t size, Context* ctx)
  {
    m_context = ctx;
    m_name = name;
    m_gl_type = gl_type;
    m_program_obj = m_program_obj;
    m_location = m_location;
    m_size = m_size;
    m_data_type = uniform_lookup_table(gl_type);
    m_data = new uint8_t[m_data_type->element_size * size]();
  }

  Uniform::~Uniform()
  {
    delete[] m_data;
  }

  void Uniform::write(void* data, size_t size)
  {
    APPCORE_ASSERT(size == (size_t)(m_size * m_data_type->element_size), "invalid data size");

    const GLMethods& gl = m_context->gl();

    char* ptr = (char*)data;

    gl.UseProgram(m_program_obj);

    switch(m_gl_type)
    {
      case GL_BOOL: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_BOOL_VEC2: {
        gl.Uniform2iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_BOOL_VEC3: {
        gl.Uniform3iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_BOOL_VEC4: {
        gl.Uniform4iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_VEC2: {
        gl.Uniform2iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_VEC3: {
        gl.Uniform3iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_VEC4: {
        gl.Uniform4iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_UNSIGNED_INT: {
        gl.Uniform1uiv(m_location, m_size, (unsigned*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_VEC2: {
        gl.Uniform2uiv(m_location, m_size, (unsigned*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_VEC3: {
        gl.Uniform3uiv(m_location, m_size, (unsigned*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_VEC4: {
        gl.Uniform4uiv(m_location, m_size, (unsigned*)ptr);
      }
      break;
      case GL_FLOAT: {
        gl.Uniform1fv(m_location, m_size, (float*)ptr);
      }
      break;
      case GL_FLOAT_VEC2: {
        gl.Uniform2fv(m_location, m_size, (float*)ptr);
      }
      break;
      case GL_FLOAT_VEC3: {
        gl.Uniform3fv(m_location, m_size, (float*)ptr);
      }
      break;
      case GL_FLOAT_VEC4: {
        gl.Uniform4fv(m_location, m_size, (float*)ptr);
      }
      break;
      case GL_DOUBLE: {
        gl.Uniform1dv(m_location, m_size, (double*)ptr);
      }
      break;
      case GL_DOUBLE_VEC2: {
        gl.Uniform2dv(m_location, m_size, (double*)ptr);
      }
      break;
      case GL_DOUBLE_VEC3: {
        gl.Uniform3dv(m_location, m_size, (double*)ptr);
      }
      break;
      case GL_DOUBLE_VEC4: {
        gl.Uniform4dv(m_location, m_size, (double*)ptr);
      }
      break;
      case GL_SAMPLER_1D: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_SAMPLER_1D_ARRAY: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_SAMPLER_1D: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_SAMPLER_1D_ARRAY: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_SAMPLER_2D: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_SAMPLER_2D: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_SAMPLER_2D: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_SAMPLER_2D_ARRAY: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_SAMPLER_2D_ARRAY: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_SAMPLER_3D: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_SAMPLER_3D: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_SAMPLER_3D: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_SAMPLER_2D_SHADOW: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_SAMPLER_2D_MULTISAMPLE: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_SAMPLER_2D_MULTISAMPLE: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_SAMPLER_CUBE: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_INT_SAMPLER_CUBE: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_UNSIGNED_INT_SAMPLER_CUBE: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_IMAGE_2D: {
        gl.Uniform1iv(m_location, m_size, (int*)ptr);
      }
      break;
      case GL_FLOAT_MAT2: {
        gl.UniformMatrix2fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_FLOAT_MAT2x3: {
        gl.UniformMatrix2x3fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_FLOAT_MAT2x4: {
        gl.UniformMatrix2x4fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_FLOAT_MAT3x2: {
        gl.UniformMatrix3x2fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_FLOAT_MAT3: {
        gl.UniformMatrix3fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_FLOAT_MAT3x4: {
        gl.UniformMatrix3x4fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_FLOAT_MAT4x2: {
        gl.UniformMatrix4x2fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_FLOAT_MAT4x3: {
        gl.UniformMatrix4x3fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_FLOAT_MAT4: {
        gl.UniformMatrix4fv(m_location, m_size, false, (float*)ptr);
      }
      break;
      case GL_DOUBLE_MAT2: {
        gl.UniformMatrix2dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      case GL_DOUBLE_MAT2x3: {
        gl.UniformMatrix2x3dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      case GL_DOUBLE_MAT2x4: {
        gl.UniformMatrix2x4dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      case GL_DOUBLE_MAT3x2: {
        gl.UniformMatrix3x2dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      case GL_DOUBLE_MAT3: {
        gl.UniformMatrix3dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      case GL_DOUBLE_MAT3x4: {
        gl.UniformMatrix3x4dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      case GL_DOUBLE_MAT4x2: {
        gl.UniformMatrix4x2dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      case GL_DOUBLE_MAT4x3: {
        gl.UniformMatrix4x3dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      case GL_DOUBLE_MAT4: {
        gl.UniformMatrix4dv(m_location, m_size, false, (double*)ptr);
      }
      break;
      default: APPCORE_ASSERT(false, "invalid gl type"); break;
    }
  }

  void Uniform::read(void* data, size_t size)
  {

    APPCORE_ASSERT(size == (size_t)(m_size * m_data_type->element_size), "invalid data size");

    const GLMethods& gl = m_context->gl();
    char* ptr = (char*)data;
    gl.UseProgram(m_program_obj);

    for(int i = 0; i < m_size; ++i)
    {
      switch(m_gl_type)
      {
        case GL_BOOL: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_BOOL_VEC2: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_BOOL_VEC3: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_BOOL_VEC4: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_VEC2: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_VEC3: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_VEC4: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT: {
          gl.GetUniformuiv(m_program_obj, m_location + i, (unsigned*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_VEC2: {
          gl.GetUniformuiv(m_program_obj, m_location + i, (unsigned*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_VEC3: {
          gl.GetUniformuiv(m_program_obj, m_location + i, (unsigned*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_VEC4: {
          gl.GetUniformuiv(m_program_obj, m_location + i, (unsigned*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT: {
          gl.GetUniformfv(m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_VEC2: {
          gl.GetUniformfv(m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_VEC3: {
          gl.GetUniformfv(m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_VEC4: {
          gl.GetUniformfv(m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE: {
          gl.GetUniformdv(m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_VEC2: {
          gl.GetUniformdv(m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_VEC3: {
          gl.GetUniformdv(m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_VEC4: {
          gl.GetUniformdv(m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_1D: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_1D_ARRAY: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_SAMPLER_1D: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_SAMPLER_1D_ARRAY: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_2D: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_SAMPLER_2D: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_SAMPLER_2D: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_2D_ARRAY: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_SAMPLER_2D_ARRAY: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_3D: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_SAMPLER_3D: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_SAMPLER_3D: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_2D_SHADOW: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_2D_MULTISAMPLE: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_SAMPLER_2D_MULTISAMPLE: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_SAMPLER_CUBE: {
          {
            gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
          }
          break;
        }
        case GL_INT_SAMPLER_CUBE: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_UNSIGNED_INT_SAMPLER_CUBE: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_IMAGE_2D: {
          gl.GetUniformiv(m_program_obj, m_location + i, (int*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT2: {
          gl.GetUniformfv(m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT2x3: {
          gl.GetUniformfv(m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT2x4: {
          gl.GetUniformfv(m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT3x2: {
          gl.GetUniformfv(m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT3: {
          gl.GetUniformfv(m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT3x4: {
          gl.GetUniformfv(m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT4x2: {
          gl.GetUniformfv(m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT4x3: {
          gl.GetUniformfv(m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_FLOAT_MAT4: {
          gl.GetUniformfv(m_program_obj, m_location + i, (float*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT2: {
          gl.GetUniformdv(m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT2x3: {
          gl.GetUniformdv(m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT2x4: {
          gl.GetUniformdv(m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT3x2: {
          gl.GetUniformdv(m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT3: {
          gl.GetUniformdv(m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT3x4: {
          gl.GetUniformdv(m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT4x2: {
          gl.GetUniformdv(m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT4x3: {
          gl.GetUniformdv(m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        case GL_DOUBLE_MAT4: {
          gl.GetUniformdv(m_program_obj, m_location + i, (double*)(ptr + i * m_data_type->element_size));
        }
        break;
        default: APPCORE_ASSERT(false, "invalid gl type"); break;
      }
    }
  }
} // namespace AppGL
