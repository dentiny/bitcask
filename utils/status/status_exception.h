// Define exception class for ghost::Status.

#pragma once

#include <stdexcept>
#include <string>
#include <utility>

#include "utils/status/status.h"

namespace ghost {

class StatusException : public std::runtime_error {
 public:
  explicit StatusException(Status status)
    : std::runtime_error(status.DebugString()),
      status_(std::move(status)) {}

  StatusException(StatusCode code, SourceLocation loc, std::string message)
    : StatusException{Status{code, loc, std::move(message)}} {}

  const Status& status() const { return status_; }

  template <typename T>
  StatusException& operator<<(T&& msg) {
    status_.operator<<(std::forward<T>(msg));
    return *this;
  }

 private:
  Status status_;
  const std::string error_status_str_;
};

}  // namespace ghost
