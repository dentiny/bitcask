#include "base/file/os_read_handle.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <string_view>

#include "base/memory_view.h"
#include "utils/status/testing.h"

namespace base::file {

namespace {
constexpr const char* const kTestFile = "base/file/testdata/test_file.txt";
constexpr uint64_t kFileSize = 11;
}  // namespace

TEST(OsFileHandleTest, BasicRead) {
  GHOST_ASSIGN_OR_ASSERT(auto file_handle, OsReadHandle::New(kTestFile));
  ASSERT_EQ(file_handle->GetFileSize(), kFileSize);

  std::array<char, kFileSize> content{};
  MemoryView memory_view{content};

  // Read.
  GHOST_ASSERT_OK(file_handle->Read(&memory_view, 5));
  std::string actual_content{content.begin(), content.begin() + 5};
  ASSERT_EQ(actual_content, "hello");
  ASSERT_EQ(memory_view.GetActualSize(), 5);

  // PRead.
  GHOST_ASSERT_OK(file_handle->PRead(&memory_view, 5, 6));
  actual_content = std::string{content.begin(), content.begin() + 5};
  ASSERT_EQ(actual_content, "world");
  ASSERT_EQ(memory_view.GetActualSize(), 5);

  // Read full content.
  GHOST_ASSIGN_OR_ASSERT(const auto full_content, file_handle->CompleteRead());
  ASSERT_EQ(full_content, "hello world");
}

}  // namespace base::file
