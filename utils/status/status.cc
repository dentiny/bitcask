#include "utils/status/status.h"

#include <sstream>

#include "absl/strings/str_cat.h"

namespace ghost {

Status::Status(StatusCode code, SourceLocation loc, std::string message)
    : code_(code), message_(std::move(message)) {
  if (IsValidSourceLoc(loc)) {
    source_locs_.emplace_back(loc);
  }
}

std::string Status::DebugString() const {
  std::string res = absl::StrCat(StatusCodeToString(code_), ": ", message_);
  for (auto iter = source_locs_.rbegin(); iter != source_locs_.rend(); ++iter) {
    absl::StrAppend(&res, "\n");

    std::stringstream ss;
    ss << *iter;
    absl::StrAppend(&res, std::move(ss).str());
  }
  absl::StrAppend(&res, "\n");
  return res;
}

std::ostream& operator<<(std::ostream& os, const Status& status) {
  os << status.DebugString();
  return os;
}

}  // namespace ghost

