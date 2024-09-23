// A struct which represents source code location (aka. filename and line
// number).

#pragma once

#include <iostream>

namespace ghost {

struct SourceLocation {
  std::string_view filename;
  int line_no = 0;
};

// Whether given [loc] indicates a valid source code location.
bool IsValidSourceLoc(const SourceLocation& loc);

std::ostream& operator<<(std::ostream& os, const SourceLocation& loc);

#define GHOST_LOC()         \
  ghost::SourceLocation {   \
    .filename = __FILE__,   \
    .line_no = __LINE__,    \
  }

}  // namespace ghost
