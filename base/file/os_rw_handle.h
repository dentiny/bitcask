// File handle of local filesystem, or mount filesystems.

#pragma once

#include <memory>
#include <string>
#include <utility>

#include "base/file/base_file_handle.h"
#include "utils/status/status_or.h"

namespace base::file {

class OsRwHandle : public BaseFileHandle {
 public:
  static ghost::StatusOr<std::unique_ptr<BaseFileHandle>> New(
      const std::string& path);

  ~OsRwHandle() override;

  const std::string& Name() const override { return filename_; }

  uint64_t GetFileSize() const override { return file_size_; }

  ghost::Status Close() override;

  ghost::Status Read(absl::Nonnull<MemoryView*> mem_view,
                     uint64_t bytes_to_read) override;

  ghost::Status PRead(absl::Nonnull<MemoryView*> mem_view,
                      uint64_t bytes_to_read, uint64_t pos) override;

  ghost::StatusOr<std::string> CompleteRead() override;

  ghost::Status Flush() override;

  ghost::Status Write(absl::Nonnull<const char*> buffer,
                      uint64_t bytes_to_write) override;

 private:
  OsRwHandle(std::string fname, int fd, uint64_t file_size)
    : filename_(std::move(fname)), fd_(fd), file_size_(file_size) {}

  std::string filename_;
  int fd_;
  uint64_t file_size_ = 0;
  uint64_t offset_ = 0;
  bool closed_ = false;
};

}  // namespace base::file
