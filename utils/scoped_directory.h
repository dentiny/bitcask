// A scoped directory, which cleans up itself after destruction.

#pragma once

#include <string>

namespace utils {

class ScopedDirectory {
 public:
  ScopedDirectory();

  ~ScopedDirectory();

  const std::string& get_directory() const {
    return base_temp_dir_;
  }

 private:
  std::string base_temp_dir_;
};

}  // namespace utils
