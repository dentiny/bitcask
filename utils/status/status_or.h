#pragma once

#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <utility>

#include "absl/base/attributes.h"
#include "utils/status/status.h"
#include "utils/status/status_code.h"

namespace ghost {

template <typename T>
class StatusOr {
 public:
  StatusOr() noexcept = default;
  // NOLINTNEXTLINE(runtime/explicit)
  StatusOr(Status status) : status_(std::move(status)) {}
  // NOLINTNEXTLINE(runtime/explicit)
  StatusOr(const T& data) : status_(StatusCode::kOk), data_(data) {}
  // NOLINTNEXTLINE(runtime/explicit)
  StatusOr(T&& data) : status_(StatusCode::kOk), data_(std::move(data)) {}

  template <typename... Args>
  explicit StatusOr(std::in_place_t ip, Args&&... args)
    : data_(std::forward<Args>(args)...) {}

  template <typename U>
  StatusOr(const StatusOr<U>& status_or) {
    if (status_or.ok()) {
      data_ = status_or.value();
      status_ = ghost::Ok();
    } else {
      status_ = status_or.status();
    }
  }

  template <typename U>
  StatusOr(StatusOr<U>&& status_or) {
    if (status_or.ok()) {
      data_ = std::move(status_or).value();
      status_ = ghost::Ok();
    } else {
      status_ = std::move(status_or).status();
    }
  }

  StatusOr(const StatusOr&) = default;
  StatusOr& operator=(const StatusOr&) = default;

  StatusOr(StatusOr&&) noexcept(std::is_nothrow_move_constructible_v<T>)
    = default;
  StatusOr& operator=(StatusOr&&) noexcept(std::is_nothrow_move_assignable_v<T>)
    = default;

  ~StatusOr() noexcept(std::is_nothrow_destructible_v<T>) = default;

  bool ok() const { return status_.ok(); }
  bool has_value() const { return ok(); }
  explicit operator bool() const { return ok(); }

  template <typename U>
  T value_or(U&& u) { return ok() ? get() : T{std::forward<U>(u)}; }

  ABSL_MUST_USE_RESULT StatusCode code() const { return status_.code(); }

  std::string_view message() const { return status_.message(); }

  ABSL_MUST_USE_RESULT const Status& status() const& { return status_; }
  ABSL_MUST_USE_RESULT Status status() && {
    Status new_status = std::move(status_);
    return new_status;
  }

  T& operator*() & { return get(); }
  const T& operator*() const& { return get(); }
  T&& operator*() && { return std::move(get()); }

  T* operator->() & { return &data_; }
  T* operator->() const& { return &data_; }

  // @throws if `has_value` is false.
  T& value() &;
  const T& value() const&;
  T&& value() &&;

 private:
  T& get() { return data_; }
  const T& get() const { return data_; }

  void CheckHasValue() const {
    if (!has_value()) {
      throw std::runtime_error(status_.DebugString());
    }
  }

  Status status_;
  T data_;
};

template <typename T>
T& StatusOr<T>::value() & {
  CheckHasValue();
  return get();
}

template <typename T>
const T& StatusOr<T>::value() const& {
  CheckHasValue();
  return get();
}

template <typename T>
T&& StatusOr<T>::value() && {
  CheckHasValue();
  auto& val = get();
  return std::move(val);
}

template <typename T>
bool operator==(const StatusOr<T>& lhs, const StatusOr<T>& rhs) {
  bool lhs_ok = lhs.ok();
  bool rhs_ok = rhs.ok();
  if (lhs_ok && rhs_ok) {
    return *lhs == *rhs;
  }
  if (!lhs_ok && !rhs_ok) {
    return lhs.status().code() == rhs.status().code();
  }
  return false;
}

}  // namespace ghost

