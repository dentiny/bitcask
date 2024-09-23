#include "ghost/visitor/visitor.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "utils/strings/container_printer.h"
#include "utils/strings/std_byte_printer.h"
#include "utils/strings/std_printer.h"
#include "utils/strings/type_printer.h"

namespace ghost {

namespace {
class MyClass {};
}  // namespace

TEST(VisitorTest, BasicTest) {
  OrderedVisitor ostream_visitor{StdPrinter{}};
  OrderedVisitor container_visitor{ContainerPrinter{}};
  OrderedVisitor type_printer{TypePrinter{}};

  // An ordered visitor with all printers.
  OrderedVisitor<StdPrinter, ContainerPrinter, TypePrinter> ordered_visitor{};

  VisitImpl{}(ordered_visitor, ostream_visitor, specialize{}, std::cout,
                   std::byte{10});
  VisitImpl{}(ordered_visitor, ostream_visitor, specialize{}, std::cout,
                   10);
  VisitImpl{}(ordered_visitor, ostream_visitor, specialize{}, std::cout,
                   std::vector<int>{1, 2, 3});
  VisitImpl{}(ordered_visitor, ostream_visitor, specialize{}, std::cout,
                   MyClass{});
}

}  // namespace ghost
