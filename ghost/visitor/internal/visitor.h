#pragma once

#include <utility>

namespace ghost {

// Struct used to break ties for calls that would otherwise be ambiguous.
// Reference: https://abseil.io/tips/229
struct general {};
struct specialize : general {};

// A callable that prefers using the 'visitor' over 'fallback' even when both
// can be used.
struct VisitImpl {
  template <typename V, typename F, typename... Args>
  std::invoke_result_t<V, Args...> operator()(V&& visitor, F&&, specialize,
                                              Args&&... args) const {
    return visitor(std::forward<Args>(args)...);
  }

  template <typename V, typename F, typename... Args>
  std::invoke_result_t<F, Args...> operator()(V&&, F&& fallback, general,
                                              Args&&... args) const {
    return fallback(std::forward<Args>(args)...);
  }
};

}  // namespace ghost
