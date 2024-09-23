#pragma once

#include <string>

#include "absl/base/nullability.h"
#include "absl/strings/internal/resize_uninitialized.h"

namespace utils::strings {

inline void STLStringResizeUninitialized(
    absl::Nonnull<std::string*> s, size_t new_size) {
  absl::strings_internal::STLStringResizeUninitialized(s, new_size);
}

// Create a string with the given [len] without initializing its memory.
inline std::string CreateStringWithSizeUninitialized(size_t len) {
  std::string s;
  STLStringResizeUninitialized(&s, len);
  return s;
}

}  // namespace utils::strings
