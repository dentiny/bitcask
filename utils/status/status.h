#pragma once

#include <string>
#include <ostream>
#include <utility>
#include <string_view>

#include "absl/base/attributes.h"
#include "absl/container/inlined_vector.h"
#include "absl/strings/str_cat.h"
#include "utils/status/status_code.h"
#include "ghost/source_location.h"

namespace ghost {

class ABSL_MUST_USE_RESULT Status {
 public:
  Status() = default;
  explicit Status(StatusCode code) : code_(code) {}
  Status(StatusCode code, const char* message)
    : Status(code, {}, message) {}
  Status(StatusCode code, std::string message)
    : Status(code, {}, std::move(message)) {}
  Status(StatusCode code, SourceLocation loc, std::string message);

  // Used to suppress MUST_USE annotation.
  void IgnoreError() {}

  bool ok() const { return code_ == StatusCode::kOk; }

  explicit operator bool() const { return ok(); }

  StatusCode code() const { return code_; }

  std::string& message() & { return message_; }
  const std::string& message() const& { return message_; }
  std::string&& message() && { return std::move(message_); }

  std::string DebugString() const;

  static Status Ok() { return Status(StatusCode::kOk); }

  bool operator==(const Status& rhs) const { return code_ == rhs.code_; }

  friend std::ostream& operator<<(std::ostream& os, const Status& status);

  // Support abseil string formatting function, for example, `absl::StrCat`,
  // `absl::StrFormat`, etc.
  template <typename Sink>
  friend void AbslStringify(Sink& sink, const Status& status) {
    sink.Append(status.DebugString());
  }

  // TODO(hjiang): Update to varidic template.
  // For example, ghost::Status{...} << ... << ...;
  template <typename T>
  Status& operator<<(T&& msg) {
    absl::StrAppend(&message_, std::forward<T>(msg));
    return *this;
  }

 private:
  StatusCode code_ = StatusCode::kUnknown;
  // Vector of error propagation stack, new error trace is appended to the back.
  // It only contains valid source locations.
  static constexpr size_t kInlineErrorStackNum = 10;
  // TODO(hjiang): Implement source location append via macros.
  absl::InlinedVector<SourceLocation, kInlineErrorStackNum> source_locs_;
  // Error message.
  std::string message_;
};

inline Status Ok() { return Status::Ok(); }

// Util functions on ghost status for all error types.
inline Status CancelledStatus(SourceLocation loc, std::string message) {
  return {StatusCode::kCancelled, std::move(loc), std::move(message)};
}
inline Status UnknownStatus(SourceLocation loc, std::string message) {
  return {StatusCode::kUnknown, std::move(loc), std::move(message)};
}
inline Status InvalidArgumentStatus(SourceLocation loc, std::string message) {
  return {StatusCode::kInvalidArgument, std::move(loc), std::move(message)};
}
inline Status DeadlineExceededStatus(SourceLocation loc, std::string message) {
  return {StatusCode::kDeadlineExceeded, std::move(loc), std::move(message)};
}
inline Status NotFoundStatus(SourceLocation loc, std::string message) {
  return {StatusCode::kNotFound, std::move(loc), std::move(message)};
}
inline Status AlreadyExistsStatus(SourceLocation loc, std::string message) {
  return {StatusCode::kAlreadyExists, std::move(loc), std::move(message)};
}
inline Status PermissionDeniedStatus(SourceLocation loc, std::string message) {
  return {StatusCode::kPermissionDenied, std::move(loc), std::move(message)};
}
inline Status UnauthenticatedStatus(SourceLocation loc, std::string message) {
  return {StatusCode::kUnauthenticated, std::move(loc), std::move(message)};
}
inline Status ResourceExhaustedStatus(SourceLocation loc, std::string message) {
  return {StatusCode::kResourceExhausted, std::move(loc), std::move(message)};
}
inline Status FailedPreconditionStatus(SourceLocation loc,
                                       std::string message) {
  return {StatusCode::kFailedPrecondition, std::move(loc), std::move(message)};
}
inline Status AbortedStatus(SourceLocation loc, std::string message) {
  return {StatusCode::kAborted, std::move(loc), std::move(message)};
}
inline Status OutOfRangeStatus(SourceLocation loc, std::string message) {
  return {StatusCode::kOutOfRange, std::move(loc), std::move(message)};
}
inline Status UnimplementedStatus(SourceLocation loc, std::string message) {
  return {StatusCode::kUnimplemented, std::move(loc), std::move(message)};
}
inline Status InternalStatus(SourceLocation loc, std::string message) {
  return {StatusCode::kInternal, std::move(loc), std::move(message)};
}
inline Status UnavailableStatus(SourceLocation loc, std::string message) {
  return {StatusCode::kUnavailable, std::move(loc), std::move(message)};
}
inline Status DataLossStatus(SourceLocation loc, std::string message) {
  return {StatusCode::kDataLoss, std::move(loc), std::move(message)};
}

}  // namespace ghost

