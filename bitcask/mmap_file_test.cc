#include "bitcask/mmap_file.h"

#include <gtest/gtest.h>

#include "base/file/file.h"
#include "utils/scoped_directory.h"
#include "utils/status/testing.h"

namespace bitcask {

class MmapReadOnlyFileTest : public ::testing::Test {
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

TEST_F(MmapReadOnlyFileTest, PReadTest) {
  GHOST_ASSIGN_OR_ASSERT(auto file,
                        MmapReadOnlyFile::New(temp_fname_));
  GHOST_ASSIGN_OR_ASSERT(auto content,
                        file->PRead(/*offset=*/0, /*bytes_to_read=*/5));
  ASSERT_EQ(content, "hello");
  GHOST_ASSIGN_OR_ASSERT(content,
                        file->PRead(/*offset=*/6, /*bytes_to_read=*/5));
  ASSERT_EQ(content, std::string_view{"world"});
}

}  // namespace bitcask
