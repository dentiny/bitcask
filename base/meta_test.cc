#include "base/meta.h"

#include <gtest/gtest.h>

TEST(MetaTest, UniqueName) {
  // Pass two same var names, see whether it compiles.
  [[maybe_unused]] const auto GHOST_UNIQUE_VARIABLE(statusor) = 40;
  [[maybe_unused]] const auto GHOST_UNIQUE_VARIABLE(statusor) = 50;
}
