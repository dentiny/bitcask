// Macros for ghost::Status.

#pragma once

#include <string>
#include <utility>

#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "base/logging.h"
#include "base/meta.h"
#include "base/type_traits.h"
#include "ghost/likely.h"
#include "utils/status/status.h"
#include "utils/status/status_code.h"
#include "utils/status/status_exception.h"
#include "utils/strings/debug_printer.h"

// Get status code for given code in string format.
#define __GET_STATUS_CODE(code)    \
  ghost::StringToStatusCode(#code)

#define GHOST_RETURN_IF_ERROR(expr)                            \
  if (auto __status = (expr); ghost::Unlikely(!__status.ok())) \
    return __status

#define GHOST_THROW_IF_ERROR(expr)                        \
  do {                                                    \
    const auto& __status = (expr);                        \
    if (!__status.ok()) {                                 \
      throw ghost::StatusException{std::move(__status)};  \
    }                                                     \
  } while (0)

#define __GHOST_ASSIGN_OR_RETURN_IMPL(var, expr, statusor_name) \
  auto statusor_name = (expr);                                  \
  GHOST_RETURN_IF_ERROR(statusor_name.status());                \
  var = std::move(statusor_name).value()

#define GHOST_ASSIGN_OR_RETURN(var, expr) \
  __GHOST_ASSIGN_OR_RETURN_IMPL(var, expr, GHOST_UNIQUE_VARIABLE(statusor))

#define __GHOST_ASSIGN_OR_THROW_IMPL(var, expr, statusor_name)     \
  auto statusor_name = (expr);                                     \
  GHOST_THROW_IF_ERROR(statusor_name.status());                    \
  var = std::move(statusor_name).value()

#define GHOST_ASSIGN_OR_THROW(var, expr) \
  __GHOST_ASSIGN_OR_THROW_IMPL(var, expr, GHOST_UNIQUE_VARIABLE(statusor))

// NOLINTBEGIN
#define GHOST_SCHECK(code, cond)                                      \
  if (bool val = static_cast<bool>(cond); ghost::Unlikely(!val))      \
      return ghost::Status {__GET_STATUS_CODE(code), GHOST_LOC(), ""}
// NOLINTEND

// Internal implementation to get error status at comparison failure.
#define __DEF_GHOST_ERROR_STATUS(code, lhs, rhs, op)                           \
  [&]() {                                                                      \
    const auto __lhs_str = ghost::DebugString(lhs);                            \
    const auto __rhs_str = ghost::DebugString(rhs);                            \
    auto msg = absl::StrFormat("%v %s %v\n", __lhs_str, #op, __rhs_str);       \
    return ghost::Status{__GET_STATUS_CODE(code), GHOST_LOC(), std::move(msg)};\
  }()

// NOLINTBEGIN
// Internal implementations for GHOST_SCHECK comparison macros.
#define __GHOST_SCHECK_COMP(code, lhs, rhs, op)                       \
    if (const auto& __lhs_val = (lhs),                                \
          __rhs_val = static_cast<decltype(__lhs_val)>((rhs));        \
          ghost::Unlikely(__lhs_val op __rhs_val))                    \
      return __DEF_GHOST_ERROR_STATUS(code, __lhs_val, __rhs_val, op)
// NOLINTEND

#define GHOST_SCHECK_EQ(code, lhs, rhs)  \
  __GHOST_SCHECK_COMP(code, lhs, rhs, !=)
#define GHOST_SCHECK_NE(code, lhs, rhs)  \
  __GHOST_SCHECK_COMP(code, lhs, rhs, ==)
#define GHOST_SCHECK_LT(code, lhs, rhs)  \
  __GHOST_SCHECK_COMP(code, lhs, rhs, >=)
#define GHOST_SCHECK_LE(code, lhs, rhs)  \
  __GHOST_SCHECK_COMP(code, lhs, rhs, >)
#define GHOST_SCHECK_GT(code, lhs, rhs)  \
  __GHOST_SCHECK_COMP(code, lhs, rhs, <=)
#define GHOST_SCHECK_GE(code, lhs, rhs)  \
  __GHOST_SCHECK_COMP(code, lhs, rhs, <)

#define GHOST_SFAIL(code) \
  return ghost::Status {__GET_STATUS_CODE(code), GHOST_LOC(), ""}

#define GHOST_CHECK(code, cond)                                            \
  if (bool val = static_cast<bool>(cond); ghost::Unlikely(!val))           \
    throw ghost::StatusException {__GET_STATUS_CODE(code), GHOST_LOC(), ""}

// NOLINTBEGIN
// Internal implementations for GHOST_CHECK comparison macros.
#define __GHOST_CHECK_COMP(code, lhs, rhs, op)                    \
  if (const auto& __lhs_val = (lhs),                              \
        __rhs_val = static_cast<decltype(__lhs_val)>((rhs));      \
        ghost::Unlikely(__lhs_val op __rhs_val))                  \
    throw ghost::StatusException {                                 \
        __DEF_GHOST_ERROR_STATUS(code, __lhs_val, __rhs_val, op)}
// NOLINTEND

#define GHOST_CHECK_EQ(code, lhs, rhs)  \
  __GHOST_CHECK_COMP(code, lhs, rhs, !=)
#define GHOST_CHECK_NE(code, lhs, rhs)  \
  __GHOST_CHECK_COMP(code, lhs, rhs, ==)
#define GHOST_CHECK_LT(code, lhs, rhs)  \
  __GHOST_CHECK_COMP(code, lhs, rhs, >=)
#define GHOST_CHECK_LE(code, lhs, rhs)  \
  __GHOST_CHECK_COMP(code, lhs, rhs, >)
#define GHOST_CHECK_GT(code, lhs, rhs)  \
  __GHOST_CHECK_COMP(code, lhs, rhs, <=)
#define GHOST_CHECK_GE(code, lhs, rhs)  \
  __GHOST_CHECK_COMP(code, lhs, rhs, <)

/*
Example usage:
GHOST_SFAIL(UNIMPLEMENTED) << <detailed error message>;
*/
#define GHOST_FAIL(code) \
  throw ghost::StatusException {__GET_STATUS_CODE(code), GHOST_LOC(), ""}

#define GHOST_LOG_IF_ERROR(level, cond)    \
  do {                                     \
    const auto& __status = (cond);         \
    if (ghost::Unlikely(!__status.ok())) { \
      LOG(level) << __status;              \
    }                                      \
  } while (0)

// NOLINTBEGIN
// Check a given pointer cannot be nullptr.
#define GHOST_NOTNULL(ptr)                                       \
  if (auto* __ptr = (ptr); ghost::Unlikely(__ptr == nullptr))    \
    throw ghost::StatusException {                               \
      ghost::StatusCode::kInternal, GHOST_LOC(), "Ptr is null."}
// NOLINTEND

// DEBUG macros.
#ifdef DEBUG
#define GHOST_DCHECK(code, cond) GHOST_CHECK(code, cond)
#define GHOST_DCHECK_EQ(code, lhs, rhs) GHOST_CHECK_EQ(code, lhs, rhs)
#define GHOST_DCHECK_NE(code, lhs, rhs) GHOST_CHECK_NE(code, lhs, rhs)
#define GHOST_DCHECK_LT(code, lhs, rhs) GHOST_CHECK_LT(code, lhs, rhs)
#define GHOST_DCHECK_LE(code, lhs, rhs) GHOST_CHECK_LE(code, lhs, rhs)
#define GHOST_DCHECK_GT(code, lhs, rhs) GHOST_CHECK_GT(code, lhs, rhs)
#define GHOST_DCHECK_GE(code, lhs, rhs) GHOST_CHECK_GE(code, lhs, rhs)
#else  // DEBUG
#define GHOST_DCHECK(code, cond) GHOST_CHECK(code, true)
#define GHOST_DCHECK_EQ(code, lhs, rhs) GHOST_CHECK_EQ(code, true, true)
#define GHOST_DCHECK_NE(code, lhs, rhs) GHOST_CHECK_NE(code, true, false)
#define GHOST_DCHECK_LT(code, lhs, rhs) GHOST_CHECK_LT(code, 0, 1)
#define GHOST_DCHECK_LE(code, lhs, rhs) GHOST_CHECK_LE(code, 0, 1)
#define GHOST_DCHECK_GT(code, lhs, rhs) GHOST_CHECK_GT(code, 1, 0)
#define GHOST_DCHECK_GE(code, lhs, rhs) GHOST_CHECK_GE(code, 1, 0)
#endif  // DEBUG
