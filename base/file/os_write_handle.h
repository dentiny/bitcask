// File handle of local filesystem.

#pragma once

#include <memory>
#include <string>

#include "base/file/base_write_handle.h"
#include "utils/status/status_or.h"

namespace base::file {

class OsWriteHandle : public BaseWriteHandle {
 public:
  static ghost::StatusOr<std::unique_ptr<BaseFileHandle>> New(
      const std::string& path);

  // Precondition: [Close] has been called.
  ~OsWriteHandle() override;

  ghost::Status Flush() override;

  ghost::Status Close() override;

  ghost::Status Write(absl::Nonnull<const char*> buffer,
                      uint64_t bytes_to_write) override;

 private:
  explicit OsWriteHandle(int fd) : fd_(fd) {}

  int fd_;
  bool closed_ = false;
};

}  // namespace base::file
