#include "base/file/file.h"

#include <sys/stat.h>
#include <unistd.h>

#include <filesystem>
#include <memory>
#include <utility>
#include <vector>

#include "base/file/os_read_handle.h"
#include "base/file/os_rw_handle.h"
#include "base/file/os_write_handle.h"
#include "utils/status/compat_errno_macros.h"
#include "utils/status/status_macros.h"

namespace base::file {

ghost::StatusOr<std::unique_ptr<BaseFileHandle>> Open(const std::string& path,
                                                      FileOpenMode mode) {
  if (mode == FileOpenMode::kReadOnly) {
    GHOST_ASSIGN_OR_RETURN(auto read_handle, OsReadHandle::New(path));
    return read_handle;
  }

  if (mode == FileOpenMode::kAppendRead) {
    GHOST_ASSIGN_OR_RETURN(auto rw_handle, OsRwHandle::New(path));
    return rw_handle;
  }

  GHOST_ASSIGN_OR_RETURN(std::unique_ptr<BaseFileHandle> write_handle,
      OsWriteHandle::New(path));
  return write_handle;
}

ghost::StatusOr<std::string> CompleteRead(const std::string& filename) {
  GHOST_ASSIGN_OR_RETURN(auto file_handle, Open(filename,
      FileOpenMode::kReadOnly));
  GHOST_ASSIGN_OR_RETURN(auto content, file_handle->CompleteRead());
  return content;
}

ghost::Status CompleteWrite(const std::string& filename,
                            std::string_view content) {
  GHOST_ASSIGN_OR_RETURN(auto write_handle, Open(filename,
                                                 FileOpenMode::kWriteOnly));
  GHOST_RETURN_IF_ERROR(write_handle->Write(content));
  return ghost::Ok();
}

ghost::Status Delete(const std::string& path) {
  int ret = unlink(path.c_str());
  if (ret < 0 && errno == ENOENT) {
    return ghost::Ok();
  }
  GHOST_SCHECK_SYSCALL_RET(ret);
  return ghost::Ok();
}

ghost::StatusOr<uint64_t> GetFileSize(const std::string& path) {
  struct stat s;
  int ret = stat(path.c_str(), &s);
  GHOST_SCHECK_SYSCALL_RET(ret);
  GHOST_SCHECK(INVALID_ARGUMENT, !S_ISDIR(s.st_mode)) << path
      << " is directory.";
  return s.st_size;
}

ghost::Status Makedir(const std::string& dir) {
  GHOST_SCHECK_SYSCALL_RET(mkdir(dir.c_str(), S_IRWXU | S_IRWXG | S_IRWXO))
      << "Fails to make directory " << dir << ":";
  return ghost::Ok();
}

ghost::Status DeleteDir(const std::string& dir) {
  // TODO(hjiang): Use linux syscalls for better error propagation.
  std::filesystem::remove_all(dir);
  return ghost::Ok();
}

ghost::StatusOr<std::vector<std::string>> ListDirRecursively(
    const std::string& folder) {
  std::vector<std::string> files;
  for (auto& child : std::filesystem::recursive_directory_iterator(folder)) {
    auto child_name = std::string(child.path());
    if (std::filesystem::is_directory(child)) {
      child_name.push_back('/');
    }
    files.emplace_back(std::move(child_name));
  }
  return files;
}

ghost::StatusOr<bool> Exists(const std::string& path) {
  struct stat buffer;
  auto errcode = stat(path.c_str(), &buffer);
  if (errcode == -ENOENT || errcode == -EPERM) {
    return false;
  }
  GHOST_SCHECK_SYSCALL_RET(errcode);
  return true;
}

}  // namespace base::file
