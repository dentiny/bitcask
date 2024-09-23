#include "utils/status/status.h"
#include "utils/status/status_code.h"

#include <gtest/gtest.h>

#include "absl/strings/str_format.h"
#include "base/init_ghost.h"
#include "base/logging.h"

namespace ghost {

TEST(StatusTest, ErrorStatus) {
  ghost::Status status{};
  EXPECT_EQ(status.code(), StatusCode::kUnknown);

  status = ghost::Ok();
  EXPECT_TRUE(status.ok());

  status = Status{StatusCode::kDeadlineExceeded, "deadline exceeded"};
  EXPECT_FALSE(status.ok());
  EXPECT_EQ(status.code(), StatusCode::kDeadlineExceeded);
  EXPECT_EQ(status.message(), "deadline exceeded");
}

// Observe the output manually.
TEST(StatusTest, DebugStringTest) {
  Status s{StatusCode::kUnavailable, GHOST_LOC(), "Unavailable status."};
  LOG(INFO) << s;
}

TEST(StatusTest, StringFormat) {
  // Test for error status.
  {
    Status s{StatusCode::kAborted, {}, "Aborted"};
    const auto str = absl::StrFormat("%v", s);
    EXPECT_EQ(str, "Aborted: Aborted\n");
  }
  // Test for OK status.
  {
    Status s = ghost::Ok();
    const auto str = absl::StrCat(s, "OK status");
    EXPECT_EQ(str, "Ok: \nOK status");
  }
}

}  // namespace ghost
