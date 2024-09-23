#include "bitcask/file_handle_wrapper.h"

#include <gtest/gtest.h>

#include <string>

#include "base/file/file.h"
#include "utils/scoped_directory.h"
#include "utils/status/testing.h"

namespace bitcask {

class FileHandleTest : public ::testing::Test {
 public:
  void SetUp() override {
    temp_fname_ = directory_.get_directory() + "/tmpfile";
    GHOST_ASSIGN_OR_ASSERT(auto handle,
        base::file::Open(temp_fname_, base::file::FileOpenMode::kWriteOnly));
    GHOST_ASSERT_OK(handle->Write("hello world"));
    GHOST_ASSERT_OK(handle->Flush());
    GHOST_ASSERT_OK(handle->Close());
  }

 protected:
  utils::ScopedDirectory directory_;
  std::string temp_fname_;
};

TEST_F(FileHandleTest, UnreferenceTest) {
  GHOST_ASSIGN_OR_ASSERT(auto handle,
                        CreateFileHandleWithRefCount(temp_fname_));
  handle->MarkStale();
  handle.reset();
  GHOST_ASSIGN_OR_ASSERT(bool exists, base::file::Exists(temp_fname_));
  ASSERT_FALSE(exists);
}

}  // namespace bitcask
