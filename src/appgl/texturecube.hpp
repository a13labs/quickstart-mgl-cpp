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
  class TextureCube
  {
public:
    ~TextureCube() { Release(); }

    void Release();

private:
    friend class Context;
    TextureCube();

    Context* mContext;
    DataType* mDataType;
    int mGLObject;
    int mWidth;
    int mHeight;
    int mDepth;
    int mComponents;
    int mMinFilter;
    int mMagFilter;
    int mMaxLevel;
    float mAnisotropy;
    bool mReleased;
  };
} // namespace AppGL