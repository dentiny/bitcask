// Printer for types which defines `AbslStringify`.

#pragma once

#include <iostream>
#include <type_traits>

#include "absl/strings/str_cat.h"

namespace ghost {

// TODO(hjiang): Later version of abseil contains abseil string defined trait.
template <typename T, typename = void, typename = void>
struct is_stringifyable : std::false_type {};

template <typename T, typename SinkType>
struct is_stringifyable<
    T, SinkType,
    std::void_t<decltype(AbslStringify(std::declval<SinkType&>(),
                                       std::declval<const T&>()))>>
    : std::true_type {};

template <typename T>
inline constexpr bool is_stringifyable_v =
    is_stringifyable<T, std::ostream>::value;

struct AbslStringifyPrinter {
  template <typename T, std::enable_if_t<is_stringifyable_v<T>, int> = 0>
  void operator()(std::ostream& os, const T& obj) const {
    os << absl::StrCat(obj);
  }
};

}  // namespace ghost
