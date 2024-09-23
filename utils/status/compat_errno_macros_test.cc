#include "utils/status/compat_errno_macros.h"

#include <errno.h>
#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/stat.h>

#include "base/logging.h"
#include "utils/status/status.h"
#include "utils/status/status_or.h"
#include "utils/status/testing.h"

namespace ghost {

namespace {

ghost::Status GetOkStatus() {
  GHOST_SCHECK_SYSCALL_RET(0) << "OK Status: ";
  return ghost::Ok();
}
ghost::Status GetErrorStatus() {
  errno = ENOENT;
  GHOST_SCHECK_SYSCALL_RET(-ENOENT) << "Not found status: ";
  return ghost::Ok();
}
ghost::StatusOr<int> GetFdOrError() {
  GHOST_ASSIGN_OR_SYSCALL_RET(auto fd,
                              open("/tmp/non_existent_file", O_RDONLY));
  return fd;
}

}  // namespace

class ErrnoTest : public testing::Test {
 public:
  void TearDown() override {
    errno = 0;
  }
};

TEST_F(ErrnoTest, BasicTest) {
  {
    const auto s = GetOkStatus();
    GHOST_EXPECT_OK(s);
  }
  {
    const auto s = GetErrorStatus();
    EXPECT_EQ(s.code(), StatusCode::kNotFound);
  }
}

TEST_F(ErrnoTest, ErrnoMacroTest) {
  const auto fd = GetFdOrError();
  EXPECT_EQ(fd.code(), ghost::StatusCode::kNotFound);
}

}  // namespace ghost
