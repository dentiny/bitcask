// Printer for enums.

#pragma once

#include <type_traits>

#include <magic_enum.hpp>

namespace ghost {

struct EnumPrinter {
  template <typename T, std::enable_if_t<std::is_enum_v<T>, int> = 0>
  void operator()(std::ostream& os, const T& value) const {
    os << magic_enum::enum_name(value);
  }
};

}  // namespace ghost
