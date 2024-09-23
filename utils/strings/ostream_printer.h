// Printer for types which supports stream operator.

#pragma once

#include <ostream>
#include <type_traits>

#include "base/logging.h"

namespace ghost {

template <typename T, typename = void>
struct is_streamable : std::false_type {};
template <typename T>
struct is_streamable<T, std::void_t<decltype(std::declval<std::ostream&>()
                                             << std::declval<T>())>>
    : std::true_type {};
template <typename T>
inline constexpr bool is_streamable_v = is_streamable<T>::value;

struct OstreamPrinter {
  template <typename T, std::enable_if_t<is_streamable_v<T>, int> = 0>
  void operator()(std::ostream& os, const T& value) const {
    os << value;
  }

  // Specialized boolean implementation.
  template <>
  void operator()<bool>(std::ostream& os, const bool& value) const {
    os << (value ? "true" : "false");
  }
};

}  // namespace ghost
