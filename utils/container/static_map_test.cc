#include "utils/container/static_map.h"

#include <gtest/gtest.h>

namespace utils::container {

TEST(StaticMap, BasicTest) {
  auto static_map = MakeStaticMap<int, int>({
    {1, 2},
    {2, 3},
  });
  EXPECT_EQ(static_map.size(), 2);

  auto iter = static_map.find(1);
  ASSERT_NE(iter, static_map.end());
  EXPECT_EQ(iter->first, 1);
  EXPECT_EQ(iter->second, 2);

  iter = static_map.find(5);
  EXPECT_EQ(iter, static_map.end());
}

}  // namespace utils::container
