// Mmap based file.

#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "utils/status/status.h"
#include "utils/status/status_or.h"

namespace bitcask {

class MmapReadOnlyFile {
 public:
  static ghost::StatusOr<std::unique_ptr<MmapReadOnlyFile>> New(
      const std::string& fname);

  ~MmapReadOnlyFile();

  ghost::StatusOr<std::string_view> PRead(uint64_t offset,
                                         uint64_t bytes_to_read);

  ghost::Status Close();

  uint64_t GetFileSize() const { return file_size_; }

 private:
  MmapReadOnlyFile(int fd, char* data, uint64_t file_size)
      : fd_(fd), data_(data), file_size_(file_size) {}

  int fd_;
  char* data_;
  uint64_t file_size_;
  bool closed_ = false;
};

}  // namespace bitcask
