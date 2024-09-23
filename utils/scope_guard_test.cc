#include "utils/scope_guard.h"

#include <gtest/gtest.h>

#include <future>

#include "base/init_ghost.h"

namespace utils {

TEST(ScopeGuardTest, BasicTest) {
  std::promise<void> promise{};
  {
    ScopeGuard wg { [&]() { promise.set_value(); } };
  }
  promise.get_future().get();
}

TEST(ScopeGuardTest, MacroTest) {
  std::promise<void> promise{};
  {
    SCOPE_EXIT { promise.set_value(); };
  }
  promise.get_future().get();
}

}  // namespace utils

int main(int argc, char** argv) {
  ::base::InitGhost(&argc, &argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

