#include "bitcask/file_handle_wrapper.h"

#include <utility>

#include "base/file/file.h"
#include "utils/status/status_macros.h"

namespace bitcask {

FileHandleWrapper::FileHandleWrapper(
      std::unique_ptr<base::file::BaseFileHandle> handle)
  : handle_(std::move(handle)) {}

FileHandleWrapper::~FileHandleWrapper() {
  if (!delete_after_unref_) {
    return;
  }
  const auto& fname = handle_->Name();
  GHOST_THROW_IF_ERROR(base::file::Delete(fname));
}

uint64_t FileHandleWrapper::GetFileSize() const {
  return handle_->GetFileSize();
}

ghost::Status FileHandleWrapper::PRead(
    void* buffer, uint64_t bytes_to_read, uint64_t offset) {
  base::MemoryView mem_view{static_cast<char*>(buffer), bytes_to_read};
  return PRead(&mem_view, bytes_to_read, offset);
}

ghost::Status FileHandleWrapper::PRead(
    absl::Nonnull<base::MemoryView*> mem_view,
    uint64_t bytes_to_read, uint64_t offset) {
  return handle_->PRead(mem_view, bytes_to_read, offset);
}

ghost::Status FileHandleWrapper::Write(std::string_view content) {
  return handle_->Write(content);
}

ghost::Status FileHandleWrapper::Flush() { return handle_->Flush(); }

ghost::Status FileHandleWrapper::Close() { return handle_->Close(); }

ghost::StatusOr<FileHandleWrapperPtr> CreateFileHandleWithRefCount(
    const std::string& fname) {
  GHOST_ASSIGN_OR_RETURN(auto handle,
      base::file::Open(fname, base::file::FileOpenMode::kAppendRead));
  auto wrapper = std::make_shared<FileHandleWrapper>(std::move(handle));
  return wrapper;
}

}  // namespace bitcask
