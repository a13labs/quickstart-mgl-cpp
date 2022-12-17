
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
#include "datatype.hpp"
#include "appcore/log.hpp"
#include "opengl.hpp"

namespace AppGL
{
  static int floatBaseFormat[5] = {0, GL_RED, GL_RG, GL_RGB, GL_RGBA};
  static int intBaseFormat[5] = {0, GL_RED_INTEGER, GL_RG_INTEGER, GL_RGB_INTEGER, GL_RGBA_INTEGER};

  static int f1InternalFormat[5] = {0, GL_R8, GL_RG8, GL_RGB8, GL_RGBA8};
  static int f2InternalFormat[5] = {0, GL_R16F, GL_RG16F, GL_RGB16F, GL_RGBA16F};
  static int f4InternalFormat[5] = {0, GL_R32F, GL_RG32F, GL_RGB32F, GL_RGBA32F};
  static int u1InternalFormat[5] = {0, GL_R8UI, GL_RG8UI, GL_RGB8UI, GL_RGBA8UI};
  static int u2InternalFormat[5] = {0, GL_R16UI, GL_RG16UI, GL_RGB16UI, GL_RGBA16UI};
  static int u4InternalFormat[5] = {0, GL_R32UI, GL_RG32UI, GL_RGB32UI, GL_RGBA32UI};
  static int i1InternalFormat[5] = {0, GL_R8I, GL_RG8I, GL_RGB8I, GL_RGBA8I};
  static int i2InternalFormat[5] = {0, GL_R16I, GL_RG16I, GL_RGB16I, GL_RGBA16I};
  static int i4InternalFormat[5] = {0, GL_R32I, GL_RG32I, GL_RGB32I, GL_RGBA32I};

  static int n1InternalFormat[5] = {0, GL_R8, GL_RG8, GL_RGB8, GL_RGBA8};
  static int n2InternalFormat[5] = {0, GL_R16, GL_RG16, GL_RGB16, GL_RGBA16};

  static DataType f1 = {floatBaseFormat, f1InternalFormat, GL_UNSIGNED_BYTE, 1, true};
  static DataType f2 = {floatBaseFormat, f2InternalFormat, GL_HALF_FLOAT, 2, true};
  static DataType f4 = {floatBaseFormat, f4InternalFormat, GL_FLOAT, 4, true};
  static DataType u1 = {intBaseFormat, u1InternalFormat, GL_UNSIGNED_BYTE, 1, false};
  static DataType u2 = {intBaseFormat, u2InternalFormat, GL_UNSIGNED_SHORT, 2, false};
  static DataType u4 = {intBaseFormat, u4InternalFormat, GL_UNSIGNED_INT, 4, false};
  static DataType i1 = {intBaseFormat, i1InternalFormat, GL_BYTE, 1, false};
  static DataType i2 = {intBaseFormat, i2InternalFormat, GL_SHORT, 2, false};
  static DataType i4 = {intBaseFormat, i4InternalFormat, GL_INT, 4, false};

  static DataType nu1 = {floatBaseFormat, n1InternalFormat, GL_UNSIGNED_BYTE, 1, false};
  static DataType nu2 = {floatBaseFormat, n2InternalFormat, GL_UNSIGNED_SHORT, 2, false};
  static DataType ni1 = {floatBaseFormat, n1InternalFormat, GL_BYTE, 1, false};
  static DataType ni2 = {floatBaseFormat, n2InternalFormat, GL_SHORT, 2, false};

  DataType* FromDType(const char* dtype, size_t size)
  {
    if(size < 2 || size > 3)
      return 0;

    if(size == 2)
    {
      switch(dtype[0] * 256 + dtype[1])
      {
        case('f' * 256 + '1'): return &f1;

        case('f' * 256 + '2'): return &f2;

        case('f' * 256 + '4'): return &f4;

        case('u' * 256 + '1'): return &u1;

        case('u' * 256 + '2'): return &u2;

        case('u' * 256 + '4'): return &u4;

        case('i' * 256 + '1'): return &i1;

        case('i' * 256 + '2'): return &i2;

        case('i' * 256 + '4'): return &i4;

        default: return 0;
      }
    }
    else if(size == 3)
    {
      switch(dtype[0] * 65536 + dtype[1] * 256 + dtype[2])
      {
        case('n' * 65536 + 'i' * 256 + '1'): return &ni1;

        case('n' * 65536 + 'i' * 256 + '2'): return &ni2;

        case('n' * 65536 + 'u' * 256 + '1'): return &nu1;

        case('n' * 65536 + 'u' * 256 + '2'): return &nu2;

        default: return 0;
      }
    }
    return 0;
  }

} // namespace AppGL
