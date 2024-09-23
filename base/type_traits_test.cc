#include "base/type_traits.h"

#include <gtest/gtest.h>

#include <list>
#include <map>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

#include "absl/container/flat_hash_map.h"

namespace ghost {

TEST(TypeTraitsTest, Container) {
  EXPECT_TRUE(is_container_v<std::vector<int>>);
  EXPECT_TRUE(is_container_v<std::list<int>>);
  EXPECT_FALSE(is_container_v<int>);
  EXPECT_FALSE(is_container_v<double>);
}

TEST(TypeTraitsTest, Map) {
  using T1 = std::map<int, int>;
  EXPECT_TRUE(is_map_v<T1>);
  using T2 = std::unordered_map<int, int>;
  EXPECT_TRUE(is_map_v<T2>);
  EXPECT_FALSE(is_map_v<int>);
  EXPECT_FALSE(is_map_v<double>);
}

}  // namespace ghost
