#pragma once

#include <type_traits>
#include <utility>

#include "base/logging.h"
#include "ghost/visitor/internal/visitor.h"

namespace ghost {

template <typename V, typename... Args>
struct visit_result {
  using type = decltype(std::declval<V>()(std::declval<Args>()...));
};

template <typename V, typename F, typename... Args>
using maybe_visit_result =
    visit_result<VisitImpl, V, F, specialize, Args...>;

template <typename V, typename F, typename... Args>
using maybe_visit_result_t = typename maybe_visit_result<V, F, Args...>::type;

template <typename V, typename F, typename... Args>
maybe_visit_result_t<V, F, Args...> maybe_visit(V&& visitor, F&& fallback,
                                                Args&&... args) {
  return VisitImpl {}(std::forward<V>(visitor), std::forward<F>(fallback),
                           specialize {}, std::forward<Args>(args)...);
}

template <typename V, typename F, typename... Args>
using maybe_invoke_result_t =
    std::invoke_result_t<VisitImpl, V, F, specialize, Args...>;

// A visitor that forwards calls to the first applicable visitor in the given
// visitor.
// @param VT: Ordered types of visitors to use.
template <typename... VT>
class OrderedVisitor;

// Single visitor case.
template <typename V>
class OrderedVisitor<V> {
 public:
  OrderedVisitor() = default;
  OrderedVisitor(const OrderedVisitor&) = default;
  OrderedVisitor& operator=(const OrderedVisitor&) = default;
  OrderedVisitor(OrderedVisitor&&) = default;
  OrderedVisitor& operator=(OrderedVisitor&&) = default;

  explicit OrderedVisitor(const V& visitor) : visitor_(visitor) {}
  explicit OrderedVisitor(V&& visitor) : visitor_(std::move(visitor)) {}

  // Non-const version.
  template <typename... Args>
  std::invoke_result_t<V&, Args&&...> operator()(Args&&... args) {
    return visitor_(std::forward<Args>(args)...);
  }

  // Const version.
  template <typename... Args>
  std::invoke_result_t<const V&, Args&&...> operator()(Args&&... args) const {
    return visitor_(std::forward<Args>(args)...);
  }

 private:
  V visitor_;
};

// Multiple visitors, try head and fallback on tail if needed.
template <typename Head, typename... Tail>
class OrderedVisitor<Head, Tail...> {
  using V = Head;
  using F = OrderedVisitor<Tail...>;

 public:
  OrderedVisitor() = default;
  OrderedVisitor(const OrderedVisitor&) = default;
  OrderedVisitor& operator=(const OrderedVisitor&) = default;
  OrderedVisitor(OrderedVisitor&&) = default;
  OrderedVisitor& operator=(OrderedVisitor&&) = default;

  template <typename T, typename... Args>
  explicit OrderedVisitor(T&& visitor, Args&&... args)
      : visitor_(std::forward<T>(visitor)),
        fallback_(std::forward<Args>(args)...) {}

  // Non-const overload.
  template <typename... Args>
  maybe_invoke_result_t<V&, F&, Args...> operator()(Args&&... args) {
    return maybe_visit(visitor_, fallback_, std::forward<Args>(args)...);
  }

  // Const overload.
  template <typename... Args>
  maybe_invoke_result_t<const V&, const F&, Args...> operator()(
      Args&&... args) const {
    return maybe_visit(visitor_, fallback_, std::forward<Args>(args)...);
  }

 private:
  V visitor_;
  F fallback_;
};

template <class... Ts>
OrderedVisitor(Ts...) -> OrderedVisitor<std::remove_cvref_t<Ts>...>;

}  // namespace ghost
