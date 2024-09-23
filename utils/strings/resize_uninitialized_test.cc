#include "utils/strings/resize_uninitialized.h"

#include <gtest/gtest.h>

#include <memory>
#include <string>

namespace utils::strings {

TEST(ResizeUninitializedTest, BasicTest) {
  constexpr size_t N = 10000;
  std::string s;

  EXPECT_EQ(s.size(), 0);
  STLStringResizeUninitialized(&s, N);
  EXPECT_EQ(s.size(), N);

  STLStringResizeUninitialized(&s, N / 2);
  EXPECT_EQ(s.size(), N / 2);
}

TEST(ResizeUninitializedTest, CreateNewString) {
  constexpr size_t kLen = 100;
  auto s = CreateStringWithSizeUninitialized(kLen);
  EXPECT_EQ(s.length(), kLen);
}

}  // namespace utils::strings
