#include "base/file/os_write_handle.h"

#include <fcntl.h>
#include <unistd.h>

#include <memory>
#include <utility>

#include "absl/memory/memory.h"
#include "base/logging.h"
#include "utils/status/compat_errno_macros.h"
#include "utils/status/status_macros.h"

namespace base::file {

/*static*/ ghost::StatusOr<std::unique_ptr<BaseFileHandle>> OsWriteHandle::New(
    const std::string& path) {
  constexpr int kOpenOpt = O_CREAT | O_WRONLY;
  constexpr int kCreatePerm = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  GHOST_ASSIGN_OR_SYSCALL_RET(int fd, open(path.data(), kOpenOpt, kCreatePerm));
  auto handle = absl::WrapUnique(new OsWriteHandle(fd));
  // TODO(hjiang): Fix compilation, the ideal form should be
  // return absl::WrapUnique(new OsWriteHandle(fd))
  return ghost::StatusOr<std::unique_ptr<BaseFileHandle>>{std::move(handle)};
}

OsWriteHandle::~OsWriteHandle() {
  GHOST_CHECK(FAILED_PRECONDITION, closed_);
}

ghost::Status OsWriteHandle::Close() {
  if (closed_) {
    return ghost::Ok();
  }
  closed_ = true;
  GHOST_RETURN_IF_ERROR(Flush());
  GHOST_SCHECK_SYSCALL_RET(close(fd_));
  return ghost::Ok();
}

ghost::Status OsWriteHandle::Write(absl::Nonnull<const char*> buffer,
                                   uint64_t bytes_to_write) {
  GHOST_ASSIGN_OR_SYSCALL_RET(ssize_t bytes_written,
      write(fd_, buffer, bytes_to_write));
  GHOST_SCHECK_EQ(UNKNOWN, bytes_to_write, bytes_written) << "Expects to write "
      << bytes_to_write << " bytes, actually writes " << bytes_written
      << " bytes.";
  return ghost::Ok();
}

ghost::Status OsWriteHandle::Flush() {
  int ret = fsync(fd_);
  if (ret < 0) {
    closed_ = true;
    GHOST_SCHECK_SYSCALL_RET(ret);
  }
  return ghost::Ok();
}


}  // namespace base::file
