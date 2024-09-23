// std::array related utils.

#pragma once

#include <array>
#include <type_traits>
#include <utility>

namespace utils::container {

namespace internal {

template <bool IsMoveConstructible = false, typename T, std::size_t... I>
constexpr std::array<std::remove_cv_t<T>, sizeof...(I)> to_array_impl(
    // NOLINTNEXTLINE
    T (&a)[sizeof...(I)], std::index_sequence<I...>) {
  if constexpr (IsMoveConstructible) {
    static_assert(std::is_move_constructible_v<T>);
    return {{std::move(a[I])...}};
  } else {
    return {{a[I]...}};
  }
}

}  // namespace internal

template <typename T, std::size_t N>
// NOLINTNEXTLINE
constexpr std::array<std::remove_cv_t<T>, N> to_array(T (&a)[N]) {
  static_assert(!std::is_array_v<T>);
  static_assert(std::is_constructible_v<T, T&>);
  return internal::to_array_impl(a, std::make_index_sequence<N>{});
}

template <typename T, std::size_t N>
// NOLINTNEXTLINE
constexpr std::array<std::remove_cv_t<T>, N> to_array(T(&&a)[N]) {
  static_assert(!std::is_array_v<T>);
  return internal::to_array_impl<std::is_move_constructible_v<T>>(
      a, std::make_index_sequence<N>{});
}

}  // namespace utils::container
