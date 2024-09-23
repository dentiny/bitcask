#include "utils/status/status_code.h"

#include "absl/container/flat_hash_map.h"
#include "base/logging.h"
#include "utils/container/static_map.h"

namespace ghost {

std::string StatusCodeToString(StatusCode code) {
  switch (code) {
    case StatusCode::kOk:
      return "Ok";
    case StatusCode::kCancelled:
      return "Cancelled";
    case StatusCode::kUnknown:
      return "Unknown";
    case StatusCode::kInvalidArgument:
      return "InvalidArgument";
    case StatusCode::kDeadlineExceeded:
      return "DeadlineExceeded";
    case StatusCode::kNotFound:
      return "NotFound";
    case StatusCode::kAlreadyExists:
      return "AlreadyExists";
    case StatusCode::kPermissionDenied:
      return "PermissionDenied";
    case StatusCode::kUnauthenticated:
      return "Unauthenticated";
    case StatusCode::kResourceExhausted:
      return "ResourceExhausted";
    case StatusCode::kFailedPrecondition:
      return "FailedPrecondition";
    case StatusCode::kAborted:
      return "Aborted";
    case StatusCode::kOutOfRange:
      return "OutOfRange";
    case StatusCode::kUnimplemented:
      return "Unimplemented";
    case StatusCode::kInternal:
      return "Internal";
    case StatusCode::kUnavailable:
      return "Unavailable";
    case StatusCode::kDataLoss:
      return "DataLoss";
    default:
      return "UnexpectedStatusCode";
  }
}

StatusCode StringToStatusCode(std::string_view s) {
  static constexpr auto kStrToCodeMap =
      utils::container::MakeStaticMap<std::string_view, StatusCode>({
    {"OK", StatusCode::kOk},
    {"CANCELLED", StatusCode::kCancelled},
    {"UNKNOWN", StatusCode::kUnknown},
    {"INVALID_ARGUMENT", StatusCode::kInvalidArgument},
    {"DEADLINE_EXCEEDED", StatusCode::kDeadlineExceeded},
    {"NOT_FOUND", StatusCode::kNotFound},
    {"ALREADY_EXISTS", StatusCode::kAlreadyExists},
    {"PERMISSION_DENIED", StatusCode::kPermissionDenied},
    {"AUTHENTICATED", StatusCode::kUnauthenticated},
    {"RESOURCE_EXHAUSTED", StatusCode::kResourceExhausted},
    {"FAILED_PRECONDITION", StatusCode::kFailedPrecondition},
    {"ABORTED", StatusCode::kAborted},
    {"OUT_OF_RANGE", StatusCode::kOutOfRange},
    {"UNIMPLEMENTED", StatusCode::kUnimplemented},
    {"INTERNAL", StatusCode::kInternal},
    {"UNAVAILABLE", StatusCode::kUnavailable},
    {"DATA_LOSS", StatusCode::kDataLoss},
  });

  auto iter = kStrToCodeMap.find(s);
  if (iter == kStrToCodeMap.end()) {
    LOG(ERROR) << s << " is not a valid status code string.";
    std::terminate();
  }
  return iter->second;
}

}  // namespace ghost

