#include "bitcask/mmap_file.h"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "absl/memory/memory.h"
#include "base/logging.h"
#include "utils/status/compat_errno_macros.h"
#include "utils/status/status_macros.h"

namespace bitcask {

/*static*/ ghost::StatusOr<std::unique_ptr<MmapReadOnlyFile>>
MmapReadOnlyFile::New(const std::string& fname) {
  GHOST_ASSIGN_OR_SYSCALL_RET(int fd, open(fname.data(), O_RDONLY));
  struct stat sb;
  GHOST_SCHECK_SYSCALL_RET(fstat(fd, &sb));
  uint64_t file_size = sb.st_size;

  // We could have empty data files.
  if (file_size == 0) {
    auto mmap_file = absl::WrapUnique(
        new MmapReadOnlyFile(fd, /*data=*/nullptr, /*file_size=*/0));
    mmap_file->closed_ = true;  // As if it was closed.
    return mmap_file;
  }

  // Regular non-empty data files.
  void* data = mmap(/*address=*/nullptr, file_size, PROT_READ, MAP_PRIVATE, fd,
                    /*offset=*/0);
  GHOST_SYSCALL_SCHECK_NE(data, MAP_FAILED);
  return absl::WrapUnique(
      new MmapReadOnlyFile(fd, static_cast<char*>(data), file_size));
}

ghost::StatusOr<std::string_view> MmapReadOnlyFile::PRead(
    uint64_t offset, uint64_t bytes_to_read) {
  const uint64_t end_pos = offset + bytes_to_read;
  GHOST_SCHECK_LE(OUT_OF_RANGE, end_pos, file_size_);
  return std::string_view{data_ + offset, bytes_to_read};
}

ghost::Status MmapReadOnlyFile::Close() {
  if (closed_) {
    return ghost::Ok();
  }
  closed_ = true;
  GHOST_SCHECK_SYSCALL_RET(munmap(data_, file_size_));
  GHOST_SCHECK_SYSCALL_RET(close(fd_));
  return ghost::Ok();
}

MmapReadOnlyFile::~MmapReadOnlyFile() {
  if (closed_) {
    return;
  }
  GHOST_THROW_IF_ERROR(Close());
}

}  // namespace bitcask
