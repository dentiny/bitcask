#pragma once

#include <condition_variable>
#include <cstdint>
#include <memory>
#include <string>
#include <thread>
#include <utility>
#include <variant>
#include <vector>

#include "absl/container/flat_hash_map.h"
#include "base/file/base_file_handle.h"
#include "bitcask/base_db.h"
#include "bitcask/config.h"
#include "bitcask/file_handle_wrapper.h"
#include "ghost/mutex/mutex.h"
#include "utils/status/status.h"
#include "utils/status/status_or.h"

namespace bitcask {

class DbImpl : public BaseDb {
 public:
  static ghost::StatusOr<std::unique_ptr<BaseDb>> New(const std::string& folder,
                                                     Config config);
  ~DbImpl() override;

  // 1. Get entry based on key (file id and offset).
  // 2. Acquire a file handle with reference count.
  // 3. Decode the row and return value.
  ghost::StatusOr<std::string> Get(std::string_view key) override;

  // 1. Encode key-value pair into bytes.
  // 2. Dump the byte array into the unsealed file, notice a write lock needs to
  // be acquired.
  // 3. If unsealed file exceeds configured threshold, seal the current file and
  // start a new file.
  // 4. If accumulated seal file size exceeds configured threshold, trigger
  // background compaction.
  ghost::Status Put(std::string key, std::string val) override;

  ghost::Status Delete(std::string key) override;

 private:
  struct DiskHandle {
    FileHandleWrapperPtr handle;
    uint64_t offset = 0;
  };
  struct InlineHandle {
    std::string fname;
    std::string value;
  };

  struct Entry {
    std::variant<std::monostate, DiskHandle, InlineHandle> handle =
        std::monostate{};
    uint64_t ts_nano_sec = 0;
  };

  struct Metadata {
    uint64_t ts_nano_sec = 0;
    uint64_t key_len = 0;
    uint64_t val_len = 0;
  };

  // Data structure used for recovery and compaction.
  struct ValueWrapper {
    std::string value;
    uint64_t timestamp;
    std::string filename;
    uint64_t offset;
  };
  using CompactedKeyValue = absl::flat_hash_map<std::string, ValueWrapper>;

  DbImpl(std::string folder, Config config);
  DbImpl(std::string folder, FileHandleWrapperPtr handle, Config config);

  // Encode the key-value pair into bytes array.
  static std::string Encode(const std::string& key, const std::string& val,
                            uint64_t ts_nano_sec);

  // Implementation for compaction thread.
  void CompactionThreadImpl();

  // Crash recover from existing db file, meanwhile compact these files
  ghost::Status RecoverAndCompact(const std::vector<std::string>& files)
      GHOST_NO_THREAD_SAFETY_ANALYSIS;

  // Perform background compaction operation.
  // 1. Read all sealed files and perform compaction.
  // 2. Dump merged key-value pairs into a new sealed file.
  // 3. Update in-memory hash entries, so old files won't be visited for new
  // read operations.
  // 4. Decrease reference count (potentially delete) old sealed files.
  ghost::Status Compact(const std::vector<std::string>& files);

  // Read all key-value pairs and perform a compaction for all given [files].
  template <typename Container>
  ghost::StatusOr<CompactedKeyValue> CompactImpl(const Container& files);

  // Make entry, considering both disk storage and inline storage.
  Entry MakeEntry(std::string val, uint64_t timestamp,
                  FileHandleWrapperPtr handle, uint64_t offset);

  // Get value from entry.
  ghost::StatusOr<std::string> GetValueFromDiskEntry(const DiskHandle& handle);
  ghost::StatusOr<std::string> GetValueFromEntry(Entry entry);

  // Get filename from entry.
  const std::string& GetFnameFromEntry(const Entry& entry);

  const std::string folder_;
  const Config config_;

  absl::flat_hash_map<std::string, Entry> entries_ GHOST_GUARDED_BY(read_mtx_);
  ghost::mutex read_mtx_;

  uint64_t file_len_ GHOST_GUARDED_BY(write_mtx_) = 0;
  FileHandleWrapperPtr active_handle_ GHOST_GUARDED_BY(write_mtx_);

  ghost::mutex write_mtx_;

  // Background thread to perform compaction.
  ghost::mutex compaction_mtx_;
  std::condition_variable_any compaction_cv_;
  bool stopped_ GHOST_GUARDED_BY(compaction_mtx_) = false;
  bool compaction_triggered_ GHOST_GUARDED_BY(compaction_mtx_) = false;
  // Thread should be placed as the last data member.
  std::thread compaction_thread_;
};

}  // namespace bitcask
