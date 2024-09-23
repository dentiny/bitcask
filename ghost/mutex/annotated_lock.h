// Wrapper around lock types in standard libraries.
// See: https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#mutex-h

#pragma once

#include <chrono>
#include <mutex>

#include "ghost/mutex/thread_annotations.h"
#include "ghost/mutex/internal/mutex_impl.h"

namespace ghost {

template <typename M>
class GHOST_SCOPED_CAPABILITY lock_guard
  : public internal::mutex::lock_impl_t<lock_guard<M>> {
 private:
  using Impl = internal::mutex::lock_impl_t<lock_guard<M>>;

 public:
  explicit lock_guard(M& m) GHOST_ACQUIRE(m) : Impl(m) {}
  lock_guard(M& m, std::adopt_lock_t t) GHOST_REQUIRES(m) : Impl(m, t) {}
  ~lock_guard() GHOST_RELEASE() = default;
};

template <typename M>
class GHOST_SCOPED_CAPABILITY unique_lock
  : public internal::mutex::lock_impl_t<unique_lock<M>> {
 private:
  using Impl = internal::mutex::lock_impl_t<unique_lock<M>>;

 public:
  unique_lock() = default;
  explicit unique_lock(M& m) GHOST_ACQUIRE(m) : Impl(m) {}
  unique_lock(M& m, std::defer_lock_t t) noexcept GHOST_EXCLUDES(m)
    : Impl(m, t) {}
  unique_lock(M& m, std::adopt_lock_t t) GHOST_REQUIRES(m) : Impl(m, t) {}
  ~unique_lock() GHOST_RELEASE() = default;

  void lock() GHOST_ACQUIRE() { Impl::lock(); }
  bool try_lock() GHOST_TRY_ACQUIRE(true) { return Impl::try_lock(); }
  template <typename R, typename P>
  bool try_lock_for(const std::chrono::duration<R, P>& timeout)
      GHOST_TRY_ACQUIRE(true) {
    return Impl::try_lock_for(timeout);
  }
  template <typename C, typename D>
  bool try_lock_until(const std::chrono::time_point<C, D>& timeout)
      GHOST_TRY_ACQUIRE(true) {
    return Impl::try_lock_until(timeout);
  }
  void unlock() GHOST_RELEASE() { Impl::unlock(); }
};

// scoped_lock (for one mutex type).
template <typename M>
class GHOST_SCOPED_CAPABILITY scoped_lock<M>
    : public internal::mutex::lock_impl_t<scoped_lock<M>> {
  using Impl = internal::mutex::lock_impl_t<scoped_lock<M>>;

 public:
  explicit scoped_lock(M& m) GHOST_ACQUIRE(m) : Impl(m) {}
  scoped_lock(std::adopt_lock_t t, M& m) GHOST_REQUIRES(m) : Impl(t, m) {}
  ~scoped_lock() GHOST_RELEASE() = default;
};

// scoped_lock (for two mutex types).
template <typename M1, typename M2>
class GHOST_SCOPED_CAPABILITY scoped_lock<M1, M2>
    : public internal::mutex::lock_impl_t<scoped_lock<M1, M2>> {
  using Impl = internal::mutex::lock_impl_t<scoped_lock<M1, M2>>;

 public:
  explicit scoped_lock(M1& m1, M2& m2) GHOST_ACQUIRE(m1, m2) : Impl(m1, m2) {}
  scoped_lock(std::adopt_lock_t t, M1& m1, M2& m2) GHOST_REQUIRES(m1, m2)
      : Impl(t, m1, m2) {}
  ~scoped_lock() GHOST_RELEASE() = default;
};

// scoped_lock (for three mutex types).
template <typename M1, typename M2, typename M3>
class GHOST_SCOPED_CAPABILITY scoped_lock<M1, M2, M3>
    : public internal::mutex::lock_impl_t<scoped_lock<M1, M2, M3>> {
  using Impl = internal::mutex::lock_impl_t<scoped_lock<M1, M2, M3>>;

 public:
  explicit scoped_lock(M1& m1, M2& m2, M3& m3) GHOST_ACQUIRE(m1, m2, m3)
      : Impl(m1, m2, m3) {}
  scoped_lock(std::adopt_lock_t t, M1& m1, M2& m2, M3& m3)
      GHOST_REQUIRES(m1, m2, m3)
      : Impl(t, m1, m2, m3) {}
  ~scoped_lock() GHOST_RELEASE() = default;
};

// scoped_lock (for four mutex types).
template <typename M1, typename M2, typename M3, typename M4>
class GHOST_SCOPED_CAPABILITY scoped_lock<M1, M2, M3, M4>
    : public internal::mutex::lock_impl_t<scoped_lock<M1, M2, M3, M4>> {
  using Impl = internal::mutex::lock_impl_t<scoped_lock<M1, M2, M3, M4>>;

 public:
  explicit scoped_lock(M1& m1, M2& m2, M3& m3, M4& m4)
      GHOST_ACQUIRE(m1, m2, m3, m4)
      : Impl(m1, m2, m3, m4) {}
  scoped_lock(std::adopt_lock_t t, M1& m1, M2& m2, M3& m3, M4& m4)
      GHOST_REQUIRES(m1, m2, m3, m4)
      : Impl(t, m1, m2, m3, m4) {}
  ~scoped_lock() GHOST_RELEASE() = default;
};

// scoped_lock (for variadic).
template <typename... M>
class scoped_lock : public internal::mutex::lock_impl_t<scoped_lock<M...>> {
  using Impl = internal::mutex::lock_impl_t<scoped_lock<M...>>;

 public:
  explicit scoped_lock(M&... m) : Impl(m...) {}
  // NOLINTNEXTLINE
  scoped_lock(std::adopt_lock_t t, M&... m) : Impl(t, m...) {}
  ~scoped_lock() = default;
};

}  // namespace ghost

