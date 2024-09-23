// Interface for file read handle.

#pragma once

#include <cstdint>

#include "absl/base/nullability.h"
#include "base/memory_view.h"
#include "base/file/base_file_handle.h"
#include "utils/status/status.h"
#include "utils/status/status_or.h"

namespace base::file {

class BaseReadHandle : public BaseFileHandle {
 public:
  BaseReadHandle() = default;
  virtual ~BaseReadHandle() = default;

  // Close the underlying file handle resource.
  virtual ghost::Status Close() = 0;

  // Read [bytes_to_read] from current position, and fill in to [mem_view].
  virtual ghost::Status Read(absl::Nonnull<MemoryView*> mem_view,
                             uint64_t bytes_to_read) = 0;

  // Read [bytes_to_read] from given position, and fill in to [mem_view].
  virtual ghost::Status PRead(absl::Nonnull<MemoryView*> mem_view,
                              uint64_t bytes_to_read, uint64_t pos) = 0;
};

}  // namespace base::file
