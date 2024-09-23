#include "utils/status/testing.h"

#include <gtest/gtest.h>

#include "utils/status/status.h"

namespace ghost {

TEST(StatusTest, OkTest) {
  const auto ok = Status::Ok();
  GHOST_EXPECT_OK(ok);
  GHOST_ASSERT_OK(ok);
}

// gtest macros are able to print two status out.
// TEST(StatusTest, StringifyTest) {
//   const Status s1 = OutOfRangeStatus(GHOST_LOC(), "Out of range");
//   const Status s2 = DeadlineExceededStatus(GHOST_LOC(), "Deadline exceeded");
//   EXPECT_EQ(s1, s2);
// }

}  // namespace ghost
