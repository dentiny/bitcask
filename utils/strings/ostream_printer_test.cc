#include "utils/strings/ostream_printer.h"
#include <gtest/gtest.h>

#include <string>
#include <string_view>

namespace ghost {

TEST(OstreamPrinterTest, BasicTest) {
  OstreamPrinter printer{};
  // Integer type.
  printer(std::cout, 1);
  // Double type.
  printer(std::cout, 1.6);
  // Char type.
  printer(std::cout, "hello");
  // String type.
  printer(std::cout, std::string{"hello"});
  // String view type.
  printer(std::cout, std::string_view{"hello"});
}

}  // namespace ghost
