#include "utils/scoped_directory.h"

#include "base/file/file.h"
#include "utils/status/compat_errno_macros.h"
#include "utils/status/status_macros.h"

namespace utils {

ScopedDirectory::ScopedDirectory() {
  static constexpr auto* const kDirTemplate = "/tmp/temporary_dir_XXXXXX";
  base_temp_dir_ = kDirTemplate;
  GHOST_SYSCALL_CHECK_NE(mkdtemp(base_temp_dir_.data()), nullptr)
      << "Fails to create temporary directory: ";
}

ScopedDirectory::~ScopedDirectory() {
  GHOST_THROW_IF_ERROR(base::file::DeleteDir(base_temp_dir_));
}

}  // namespace utils
