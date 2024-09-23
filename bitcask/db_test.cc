#include "bitcask/db_impl.h"

#include <gtest/gtest.h>

#include <thread>
#include <vector>

#include "absl/strings/str_format.h"
#include "base/logging.h"
#include "utils/scoped_directory.h"
#include "utils/status/testing.h"
#include "utils/thread_pool.h"

namespace bitcask {

class BitcaskTest : public ::testing::Test {
 protected:
  utils::ScopedDirectory dir_;
};

TEST_F(BitcaskTest, BasicuPutAndGet) {
  const auto db_folder = dir_.get_directory();
  GHOST_ASSIGN_OR_ASSERT(auto db, DbImpl::New(db_folder, Config{}));
  ASSERT_EQ(db->Get("key").code(), ghost::StatusCode::kNotFound);
  // Put.
  GHOST_ASSERT_OK(db->Put("key", "val-1"));
  GHOST_ASSIGN_OR_ASSERT(auto val, db->Get("key"));
  ASSERT_EQ(val, "val-1");
  // Overwrite.
  GHOST_ASSERT_OK(db->Put("key", "val-2"));
  GHOST_ASSIGN_OR_ASSERT(val, db->Get("key"));
  ASSERT_EQ(val, "val-2");
  // Delete.
  GHOST_ASSERT_OK(db->Delete("key"));
  ASSERT_EQ(db->Get("key").code(), ghost::StatusCode::kNotFound);
  // Put.
  GHOST_ASSERT_OK(db->Put("key", "val-3"));
  GHOST_ASSIGN_OR_ASSERT(val, db->Get("key"));
  ASSERT_EQ(val, "val-3");
  // Crash recovery.
  db = nullptr;
  GHOST_ASSIGN_OR_ASSERT(db, DbImpl::New(db_folder, Config{}));
  GHOST_ASSIGN_OR_ASSERT(val, db->Get("key"));
  ASSERT_EQ(val, "val-3");
}

TEST_F(BitcaskTest, PutWithSmallNewFileSizeThreshold) {
  const auto db_folder = dir_.get_directory();
  GHOST_ASSIGN_OR_ASSERT(
      auto db, DbImpl::New(db_folder, Config{
                                          .compact_file_size_threshold = 20,
                                          .seal_file_size_threshold = 50,
                                      }));
  ASSERT_EQ(db->Get("key").code(), ghost::StatusCode::kNotFound);
  // Put.
  GHOST_ASSERT_OK(db->Put("key", "val-1"));
  GHOST_ASSIGN_OR_ASSERT(auto val, db->Get("key"));
  ASSERT_EQ(val, "val-1");
  // Overwrite.
  GHOST_ASSERT_OK(db->Put("key", "val-2"));
  GHOST_ASSIGN_OR_ASSERT(val, db->Get("key"));
  ASSERT_EQ(val, "val-2");
  // Delete.
  GHOST_ASSERT_OK(db->Delete("key"));
  ASSERT_EQ(db->Get("key").code(), ghost::StatusCode::kNotFound);
  // Put.
  GHOST_ASSERT_OK(db->Put("key", "val-3"));
  GHOST_ASSIGN_OR_ASSERT(val, db->Get("key"));
  ASSERT_EQ(val, "val-3");
  // Crash recovery.
  db = nullptr;
  GHOST_ASSIGN_OR_ASSERT(db, DbImpl::New(db_folder, Config{}));
  GHOST_ASSIGN_OR_ASSERT(val, db->Get("key"));
  ASSERT_EQ(val, "val-3");
}

TEST_F(BitcaskTest, ConcurrencyTest) {
  const auto db_folder = dir_.get_directory();
  GHOST_ASSIGN_OR_ASSERT(
      auto db, DbImpl::New(db_folder, Config{
                                          .compact_file_size_threshold = 1000,
                                          .seal_file_size_threshold = 1000,
                                      }));

  constexpr int kWriteThdNum = 5;
  constexpr int kKeyValNum = 5;
  utils::ThreadPool tp{kWriteThdNum};
  std::vector<std::thread> write_threads;
  write_threads.reserve(kWriteThdNum);
  for (int tid = 0; tid < kWriteThdNum; ++tid) {
    tp.Push([tid = tid, &db]() {
      for (int kvid = 0; kvid < kKeyValNum; ++kvid) {
        std::string key = absl::StrFormat("key-%d-%d", tid, kvid);
        std::string val = absl::StrFormat("val-%d-%d", tid, kvid);
        GHOST_ASSERT_OK(db->Put(std::move(key), std::move(val)));
      }
    });
  }
  tp.Wait();

  const std::unordered_map<std::string, std::string> expected = {
      {"key-4-4", "val-4-4"},   {"key-1-4", "val-1-4"},
      {"key-1-6", "val-1-6"}
  };
  for (const auto& [key, expected_val] : expected) {
    GHOST_ASSIGN_OR_ASSERT(auto actual_val, db->Get(key));
    ASSERT_EQ(actual_val, expected_val);
  }
  for (auto& thd : write_threads) {
    thd.join();
  }
}

}  // namespace bitcask
