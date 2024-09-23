#include "utils/status/status_macros.h"

#include <gtest/gtest.h>

#include "absl/container/flat_hash_map.h"
#include "base/logging.h"
#include "ghost/source_location.h"
#include "utils/status/status.h"
#include "utils/status/status_or.h"
#include "utils/status/testing.h"

namespace ghost {

namespace {

Status GetOkStatus() {
  const auto s = Status::Ok();
  GHOST_RETURN_IF_ERROR(s);
  return Status::Ok();
}
Status GetErrorStatus() {
  const auto s = Status{StatusCode::kNotFound, "Not found"};
  GHOST_RETURN_IF_ERROR(s);
  return Status::Ok();
}

Status GetErrorStatusWithContext() {
  const auto s = Status{StatusCode::kNotFound, "Not found"};
  GHOST_RETURN_IF_ERROR(s) << "; extra context";
  return Status::Ok();
}

StatusOr<int> GetOkStatusOr() {
  const StatusOr<int> statusor = 5;
  GHOST_ASSIGN_OR_RETURN(const auto val, statusor);
  return val;
}
StatusOr<int> GetErrorStatusOr() {
  const StatusOr<int> statusor = Status{StatusCode::kNotFound, "Not found"};
  GHOST_ASSIGN_OR_RETURN(const auto val, statusor);
  return val;
}

// Used for GHOST_SCHECK macros.
Status GetOkStatusAtCheck() {
  GHOST_SCHECK(UNKNOWN, 5 == 5) << "Unexpected error";
  return ghost::Ok();
}
Status GetErrorStatusAtCheck() {
  GHOST_SCHECK(UNKNOWN, 5 != 5) << "Unexpected error";
  return ghost::Ok();
}

// Used for GHOST_SCHECK_EQ macros.
Status GetOkStatusAtComparison() {
  GHOST_SCHECK_EQ(UNKNOWN, 5, 5) << "Unexpected error";
  return ghost::Ok();
}
Status GetErrorStatusAtComparison() {
  GHOST_SCHECK_EQ(UNKNOWN, 5, 6) << "Unexpected error";
  return ghost::Ok();
}

// Used to inequality comparison macros.
Status GetOkStatusAtInequalComp() {
  GHOST_SCHECK_LE(UNKNOWN, 4, 5) << "Unexpected error";
  return ghost::Ok();
}
Status GetErrorStatusAtInequalComp() {
  GHOST_SCHECK_GT(UNKNOWN, 4, 5) << "Unexpected error";
  return ghost::Ok();
}

}  // namespace

TEST(StatusMacrosTest, StatusTest) {
  {
    const auto ok_status = GetOkStatus();
    GHOST_EXPECT_OK(ok_status);
  }
  {
    const auto error_status = GetErrorStatus();
    EXPECT_EQ(error_status.code(), StatusCode::kNotFound);
  }
  {
    const auto error_status = GetErrorStatusWithContext();
    EXPECT_EQ(error_status.code(), StatusCode::kNotFound);
    EXPECT_EQ(error_status.message(), "Not found; extra context");
  }
}

TEST(StatusMacrosTest, StatusOrTest) {
  {
    const auto ok_statusor = GetOkStatusOr();
    EXPECT_EQ(ok_statusor.value(), 5);
  }
  {
    const auto error_statusor = GetErrorStatusOr();
    EXPECT_EQ(error_statusor.status().code(), StatusCode::kNotFound);
  }
}

TEST(StatusMacrosTest, ScheckTest) {
  // GHOST_SCHECK macros.
  {
    const auto ok_status = GetOkStatusAtCheck();
    GHOST_EXPECT_OK(ok_status);
  }
  {
    const auto error_status = GetErrorStatusAtCheck();
    EXPECT_EQ(error_status.code(), StatusCode::kUnknown);
    // Manually check whether error status and source location is correctly
    // aggregated by macros.
    LOG(INFO) << error_status;
  }
}

TEST(StatusMacrosTest, ScheckEqualTest) {
  // GHOST_SCHECK_EQ macros.
  {
    const auto ok_status = GetOkStatusAtComparison();
    GHOST_EXPECT_OK(ok_status);
  }
  {
    const auto error_status = GetErrorStatusAtComparison();
    EXPECT_EQ(error_status.code(), StatusCode::kUnknown);
    // Manually check whether error status and source location is correctly
    // aggregated by macros.
    LOG(INFO) << error_status;
  }
}

TEST(StatusMacrosTest, ScheckInequalityTest) {
  // Inequality comparison macros.
  {
    const auto ok_status = GetOkStatusAtInequalComp();
    GHOST_EXPECT_OK(ok_status);
  }
  {
    const auto error_status = GetErrorStatusAtInequalComp();
    EXPECT_EQ(error_status.code(), StatusCode::kUnknown);
    // Manually check whether error status and source location is correctly
    // aggregated by macros.
    LOG(INFO) << error_status;
  }
}

TEST(StatusMacrosTest, DebugCheckTest) {
#ifdef DEBUG
  {
    auto f = []() {
      GHOST_DCHECK(UNKNOWN, 4 == 5) << "Unexpected error";
    };
    EXPECT_THROW(f(), StatusException);
  }
  {
    auto f = []() {
      GHOST_DCHECK_GE(UNKNOWN, 4, 5) << "Unexpected error";
    };
    EXPECT_THROW(f(), StatusException);
  }
#else  // DEBUG
  {
    auto f = []() {
      GHOST_DCHECK(UNKNOWN, 4 == 5) << "Unexpected error";
    };
    f();
  }
  {
    auto f = []() {
      GHOST_DCHECK_GE(UNKNOWN, 4, 5) << "Unexpected error";
    };
    f();
  }
#endif  // DEBUG
}

TEST(StatusMacrosTest, CheckTest) {
  {
    auto f = []() {
      GHOST_CHECK_EQ(UNKNOWN, 4, 5) << "Unexpected error";
    };
    EXPECT_THROW(f(), StatusException);
  }
  {
    auto f = [] {
      absl::flat_hash_map<int, int> m;
      auto iter = m.find(5);
      GHOST_CHECK_NE(UNKNOWN, iter, m.end()) << "Unexpected";
    };
    EXPECT_THROW(f(), StatusException);
  }
  {
    auto f = [] {
      const auto s = GetErrorStatus();
      GHOST_THROW_IF_ERROR(s);
    };
    EXPECT_THROW(f(), StatusException);
  }
}

// Manually check error status logging.
TEST(StatusMacrosTest, LogIfError) {
  GHOST_LOG_IF_ERROR(ERROR, GetErrorStatus());
}

}  // namespace ghost
