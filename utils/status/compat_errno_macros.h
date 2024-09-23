#pragma once

#include <cstring>

#include "ghost/likely.h"
#include "ghost/source_location.h"
#include "utils/status/compat_errno.h"
#include "utils/status/status.h"
#include "utils/status/status_exception.h"

#define __GET_ERRNO_STATUS_CODE(ret) \
  ghost::ConvertErrnoToStatusCode((ret))

#define GHOST_SCHECK_SYSCALL_RET(cond)                                \
    if (const int __ret = (cond); ghost::Unlikely(__ret < 0))         \
      return ghost::Status {                                          \
        __GET_ERRNO_STATUS_CODE(errno), GHOST_LOC(), strerror(errno)}

#define GHOST_CHECK_SYSCALL_RET(cond)                                 \
    if (const int __ret = (cond); ghost::Unlikely(__ret < 0))         \
      throw ghost::StatusException {                                  \
        __GET_ERRNO_STATUS_CODE(errno), GHOST_LOC(), strerror(errno)}

#define __GHOST_ASSIGN_OR_SYSCALL_RET_IMPL(var, expr, return_name)  \
  auto return_name = (expr);                                        \
  GHOST_SCHECK_SYSCALL_RET(return_name);                            \
  var = return_name

#define GHOST_ASSIGN_OR_SYSCALL_RET(var, expr)                            \
  __GHOST_ASSIGN_OR_SYSCALL_RET_IMPL(var, expr,                           \
                                     GHOST_UNIQUE_VARIABLE(statusor_var))

#define GHOST_SYSCALL_RET_ASSIGN_OR_THROW(cond)                       \
    if (const int __ret = (cond); ghost::Unlikely(__ret < 0))         \
        throw ghost::StatusException {                                \
        __GET_ERRNO_STATUS_CODE(errno), GHOST_LOC(), strerror(errno)}

#define __GHOST_SYSCALL_ASSIGN_OR_THROW_IMPL(var, expr, return_name) \
  auto return_name = (expr);                                         \
  GHOST_SYSCALL_RET_ASSIGN_OR_THROW(return_name);                    \
  var = return_name

#define GHOST_SYSCALL_ASSIGN_OR_THROW(var, expr)                            \
  __GHOST_SYSCALL_ASSIGN_OR_THROW_IMPL(var, expr,                           \
                                       GHOST_UNIQUE_VARIABLE(statusor_var))

#define GHOST_SYSCALL_SCHECK_NE(expr, var)                            \
    if (ghost::Unlikely((expr) == (var)))                             \
        return ghost::Status {                                        \
        __GET_ERRNO_STATUS_CODE(errno), GHOST_LOC(), strerror(errno)}

#define GHOST_SYSCALL_CHECK_NE(expr, var)                             \
    if (ghost::Unlikely((expr) == (var)))                             \
      throw ghost::StatusException {                                  \
        __GET_ERRNO_STATUS_CODE(errno), GHOST_LOC(), strerror(errno)}
