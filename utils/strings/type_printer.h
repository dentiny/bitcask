// Printer for variable type.

#pragma once

#include <ostream>
#include <typeinfo>

#include <boost/core/demangle.hpp>

#include "utils/strings/get_type_name.h"

namespace ghost {

struct TypePrinter {
  void operator()(std::ostream& os, const std::type_info& value) const {
    // `std::type_info::name` returns mangled symbol name.
    os << boost::core::demangle(value.name());
  }

  template <typename T>
  void operator()(std::ostream& os, const T& value) const {
    os << absl::StreamFormat("<Type: %s, address: %p>",
        GetTypeName<T>(), &value);
  }
};

}  // namespace ghost
