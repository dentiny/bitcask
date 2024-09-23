#include "utils/scoped_directory.h"

#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "base/logging.h"

namespace utils {

// Manually check the creation and deletion for scoped directory.
TEST(ScopedDirectoryTest, BasicTest) {
  ScopedDirectory dir{};
  LOG(INFO) << "Directory = " << dir.get_directory();
  std::this_thread::sleep_for(std::chrono::seconds(5));
}

}  // namespace utils
