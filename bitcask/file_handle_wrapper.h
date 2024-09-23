// A file handle wrapper, which deletes the file underlying if
// - reference count decreases to 0
// - and, file handle is configured to cleanup.

#pragma once

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "absl/base/nullability.h"
#include "base/memory_view.h"
#include "base/file/base_file_handle.h"
#include "utils/status/status_or.h"

namespace bitcask {

// TODO(hjiang): Current implementation brings two pointer indirections, maybe
// consider using fd directly.
class FileHandleWrapper {
 public:
  explicit FileHandleWrapper(
    std::unique_ptr<base::file::BaseFileHandle> handle);
  ~FileHandleWrapper();

  void MarkStale() { delete_after_unref_ = true; }

  uint64_t GetFileSize() const;

  const std::string& Name() const { return handle_->Name(); }

  ghost::Status PRead(void* buffer, uint64_t bytes_to_read, uint64_t offset);

  ghost::Status PRead(absl::Nonnull<base::MemoryView*> mem_view,
    uint64_t bytes_to_read, uint64_t offset);

  ghost::Status Write(std::string_view content);

  ghost::Status Flush();

  ghost::Status Close();

 private:
  std::unique_ptr<base::file::BaseFileHandle> handle_;
  // Delete the file if unreferenced.
  // Compaction will mark file stale.
  std::atomic<bool> delete_after_unref_{false};
};

using FileHandleWrapperPtr = std::shared_ptr<FileHandleWrapper>;

// Create a file handle with reference count.
// It's the `Open` syscall for all file operations.
ghost::StatusOr<FileHandleWrapperPtr> CreateFileHandleWithRefCount(
    const std::string& fname);

}  // namespace bitcask
