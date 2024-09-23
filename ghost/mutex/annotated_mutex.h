// Wrapper around mutex types within standard libraries.
// See: https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#mutex-h

#pragma once

#include <mutex>

#include "ghost/mutex/thread_annotations.h"
#include "ghost/mutex/internal/mutex_impl.h"

namespace ghost {

class GHOST_CAPABILITY("mutex") mutex
  : public internal::mutex::mutex_impl_t<mutex> {
 private:
  using Impl = internal::mutex::mutex_impl_t<mutex>;

 public:
  void lock() GHOST_ACQUIRE() { Impl::lock(); }
  void unlock() GHOST_RELEASE() { Impl::unlock(); }
  bool try_lock() GHOST_TRY_ACQUIRE(true) { return Impl::try_lock(); }
};

}  // namespace ghost

