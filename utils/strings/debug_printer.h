#pragma once

#include <sstream>
#include <string>
#include <ostream>

#include "ghost/visitor/visitor.h"
#include "utils/strings/absl_stringify_printer.h"
#include "utils/strings/container_printer.h"
#include "utils/strings/enum_printer.h"
#include "utils/strings/ostream_printer.h"
#include "utils/strings/std_printer.h"
#include "utils/strings/type_printer.h"

namespace ghost {

// The ordered visitor of printers to use for printing debug strings.
using BaseDebugStringPrinters =
    OrderedVisitor<StdPrinter<>, OstreamPrinter, EnumPrinter,
    ContainerPrinter<>, AbslStringifyPrinter, TypePrinter>;
using DebugStringPrinters = OrderedVisitor<
    StdPrinter<BaseDebugStringPrinters>, OstreamPrinter, EnumPrinter,
    ContainerPrinter<BaseDebugStringPrinters>,
    AbslStringifyPrinter, TypePrinter>;

template <typename T>
std::string DebugString(const T& value) {
  std::stringstream ss;
  DebugStringPrinters{}(ss, value);  // NOLINT
  return ss.str();
}

}  // namespace ghost
