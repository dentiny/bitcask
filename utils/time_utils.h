// Time related utils.

#pragma once

#include <cstdint>

namespace utils::time {

// Get current timestamp in steady clock since epoch in nanoseconds.
inline int64_t GetSteadyNowNanoSecSinceEpoch() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             std::chrono::steady_clock::now().time_since_epoch())
      .count();
}

// Get current timestamp in steady clock since epoch in nanoseconds.
inline int64_t GetSystemNowNanoSecSinceEpoch() {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

}  // namespace utils::time
