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
#include "mgl_core/mgl_core.hpp"
#include "gtest/gtest.h"

TEST(mgl_core, string_fmt_test)
{
  auto fmt_test = mgl::core::format("This is a {0}!", "test");
  EXPECT_EQ(fmt_test, "This is a test!");
}

TEST(mgl_core, string_in_test)
{
  auto in_test = R""""(
#ifdef VERTEX_SHADER
    void main() {
    }
#endif
#ifdef FRAGMENT_SHADER
    void main() {
    }
#endif
)"""";
  EXPECT_TRUE(mgl::core::in("FRAGMENT_SHADER", in_test));
}

TEST(mgl_core, string_join_test)
{
  mgl::core::string_list vec_string = { "apple", "banana", "orange", "pear", "grape" };
  mgl::core::list<int> vec_int = { 1, 2, 3, 4, 5 };
  mgl::core::list<double> vec_double = { 1.1, 2.2, 3.3, 4.4, 5.5 };
  mgl::core::string s;

  s = mgl::core::join(',', vec_string);
  EXPECT_EQ("apple,banana,orange,pear,grape", s);

  s = mgl::core::join('-', vec_int, 1, 4);
  EXPECT_EQ("2-3-4-5", s);

  s = mgl::core::join(':', vec_double, 2, 5);
  EXPECT_EQ("", s);

  s = mgl::core::join('/', vec_string, 3, 3);
  EXPECT_EQ("pear", s);

  s = mgl::core::join(',', vec_string, 1, 1);
  EXPECT_EQ("banana", s);

  s = mgl::core::join(':', vec_double, 2, 4);
  EXPECT_EQ("3.3:4.4:5.5", s);

  s = mgl::core::join('/', vec_string, 3, 2);
  EXPECT_EQ("", s);

  s = mgl::core::join('/', vec_string, 8);
  EXPECT_EQ("", s);

  s = mgl::core::join('\n', vec_string);
  EXPECT_EQ("apple\nbanana\norange\npear\ngrape", s);
}

TEST(mgl_core, string_trim_test)
{
  mgl::core::string s = "   Hello, World!   ";
  mgl::core::string s_trimmed = mgl::core::trim(s, [](char c) { return c == ' ' || c == '!'; });
  EXPECT_EQ("Hello, World", s_trimmed);

  s_trimmed = mgl::core::trim(s);
  EXPECT_EQ("Hello, World!", s_trimmed);
}

TEST(mgl_core, string_starts_with_test)
{
  mgl::core::string s = "This is a test!";
  EXPECT_TRUE(mgl::core::starts_with(s, "This"));
}

TEST(mgl_core, string_split_test)
{
  mgl::core::string s = "apple\nbanana\norange\npear\ngrape";
  mgl::core::string_list l = mgl::core::split(s, '\n');
  EXPECT_EQ(5, l.size());
  EXPECT_EQ("banana", l[1]);
  EXPECT_EQ("grape", l[4]);

  s = "3f";
  l = mgl::core::split(s, '/');
  EXPECT_EQ(1, l.size());
  EXPECT_EQ("3f", l[0]);

  s = "3f3";
  l = mgl::core::re_split(s, "([fiudn])");
  EXPECT_EQ(2, l.size());
  EXPECT_EQ("3", l[0]);
  EXPECT_EQ("3", l[1]);
}
