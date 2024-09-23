#pragma once

#include <gmock/gmock.h>

#include <string>

#include "base/file/base_read_handle.h"
#include "base/file/base_write_handle.h"

namespace base::file {

class MockReadHandle : public BaseReadHandle {
 public:
  MockReadHandle() = default;
  ~MockReadHandle() override = default;

  MOCK_METHOD(ghost::Status, Close, (), (override));
  MOCK_METHOD(ghost::Status, Read, (absl::Nonnull<MemoryView*> mem_view,
                                    uint64_t bytes_to_read), (override));
  MOCK_METHOD(ghost::Status, PRead, (absl::Nonnull<MemoryView*> mem_view,
                                     uint64_t bytes_to_read, uint64_t pos),
                                     (override));
  MOCK_METHOD(ghost::StatusOr<std::string>, CompleteRead, (), (override));
};

class MockWriteHandle : public BaseWriteHandle {
 public:
  MockWriteHandle() = default;
  ~MockWriteHandle() override = default;

  MOCK_METHOD(ghost::Status, Close, (), (override));
  MOCK_METHOD(ghost::Status, Flush, (), (override));
  MOCK_METHOD(ghost::Status, Write, (absl::Nonnull<char*> buffer,
                                     uint64_t bytes_to_write), (override));
};

}  // namespace base::file
