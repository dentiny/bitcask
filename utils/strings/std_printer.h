// Printer for types which supports stream operator.

#pragma once

#include <ostream>
#include <system_error>
#include <type_traits>

#include "base/logging.h"
#include "utils/strings/ostream_printer.h"

namespace ghost {

template <typename ValuePrinter = OstreamPrinter>
struct StdPrinter {
 public:
  ValuePrinter printer{};

  // Printer for `std::error_code`.
  void operator()(std::ostream& os, std::error_code value) const {
    os << value.message();
  }

  // Printer for `std::byte`.
  void operator()(std::ostream& os, const std::byte& value) const {
    os << absl::StreamFormat("b[%#04x]", static_cast<unsigned int>(value));
  }
};

}  // namespace ghost
