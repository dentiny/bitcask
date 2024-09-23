#include "ghost/source_location.h"

#include <gtest/gtest.h>

#include "base/logging.h"

namespace ghost {

TEST(SourceLocationTest, MacrosTest) {
  LOG(INFO) << GHOST_LOC();
}

}  // namespace ghost
