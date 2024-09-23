// Use grpc status code.
// Reference: https://grpc.github.io/grpc/core/md_doc_statuscodes.html

#pragma once

#include <string>
#include <string_view>

namespace ghost {

enum class StatusCode {
  /// Not an error; returned on success.
  kOk = 0,

  kCancelled = 1,
  kUnknown = 2,  // Default error status.
  kInvalidArgument = 3,
  kDeadlineExceeded = 4,
  kNotFound = 5,
  kAlreadyExists = 6,
  kPermissionDenied = 7,
  kUnauthenticated = 16,
  kResourceExhausted = 8,
  kFailedPrecondition = 9,
  kAborted = 10,
  kOutOfRange = 11,
  kUnimplemented = 12,
  kInternal = 13,
  kUnavailable = 14,
  kDataLoss = 15,
};

std::string StatusCodeToString(StatusCode code);
StatusCode StringToStatusCode(std::string_view s);

}  // namespace ghost

