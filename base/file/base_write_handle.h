// Interface for file write handle.

#pragma once

#include <cstdint>
#include <string_view>

#include "absl/base/nullability.h"
#include "base/memory_view.h"
#include "base/file/base_file_handle.h"
#include "utils/status/status.h"
#include "utils/status/status_or.h"

namespace base::file {

class BaseWriteHandle : public BaseFileHandle {
 public:
  BaseWriteHandle() = default;
  virtual ~BaseWriteHandle() = default;

  // Flush file content.
  virtual ghost::Status Flush() = 0;

  // Flush content and close the underlying file handle resource.
  virtual ghost::Status Close() = 0;

  // Write [bytes_to_write] starting from [buffer] to the underlying file.
  virtual ghost::Status Write(absl::Nonnull<const char*> buffer,
                              uint64_t bytes_to_write) = 0;
};

}  // namespace base::file
