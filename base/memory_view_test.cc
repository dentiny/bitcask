#include "base/memory_view.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <list>
#include <string>
#include <vector>

namespace {

constexpr size_t kAllocSize = 10;

}  // namespace

namespace base {

TEST(MemoryViewTest, InitializeWithString) {
  // Initialize with std::string.
  {
    std::string s(kAllocSize, '\0');
    MemoryView mem_view(s);
    EXPECT_EQ(s.data(), mem_view.Data());
    EXPECT_EQ(s.size(), mem_view.GetAllocSize());
  }

  // Initialize with std::vector of char.
  {
    std::vector<char> s(kAllocSize, '\0');
    MemoryView mem_view(s);
    EXPECT_EQ(s.data(), mem_view.Data());
    EXPECT_EQ(s.size(), mem_view.GetAllocSize());
  }

  // Initialize with std::vector of uint8, which is convertible to char.
  {
    std::vector<uint8_t> s(kAllocSize, 0);
    MemoryView mem_view(s);
    // NOLINTNEXTLINE
    EXPECT_EQ(reinterpret_cast<char*>(s.data()), mem_view.Data());
    EXPECT_EQ(s.size(), mem_view.GetAllocSize());
  }
}

}  // namespace base
