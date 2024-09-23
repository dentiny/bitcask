// Configuration for bitcask.

#pragma once

#include <cstdint>

namespace bitcask {

struct Config {
  // Compaction file threshold.
  static constexpr uint64_t kDefaultCompactFileThreshold = 2ULL * 1024 * 1024;
  uint64_t compact_file_size_threshold = kDefaultCompactFileThreshold;

  // Single file threshold to declare current file sealed and create a new file.
  static constexpr uint64_t kDefaultSealFileThreshold = 2ULL * 1024 * 1024;
  uint64_t seal_file_size_threshold = kDefaultSealFileThreshold;

  // Size threshold, smaller than which could be stored in memory directly.
  static constexpr uint64_t kDefaultInlineValThreshold = 16;
  uint64_t inline_val_threshold = kDefaultInlineValThreshold;
};

}  // namespace bitcask
