/**
 * MemoryView is a string view for buffer memory. It doesn't own any memory.
 * It's user's responsibility to make sure the memory address and size given is
 * valid and large enough, otherwise segfault may happen.
 *
 * NOTE: if function invocation fails, MemoryView is not guaranteed to be in
 * valid state.
 */

#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>

#include "absl/base/nullability.h"
#include "utils/status/status_macros.h"

namespace base {

class MemoryView {
 public:
  // It's users' responsibility to allocate enough size of memory, otherwise
  // segfault may happen.
  MemoryView(absl::Nonnull<char*> data, uint64_t alloc_size)
      : data_(data), alloc_size_(alloc_size) {}

  template <typename Container>
  explicit MemoryView(Container& cont)
      // NOLINTNEXTLINE
      : data_(reinterpret_cast<char*>(cont.data())), alloc_size_(cont.size()) {
    GHOST_CHECK_GT(INVALID_ARGUMENT, alloc_size_, 0);
  }

  // Return the address for allocated memory (which is assigned by users).
  char* Data() { return data_; }

  // Get actual memory allocated.
  uint64_t GetAllocSize() const { return alloc_size_; }

  // Return actual memory size.
  uint64_t GetActualSize() const { return actual_size_; }

  // Record how many actual bytes are used.
  void SetActualSize(uint64_t actual_size) { actual_size_ = actual_size; }

 private:
  char* data_ = nullptr;
  uint64_t actual_size_ = 0;  // Actual size loaded from message
  uint64_t alloc_size_ = 0;   // Allocated memory size
};

}  // namespace base
