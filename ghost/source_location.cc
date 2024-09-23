#include "ghost/source_location.h"

#include "absl/strings/str_format.h"

namespace ghost {

bool IsValidSourceLoc(const SourceLocation& loc) {
  return !loc.filename.empty();
}

std::ostream& operator<<(std::ostream& os, const SourceLocation& loc) {
  os << absl::StreamFormat("%s:%d", loc.filename, loc.line_no);
  return os;
}

}  // namespace ghost
