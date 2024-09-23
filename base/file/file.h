// A collection of file related factory functions.

#pragma once

#include <cstdint>

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "base/file/base_file_handle.h"
#include "utils/status/status_or.h"

namespace base::file {

enum class FileOpenMode {
  kReadOnly,
  kWriteOnly,
  kAppendRead,
};

// Open a file.
ghost::StatusOr<std::unique_ptr<BaseFileHandle>> Open(const std::string& path,
                                                      FileOpenMode mode);

// Open file for the given [filename], read full content and return.
ghost::StatusOr<std::string> CompleteRead(const std::string& filename);

// Complete write [content] to [filename].
ghost::Status CompleteWrite(const std::string& filename,
                            std::string_view content);

// Delete a file, succeed on missing.
ghost::Status Delete(const std::string& path);

// Get file size.
ghost::StatusOr<uint64_t> GetFileSize(const std::string& path);

// Make directory.
ghost::Status Makedir(const std::string& dir);

// Delete directory.
ghost::Status DeleteDir(const std::string& dir);

// Return all files under the given [folder].
ghost::StatusOr<std::vector<std::string>> ListDirRecursively(
    const std::string& folder);

// Return whether given folder or file exists.
ghost::StatusOr<bool> Exists(const std::string& path);

}  // namespace base::file
