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

namespace mgl
{
  class Texture
  {
public:
    enum type
    {
      TEXTURE_2D,
      TEXTURE_3D,
      TEXTURE_CUBE,
    };

    struct filter
    {
      int min_filter;
      int mag_filter;
    };

    virtual ~Texture() = default;
    virtual Texture::type texture_type() = 0;
  };

  inline int swizzle_from_char(char c)
  {
    switch(c)
    {
      case 'R':
      case 'r': return 0x1903;
      case 'G':
      case 'g': return 0x1904;
      case 'B':
      case 'b': return 0x1905;
      case 'A':
      case 'a': return 0x1906;
      case '0': return 0;
      case '1': return 1;
    }

    return 0;
  }

  inline char char_from_swizzle(int c)
  {
    switch(c)
    {
      case 0x1903: return 'R';
      case 0x1904: return 'G';
      case 0x1905: return 'B';
      case 0x1906: return 'A';
      case 0: return '0';
      case 1: return '1';
    }

    return '?';
  }

} // namespace mgl