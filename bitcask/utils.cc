#include "bitcask/utils.h"

#include <cstdio>

#include "absl/strings/str_format.h"
#include "base/logging.h"

namespace bitcask {

std::string GetRandomFilename(const std::string& folder) {
  std::string temp_fname = absl::StrFormat("%s/db_file.XXXXXX", folder);
  mktemp(const_cast<char*>(temp_fname.data()));
  return temp_fname;
}

}  // namespace bitcask
