#include "base/file/os_rw_handle.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <memory>
#include <utility>

#include "absl/memory/memory.h"
#include "base/logging.h"
#include "utils/status/compat_errno_macros.h"
#include "utils/status/status_macros.h"
#include "utils/strings/resize_uninitialized.h"

namespace base::file {

/*static*/ ghost::StatusOr<std::unique_ptr<BaseFileHandle>> OsRwHandle::New(
    const std::string& path) {
  GHOST_ASSIGN_OR_SYSCALL_RET(int fd,
      open(path.data(),
      O_RDWR | O_APPEND | O_CREAT,
      S_IRUSR | S_IRGRP | S_IROTH));
  struct stat statbuf{};
  GHOST_SCHECK_SYSCALL_RET(fstat(fd, &statbuf));

  auto handle = absl::WrapUnique(new OsRwHandle(path, fd, statbuf.st_size));
  return ghost::StatusOr<std::unique_ptr<BaseFileHandle>>{std::move(handle)};
}

OsRwHandle::~OsRwHandle() {
  GHOST_LOG_IF_ERROR(ERROR, Close());
}

ghost::Status OsRwHandle::Close() {
  if (closed_) {
    return ghost::Ok();
  }
  closed_ = true;
  GHOST_SCHECK_SYSCALL_RET(close(fd_));
  return ghost::Ok();
}

ghost::Status OsRwHandle::Read(absl::Nonnull<MemoryView*> mem_view,
                                 uint64_t bytes_to_read) {
  GHOST_SCHECK_LE(INVALID_ARGUMENT, bytes_to_read, mem_view->GetAllocSize())
      << "Allocates " << mem_view->GetAllocSize() << " bytes, but requests "
      << bytes_to_read << " bytes.";
  GHOST_ASSIGN_OR_SYSCALL_RET(ssize_t bytes_read, read(fd_, mem_view->Data(),
      bytes_to_read));
  mem_view->SetActualSize(bytes_read);
  offset_ += bytes_read;
  return ghost::Ok();
}

ghost::Status OsRwHandle::PRead(absl::Nonnull<MemoryView*> mem_view,
                                  uint64_t bytes_to_read, uint64_t pos) {
  GHOST_SCHECK_LE(INVALID_ARGUMENT, bytes_to_read, mem_view->GetAllocSize())
      << "Allocates " << mem_view->GetAllocSize() << " bytes, but requests "
      << bytes_to_read << " bytes.";
  GHOST_ASSIGN_OR_SYSCALL_RET(ssize_t bytes_read, pread(fd_, mem_view->Data(),
      bytes_to_read, pos));
  mem_view->SetActualSize(bytes_read);
  return ghost::Ok();
}

ghost::StatusOr<std::string> OsRwHandle::CompleteRead() {
  struct stat statbuf{};
  GHOST_SCHECK_SYSCALL_RET(fstat(fd_, &statbuf));
  std::string content;
  utils::strings::STLStringResizeUninitialized(&content, statbuf.st_size);
  MemoryView mem_view{content};
  GHOST_RETURN_IF_ERROR(PRead(&mem_view, content.length(), /*pos=*/0));
  GHOST_SCHECK_EQ(UNKNOWN, mem_view.GetAllocSize(), mem_view.GetActualSize())
      << "Expects to read " << mem_view.GetAllocSize() << " bytes, actually "
      << " reads " <<  mem_view.GetActualSize() << " bytes.";
  return content;
}

ghost::Status OsRwHandle::Write(absl::Nonnull<const char*> buffer,
                                   uint64_t bytes_to_write) {
  GHOST_ASSIGN_OR_SYSCALL_RET(ssize_t bytes_written,
      write(fd_, buffer, bytes_to_write));
  GHOST_SCHECK_EQ(UNKNOWN, bytes_to_write, bytes_written) << "Expects to write "
      << bytes_to_write << " bytes, actually writes " << bytes_written
      << " bytes.";
  return ghost::Ok();
}

ghost::Status OsRwHandle::Flush() {
  int ret = fsync(fd_);
  if (ret < 0) {
    closed_ = true;
    GHOST_SCHECK_SYSCALL_RET(ret);
  }
  return ghost::Ok();
}

}  // namespace base::file
