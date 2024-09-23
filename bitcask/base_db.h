// Interface for bitcask storage model.
//
// A few core features compared with LSM:
// (pro) No need for WAL.
// (pro) Stable time complexity for all operations, all O(1).
// (con) All keys stored in memory, potentially consumes memory.
// (con) No support for range query.
//
// Storage format:
// | timestamp | key len (uint64_t) | value len (uint64_t) | key | value |

#pragma once

#include <string>
#include <string_view>

#include "utils/status/status.h"
#include "utils/status/status_or.h"

namespace bitcask {

// A special marker string to indicate deletion.
inline constexpr std::string_view kTombstone = "non-existent-key";

class BaseDb {
 public:
  BaseDb() = default;
  virtual ~BaseDb() = default;

  // Get the corresponding value for the given [key].
  // If [key] doesn't exist, return NOT_FOUND.
  virtual ghost::StatusOr<std::string> Get(std::string_view key) = 0;

  // Put the [key] and [val] into database.
  virtual ghost::Status Put(std::string key, std::string val) = 0;

  // Delete the [key] from database.
  virtual ghost::Status Delete(std::string key) = 0;
};

}  // namespace bitcask
