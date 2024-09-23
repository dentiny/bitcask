// File handle of local filesystem, or mount filesystems.

#pragma once

#include <memory>
#include <string>

#include "base/file/base_read_handle.h"
#include "utils/status/status_or.h"

namespace base::file {

class OsReadHandle : public BaseReadHandle {
 public:
  static ghost::StatusOr<std::unique_ptr<BaseFileHandle>> New(
      const std::string& path);

  ~OsReadHandle() override;

  uint64_t GetFileSize() const override { return file_size_; }

  ghost::Status Close() override;

  ghost::Status Read(absl::Nonnull<MemoryView*> mem_view,
                     uint64_t bytes_to_read) override;

  ghost::Status PRead(absl::Nonnull<MemoryView*> mem_view,
                      uint64_t bytes_to_read, uint64_t pos) override;

  ghost::StatusOr<std::string> CompleteRead() override;

 private:
  OsReadHandle(int fd, uint64_t file_size) : fd_(fd), file_size_(file_size) {}

  int fd_;
  uint64_t file_size_ = 0;
  uint64_t offset_ = 0;
  bool closed_ = false;
};

}  // namespace base::file
