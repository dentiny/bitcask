#include "utils/status/status_or.h"

#include <gtest/gtest.h>

#include <memory>
#include <utility>

#include "utils/status/testing.h"

namespace {

struct TestClass {
 public:
  int val = 10;
};

// Check conversion of templated status or.
class Base {
 public:
  virtual ~Base() {}
};
class Derived : public Base {
 public:
  ~Derived() override {}
};

}  // namespace

namespace ghost {

TEST(StatusOrTest, AssignTest) {
  // Assign with status.
  {
    auto code = StatusCode::kDeadlineExceeded;
    StatusOr<int> status_or_val = Status(code);
    EXPECT_FALSE(status_or_val.ok());
    EXPECT_EQ(status_or_val.status().code(), code);
  }

  // Assign with value.
  {
    int val = 20;
    StatusOr<int> status_or_val = val;
    EXPECT_TRUE(status_or_val.ok());
    EXPECT_EQ(status_or_val.value(), val);
    EXPECT_EQ(*status_or_val, val);
  }
  // Assign with non-copiable value.
  {
    auto get_derived = []() -> StatusOr<std::unique_ptr<Base>> {
      std::unique_ptr<Base> val = std::make_unique<Derived>();
      StatusOr<std::unique_ptr<Base>> status_or_val{std::move(val)};
      return status_or_val;
    };
    auto val = get_derived();
  }
}

TEST(StatusOrTest, CopyTest) {
  // Assign with status.
  {
    auto code = StatusCode::kDeadlineExceeded;
    StatusOr<int> status_or_val = Status(code);
    auto copied_status_or = status_or_val;
    EXPECT_FALSE(copied_status_or.ok());
    EXPECT_EQ(copied_status_or.status().code(), code);
  }

  // Assign with value.
  {
    int val = 20;
    StatusOr<int> status_or_val = val;
    auto copied_status_or = status_or_val;
    EXPECT_TRUE(copied_status_or.ok());
    EXPECT_EQ(copied_status_or.value(), val);
    EXPECT_EQ(*copied_status_or, val);
  }
}

TEST(StatusOrTest, MoveTest) {
  // Assign with status.
  {
    auto code = StatusCode::kDeadlineExceeded;
    StatusOr<int> status_or_val = Status(code);
    auto moved_status_or = std::move(status_or_val);
    EXPECT_FALSE(moved_status_or.ok());
    EXPECT_EQ(moved_status_or.status().code(), code);
  }

  // Assign with value.
  {
    int val = 20;
    StatusOr<int> status_or_val = val;
    auto moved_status_or = status_or_val;
    EXPECT_TRUE(moved_status_or.ok());
    EXPECT_EQ(moved_status_or.value(), val);
     EXPECT_EQ(*moved_status_or, val);
  }
}

TEST(StatusOrTest, OperatorTest) {
  // Test operator->
  StatusOr<TestClass> status_or_val{TestClass{}};
  EXPECT_EQ(status_or_val->val, 10);
}

TEST(StatusOrTest, EqualityTest) {
  {
    StatusOr<int> val1 = Status{};
    StatusOr<int> val2 = Status{};
    EXPECT_EQ(val1, val2);
  }

  {
    StatusOr<int> val1 = Status{};
    StatusOr<int> val2 = Status{StatusCode::kOk};
    EXPECT_NE(val1, val2);
  }

  {
    StatusOr<int> val1 = Status{};
    StatusOr<int> val2 = 20;
    EXPECT_NE(val1, val2);
  }


  {
    StatusOr<int> val1 = 20;
    StatusOr<int> val2 = 20;
    EXPECT_EQ(val1, val2);
  }

  {
    StatusOr<int> val1 = 40;
    StatusOr<int> val2 = 20;
    EXPECT_NE(val1, val2);
  }
}

TEST(StatusOrTest, ValueOrTest) {
  // OK status.
  {
    StatusOr<int> statusor = 10;
    EXPECT_EQ(statusor.value_or(100), 10);
  }
  // Error status.
  {
    StatusOr<int> statusor =
        Status(StatusCode::kDeadlineExceeded, GHOST_LOC(), "");
    EXPECT_EQ(statusor.value_or(100), 100);
  }
}

// Test StatusOr<Derived> to StatusOr<Base> conversion.
TEST(StatusOrTest, StatusOrConversion) {
  // Check value.
  {
    auto get_derived = []() -> StatusOr<std::unique_ptr<Derived>> {
      return std::make_unique<Derived>();
    };
    StatusOr<std::unique_ptr<Derived>> derived = get_derived();
    StatusOr<std::unique_ptr<Base>> base = std::move(derived);
  }
  // Check status.
  {
    Status status{StatusCode::kNotFound, GHOST_LOC(), "Not found"};
    StatusOr<std::unique_ptr<Derived>> derived{status};
    StatusOr<std::unique_ptr<Base>> base = std::move(derived);
    EXPECT_EQ(base.code(), status.code());
  }
}

}  // namespace ghost
