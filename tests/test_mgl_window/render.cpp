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
#include "mgl_window/mgl_window.hpp"
#include "gtest/gtest.h"

TEST(gml_opengl, buffers_test)
{

  mgl_core::string format = "3f 2f 1f 3f/i";
  auto parsed = mgl_window::render::parse_layout(format);

  EXPECT_EQ(4, parsed.size());
  EXPECT_EQ(parsed[0].format_string, "3f");
  EXPECT_EQ(parsed[0].components, 3);
  EXPECT_EQ(parsed[0].byte_per_component, 4);
  EXPECT_FALSE(parsed[0].per_instance);

  EXPECT_EQ(parsed[1].format_string, "2f");
  EXPECT_EQ(parsed[1].components, 2);
  EXPECT_EQ(parsed[1].byte_per_component, 4);
  EXPECT_FALSE(parsed[1].per_instance);

  EXPECT_EQ(parsed[2].format_string, "1f");
  EXPECT_EQ(parsed[2].components, 1);
  EXPECT_EQ(parsed[2].byte_per_component, 4);
  EXPECT_FALSE(parsed[2].per_instance);

  EXPECT_EQ(parsed[3].format_string, "3f/i");
  EXPECT_EQ(parsed[3].components, 3);
  EXPECT_EQ(parsed[3].byte_per_component, 4);
  EXPECT_TRUE(parsed[3].per_instance);
}