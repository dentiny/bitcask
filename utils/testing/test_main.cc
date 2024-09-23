// Main function for all gtest-based unit tests.

#include <gtest/gtest.h>

#include <cstring>
#include <string_view>

#include "base/init_ghost.h"

int main(int argc, char **argv) {
  // Differentiate gtest flags and own flags.
  int non_gtest_argc = 0;
  char **non_gtest_argv = nullptr;
  ::base::InitGhost(&non_gtest_argc, &non_gtest_argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
