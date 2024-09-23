// Testing utils for ghost::Status.

#pragma once

#include <gtest/gtest.h>

#include <utility>

#include "base/meta.h"
#include "utils/status/status.h"
#include "utils/status/testing.h"

#define GHOST_EXPECT_OK(value) EXPECT_EQ((value), ghost::Ok())
#define GHOST_ASSERT_OK(value) ASSERT_EQ((value), ghost::Ok())

#define __GHOST_ASSIGN_OR_EXPECT_IMPL(var, expr, statusor_name) \
  auto statusor_name = (expr);                                  \
  GHOST_EXPECT_OK(statusor_name.status());                      \
  var = std::move(statusor_name).value();

#define GHOST_ASSIGN_OR_EXPECT(var, expr) \
  __GHOST_ASSIGN_OR_EXPECT_IMPL(var, expr, GHOST_UNIQUE_VARIABLE(statusor))

#define __GHOST_ASSIGN_OR_ASSERT_IMPL(var, expr, statusor_name) \
  auto statusor_name = (expr);                                  \
  GHOST_ASSERT_OK(statusor_name.status());                      \
  var = std::move(statusor_name).value();

#define GHOST_ASSIGN_OR_ASSERT(var, expr) \
  __GHOST_ASSIGN_OR_ASSERT_IMPL(var, expr, GHOST_UNIQUE_VARIABLE(statusor))

namespace ghost {

// Have gtest print full status debug string.
inline void PrintTo(const Status& status, std::ostream* os) {
  *os << status.DebugString();
}

}  // namespace ghost
