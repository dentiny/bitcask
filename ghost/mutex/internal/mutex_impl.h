// Bind annotated mutex and lock type with standard implementation, so that
// ghost::unique_lock<ghost::mutex> could inherit std::unique_lock<std::mutex>.

#pragma once

#include <mutex>

namespace ghost {

// Forward declaration for annotated mutex types.
class mutex;

// Forward declaration for annotated lock types.
template <typename M>
class unique_lock;
template <typename M>
class lock_guard;

template <typename... M>
class scoped_lock;

namespace internal::mutex {

// Type alias for mutex types.
template <typename T>
struct standard_impl {
  using type = T;
};
template <typename T>
using standard_impl_t = typename standard_impl<T>::type;

// Mutex specialization.
template <>
struct standard_impl<ghost::mutex> {
  using type = std::mutex;
};

// Type alias for lock types.
template <typename M>
using mutex_impl_t = standard_impl_t<M>;

// Lock specialization.
template <typename M>
struct standard_impl<ghost::unique_lock<M>> {
  using type = std::unique_lock<mutex_impl_t<M>>;
};

template <typename M>
struct standard_impl<ghost::lock_guard<M>> {
  using type = std::lock_guard<mutex_impl_t<M>>;
};

template <typename... M>
struct standard_impl<ghost::scoped_lock<M...>> {
  using type = std::scoped_lock<mutex_impl_t<M>...>;
};

template <typename L>
using lock_impl_t = standard_impl_t<L>;

}  // namespace internal::mutex

}  // namespace ghost

