#include "base/file/os_write_handle.h"

#include <gtest/gtest.h>

#include "base/file/file.h"
#include "base/file/os_read_handle.h"
#include "utils/scope_guard.h"
#include "utils/status/testing.h"

namespace base::file {

namespace {
constexpr const char* const kTestFile = "/tmp/write_handle_test_testfile";
constexpr std::string_view kContent = "hello world";
}  // namespace

TEST(OsWriteHandleTest, BasicWrite) {
  {
    GHOST_ASSIGN_OR_ASSERT(auto file_handle, OsWriteHandle::New(kTestFile));
    GHOST_ASSERT_OK(file_handle->Write(kContent));
    GHOST_ASSERT_OK(file_handle->Write(kContent.data(), kContent.length()));
    GHOST_ASSERT_OK(file_handle->Close());
  }
  SCOPE_EXIT { GHOST_LOG_IF_ERROR(ERROR, Delete(kTestFile)); };
  GHOST_ASSIGN_OR_ASSERT(const uint64_t file_size, GetFileSize(kTestFile));

  // Check written content.
  {
    GHOST_ASSIGN_OR_ASSERT(auto file_handle, OsReadHandle::New(kTestFile));
    std::string actual_content;
    actual_content.resize(file_size);
    MemoryView memory_view{actual_content};
    GHOST_ASSERT_OK(file_handle->Read(&memory_view, file_size));
    ASSERT_EQ(actual_content, "hello worldhello world");
    ASSERT_EQ(memory_view.GetActualSize(), 22);
  }
}

}  // namespace base::file
