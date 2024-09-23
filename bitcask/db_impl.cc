#include "bitcask/db_impl.h"

#include "absl/container/flat_hash_set.h"
#include "absl/memory/memory.h"
#include "base/file/file.h"
#include "bitcask/mmap_file.h"
#include "bitcask/utils.h"
#include "utils/scope_guard.h"
#include "utils/status/status_macros.h"
#include "utils/strings/resize_uninitialized.h"
#include "utils/time_utils.h"

namespace bitcask {

/*static*/ ghost::StatusOr<std::unique_ptr<BaseDb>> DbImpl::New(
    const std::string& folder, Config config) {
  GHOST_ASSIGN_OR_RETURN(
      auto files, base::file::ListDirRecursively(folder));

  // If given an empty folder, create a new active file.
  if (files.empty()) {
    GHOST_ASSIGN_OR_RETURN(auto active_handle, CreateFileHandleWithRefCount(
                                                  GetRandomFilename(folder)));
    auto db = absl::WrapUnique(new DbImpl(
        std::move(folder), std::move(active_handle), std::move(config)));
    return ghost::StatusOr<std::unique_ptr<BaseDb>>{std::move(db)};
  }

  // Otherwise, recovery from existing files and perform a compaction.
  auto db = absl::WrapUnique(new DbImpl(std::move(folder), std::move(config)));
  GHOST_RETURN_IF_ERROR(db->RecoverAndCompact(files));
  return ghost::StatusOr<std::unique_ptr<BaseDb>>{std::move(db)};
}

DbImpl::DbImpl(std::string folder, Config config)
    : folder_(std::move(folder)), config_(std::move(config)) {
  compaction_thread_ = std::thread([this]() { CompactionThreadImpl(); });
}
DbImpl::DbImpl(std::string folder, FileHandleWrapperPtr handle, Config config)
    : folder_(std::move(folder)),
      config_(std::move(config)),
      active_handle_(std::move(handle)) {
  compaction_thread_ = std::thread([this]() { CompactionThreadImpl(); });
}

void DbImpl::CompactionThreadImpl() {
  for (;;) {
    {
      ghost::unique_lock compaction_lck(compaction_mtx_);
      compaction_cv_.wait(compaction_lck,
                          [this]() GHOST_REQUIRES(compaction_mtx_) {
                            return compaction_triggered_ || stopped_;
                          });
      // Stop has been requested.
      if (stopped_) {
        return;
      }
      compaction_triggered_ = false;
    }

    GHOST_ASSIGN_OR_THROW(auto files, base::file::ListDirRecursively(folder_));

    std::vector<std::string> files_to_compact;
    GHOST_CHECK_GE(FAILED_PRECONDITION, files.size(), 1);
    files_to_compact.reserve(files.size() - 1);
    {
      const ghost::lock_guard write_lck(write_mtx_);
      for (const auto& cur_file : files) {
        if (cur_file == active_handle_->Name()) {
          files_to_compact.emplace_back(cur_file);
        }
      }
    }
    GHOST_THROW_IF_ERROR(Compact(files_to_compact));
  }
}

DbImpl::~DbImpl() {
  // Join compaction thread before closing the active handle.
  {
    const ghost::lock_guard compaction_lck(compaction_mtx_);
    stopped_ = true;
    compaction_cv_.notify_one();
  }
  compaction_thread_.join();
  if (active_handle_ != nullptr) {
    GHOST_THROW_IF_ERROR(active_handle_->Close());
  }
}

ghost::Status DbImpl::RecoverAndCompact(const std::vector<std::string>& files) {
  GHOST_ASSIGN_OR_RETURN(auto compacted_key_val, CompactImpl(files));
  GHOST_ASSIGN_OR_RETURN(
      auto handle, CreateFileHandleWithRefCount(GetRandomFilename(folder_)));
  uint64_t offset = 0;
  for (auto iter = compacted_key_val.begin(); iter != compacted_key_val.end();
       ++iter) {
    const auto& key = iter->first;
    const auto& value_wrapper = iter->second;
    auto content = Encode(key, value_wrapper.value, value_wrapper.timestamp);
    iter->second.filename = handle->Name();
    iter->second.offset = offset;
    GHOST_RETURN_IF_ERROR(handle->Write(content));
    entries_[key] = MakeEntry(std::move(value_wrapper.value),
                              value_wrapper.timestamp, handle, offset);
    offset += content.length();

    // If file size already reaches threshold, dump existing key-value pairs and
    // start a new one.
    if (offset >= config_.compact_file_size_threshold) {
      offset = 0;
      GHOST_RETURN_IF_ERROR(handle->Flush());
      GHOST_RETURN_IF_ERROR(handle->Close());
      GHOST_ASSIGN_OR_RETURN(
          handle, CreateFileHandleWithRefCount(GetRandomFilename(folder_)));
    }
  }

  // Update active handle and its offset.
  active_handle_ = std::move(handle);
  file_len_ = offset;

  // Delete all other files.
  for (const auto& cur_file : files) {
    GHOST_RETURN_IF_ERROR(base::file::Delete(cur_file));
  }
  return ghost::Ok();
}

/*static*/ std::string DbImpl::Encode(const std::string& key,
                                      const std::string& val,
                                      uint64_t ts_nano_sec) {
  const Metadata metadata{
      .ts_nano_sec = ts_nano_sec,
      .key_len = key.length(),
      .val_len = val.length(),
  };
  const uint64_t len = metadata.key_len + metadata.val_len + sizeof(metadata);
  auto content = utils::strings::CreateStringWithSizeUninitialized(len);

  uint64_t off = 0;
  std::memmove(content.data() + off, &metadata, sizeof(metadata));
  off += sizeof(metadata);
  std::memmove(content.data() + off, key.data(), key.length());
  off += key.length();
  std::memmove(content.data() + off, val.data(), val.length());

  return content;
}

DbImpl::Entry DbImpl::MakeEntry(std::string val, uint64_t timestamp,
                                FileHandleWrapperPtr handle, uint64_t offset) {
  if (val.length() < config_.inline_val_threshold) {
    DbImpl::InlineHandle inline_handle{
        .fname = handle->Name(),
        .value = std::move(val),
    };
    return DbImpl::Entry{
        .handle = std::move(inline_handle),
        .ts_nano_sec = timestamp,
    };
  }

  DbImpl::DiskHandle disk_handle{
      .handle = handle,
      .offset = offset,
  };
  return DbImpl::Entry{
      .handle = std::move(disk_handle),
      .ts_nano_sec = timestamp,
  };
}

ghost::StatusOr<std::string> DbImpl::GetValueFromDiskEntry(
    const DiskHandle& entry) {
  Metadata metadata;
  GHOST_RETURN_IF_ERROR(
      entry.handle->PRead(&metadata, sizeof(metadata), entry.offset));
  auto val =
      utils::strings::CreateStringWithSizeUninitialized(metadata.val_len);
  const uint64_t value_offset =
      entry.offset + sizeof(metadata) + metadata.key_len;
  GHOST_RETURN_IF_ERROR(
      entry.handle->PRead(val.data(), metadata.val_len, value_offset));
  return val;
}

ghost::StatusOr<std::string> DbImpl::GetValueFromEntry(Entry entry) {
  auto* inline_handle = std::get_if<InlineHandle>(&entry.handle);
  if (inline_handle != nullptr) {
    return std::move(inline_handle->value);
  }
  auto& disk_handle = std::get<DiskHandle>(entry.handle);
  GHOST_ASSIGN_OR_RETURN(auto val, GetValueFromDiskEntry(disk_handle));
  return val;
}

// If [key] is deleted, it would be remove from [entries_].
ghost::StatusOr<std::string> DbImpl::Get(std::string_view key) {
  Entry entry;
  {
    const ghost::lock_guard read_lck(read_mtx_);
    auto iter = entries_.find(key);
    GHOST_SCHECK_NE(NOT_FOUND, iter, entries_.end());
    entry = iter->second;
  }

  GHOST_ASSIGN_OR_RETURN(auto val, GetValueFromEntry(std::move(entry)));
  return val;
}

ghost::Status DbImpl::Put(std::string key, std::string val) {
  // TODO(hjiang): Have a better representation for timestamp, to make sure db
  // key-value pairs won't suffer clock skew.
  const uint64_t now = utils::time::GetSteadyNowNanoSecSinceEpoch();
  const auto content = Encode(key, val, now);

  // Dump key value pair to file and flushes.
  //
  // TODO(hjiang): Avoid holding one lock for key-value pair write.
  const ghost::lock_guard write_lck(write_mtx_);
  GHOST_RETURN_IF_ERROR(active_handle_->Write(content));

  // Operations after successfully perform a write, and entries successfully
  // updated.
  const utils::ScopeGuard scope_guard{[&]() GHOST_REQUIRES(write_mtx_) {
    file_len_ += content.length();
    if (file_len_ < config_.seal_file_size_threshold) {
      return;
    }

    // Trigger creating a new active file.
    // TODO(hjiang): Error status propagation.
    auto new_active_handle =
        CreateFileHandleWithRefCount(GetRandomFilename(folder_)).value();
    active_handle_ = std::move(new_active_handle);
    file_len_ = 0;

    // Trigger compaction if necessary.
    {
      const ghost::lock_guard compaction_lck(compaction_mtx_);
      compaction_triggered_ = true;
      compaction_cv_.notify_one();
    }
  }};

  // If [val] to emplace is tombstone, remove from [entries_].
  const ghost::lock_guard read_lck(read_mtx_);
  if (val == kTombstone) {
    entries_.erase(key);
    return ghost::Ok();
  }

  // Otherwise, insert or overwrite entry.
  entries_[std::move(key)] =
      MakeEntry(std::move(val), now, active_handle_, file_len_);
  return ghost::Ok();
}

ghost::Status DbImpl::Delete(std::string key) {
  GHOST_RETURN_IF_ERROR(Put(std::move(key), std::string{kTombstone}));
  return ghost::Ok();
}

template <typename Container>
ghost::StatusOr<DbImpl::CompactedKeyValue> DbImpl::CompactImpl(
    const Container& files) {
  CompactedKeyValue compacted_key_val;
  for (const auto& fname : files) {
    GHOST_ASSIGN_OR_RETURN(auto mmap_handle, MmapReadOnlyFile::New(fname));
    const uint64_t file_size = mmap_handle->GetFileSize();
    uint64_t offset = 0;
    while (offset < file_size) {
      GHOST_ASSIGN_OR_RETURN(std::string_view content,
                            mmap_handle->PRead(offset, sizeof(Metadata)));
      // NOLINTNEXTLINE
      const Metadata& metadata = *(Metadata*)(content.data());
      SCOPE_EXIT {
        offset += sizeof(metadata) + metadata.key_len + metadata.val_len;
      };
      const uint64_t key_offset = offset + sizeof(metadata);
      GHOST_ASSIGN_OR_RETURN(const std::string_view key,
                            mmap_handle->PRead(key_offset, metadata.key_len));
      const uint64_t val_offset = key_offset + metadata.key_len;
      GHOST_ASSIGN_OR_RETURN(const std::string_view val,
                            mmap_handle->PRead(val_offset, metadata.val_len));
      auto iter = compacted_key_val.find(key);
      if (iter == compacted_key_val.end()) {
        compacted_key_val.emplace(key, ValueWrapper{
                                           .value = std::string{val},
                                           .timestamp = metadata.ts_nano_sec,
                                           .filename = "",  // Unused
                                           .offset = offset,
                                       });
        continue;
      }
      if (iter->second.timestamp < metadata.ts_nano_sec) {
        iter->second.timestamp = metadata.ts_nano_sec;
        iter->second.value = std::string{val};
        iter->second.offset = offset;
        continue;
      }
    }
  }
  return compacted_key_val;
}

template ghost::StatusOr<DbImpl::CompactedKeyValue> DbImpl::CompactImpl<
    std::vector<std::string>>(const std::vector<std::string>& files);
template ghost::StatusOr<DbImpl::CompactedKeyValue>
DbImpl::CompactImpl<absl::flat_hash_set<std::string>>(
    const absl::flat_hash_set<std::string>& files);

const std::string& DbImpl::GetFnameFromEntry(const Entry& entry) {
  auto* inline_handle = std::get_if<InlineHandle>(&entry.handle);
  if (inline_handle != nullptr) {
    return inline_handle->fname;
  }
  const auto& disk_handle = std::get<DiskHandle>(entry.handle);
  return disk_handle.handle->Name();
}

ghost::Status DbImpl::Compact(const std::vector<std::string>& files) {
  absl::flat_hash_set<std::string> old_sealed_files;
  {
    const ghost::lock_guard write_lck(write_mtx_);
    for (const auto& fname : files) {
      if (fname != active_handle_->Name()) {
        old_sealed_files.emplace(fname);
      }
    }
  }

  // Compact all existing sealed key-value pairs.
  GHOST_ASSIGN_OR_RETURN(auto compacted_key_val, CompactImpl(old_sealed_files));

  // Maps from filename to file handle wrapper (new files after compression).
  absl::flat_hash_map<std::string, FileHandleWrapperPtr> new_file_handles;

  // Dump key-value pairs into new sealed files.
  auto fname = GetRandomFilename(folder_);
  GHOST_ASSIGN_OR_RETURN(auto handle, CreateFileHandleWithRefCount(fname));
  new_file_handles.emplace(std::move(fname), handle);

  uint64_t offset = 0;
  for (auto iter = compacted_key_val.begin(); iter != compacted_key_val.end();
       ++iter) {
    const auto& key = iter->first;
    const auto& value_wrapper = iter->second;
    auto content = Encode(key, value_wrapper.value, value_wrapper.timestamp);
    iter->second.filename = handle->Name();
    iter->second.offset = offset;
    GHOST_RETURN_IF_ERROR(handle->Write(content));
    offset += content.length();

    // If file size already reaches threshold, dump existing key-value pairs and
    // start a new one.
    if (offset >= config_.compact_file_size_threshold) {
      offset = 0;
      GHOST_RETURN_IF_ERROR(handle->Flush());
      GHOST_RETURN_IF_ERROR(handle->Close());
      fname = GetRandomFilename(folder_);
      GHOST_ASSIGN_OR_RETURN(handle, CreateFileHandleWithRefCount(fname));
      new_file_handles.emplace(std::move(fname), handle);
    }
  }

  // Flush the last segment, even if it doesn't reach configured threshold.
  if (offset > 0) {
    GHOST_RETURN_IF_ERROR(handle->Flush());
    GHOST_RETURN_IF_ERROR(handle->Close());
  }

  // Update sealed files' entry to compacted files.
  const ghost::scoped_lock lck(read_mtx_, write_mtx_);
  for (auto& [key, old_entry] : entries_) {
    const auto& cur_fname = GetFnameFromEntry(old_entry);
    auto iter = old_sealed_files.find(cur_fname);
    // Overwrite old sealed entries with new compacted ones.
    if (iter == old_sealed_files.end()) {
      continue;
    }

    // Check timestamp for own entries, only overwrite in-memory entry when
    // compacted entries fresher.
    auto new_entry = compacted_key_val.at(key);
    if (new_entry.timestamp <= old_entry.ts_nano_sec) {
      continue;
    }

    // Mark old sealed file as stale, so they could be deleted if no other
    // reference count.
    auto* disk_handle = std::get_if<DiskHandle>(&old_entry.handle);
    if (disk_handle != nullptr) {
      disk_handle->handle->MarkStale();
    }

    auto new_handle = new_file_handles.at(cur_fname);
    old_entry = MakeEntry(std::move(new_entry.value), new_entry.timestamp,
                          new_handle, new_entry.offset);
  }

  return ghost::Ok();
}

}  // namespace bitcask
