// Base interface for both read handle and write path.

#pragma once

#include <string>

#include "absl/base/nullability.h"
#include "base/memory_view.h"
#include "utils/status/status.h"
#include "utils/status/status_macros.h"
#include "utils/status/status_or.h"

namespace base::file {

class BaseFileHandle {
 public:
  BaseFileHandle() = default;
  virtual ~BaseFileHandle() = default;

  // Get filename.
  virtual const std::string& Name() const {
    GHOST_FAIL(UNIMPLEMENTED) << __FUNCTION__ << " unimplemented";
  }

  // Get file size, which doesn't perform any IO operations.
  // File size is consolidated at construction.
  virtual uint64_t GetFileSize() const {
    GHOST_FAIL(UNIMPLEMENTED) << __FUNCTION__ << " unimplemented";
  }

  // Close the underlying file handle resource.
  virtual ghost::Status Close() {
    GHOST_FAIL(UNIMPLEMENTED) << __FUNCTION__ << " unimplemented";
  }

  // Read [bytes_to_read] from current position, and fill in to [mem_view].
  virtual ghost::Status Read(absl::Nonnull<MemoryView*> mem_view,
                             uint64_t bytes_to_read) {
    GHOST_FAIL(UNIMPLEMENTED) << __FUNCTION__ << " unimplemented";
  }

  // Read [bytes_to_read] from given position, and fill in to [mem_view].
  virtual ghost::Status PRead(absl::Nonnull<MemoryView*> mem_view,
                              uint64_t bytes_to_read, uint64_t pos) {
    GHOST_FAIL(UNIMPLEMENTED) << __FUNCTION__ << " unimplemented";
  }

  // Read full content and return.
  virtual ghost::StatusOr<std::string> CompleteRead() {
    GHOST_FAIL(UNIMPLEMENTED) << __FUNCTION__ << " unimplemented";
  }

  // Flush file content.
  virtual ghost::Status Flush() {
    GHOST_FAIL(UNIMPLEMENTED) << __FUNCTION__ << " unimplemented";
  }

  // Write [bytes_to_write] starting from [buffer] to the underlying file.
  virtual ghost::Status Write(absl::Nonnull<const char*> buffer,
                              uint64_t bytes_to_write) {
    GHOST_FAIL(UNIMPLEMENTED) << __FUNCTION__ << " unimplemented";
  }

  // Write [content] to the underlying file.
  ghost::Status Write(std::string_view content) {
    return Write(content.data(), content.length());
  }
};

}  // namespace base::file
