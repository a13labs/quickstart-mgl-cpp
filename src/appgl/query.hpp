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
  class Query
  {
public:
    enum Keys
    {
      SamplesPassed,
      AnySamplesPassed,
      TimeElapsed,
      PrimitivesGenerated,

      Count,
    };

    ~Query() = default;

    void begin();
    void end();
    void begin_render();
    void end_render();

    int samples();
    int primitives();
    int elapsed();

private:
    friend class Context;
    Query() = default;

    Context* m_context;
    int m_query_obj[Query::Keys::Count];
    bool m_released;
  };
} // namespace AppGL