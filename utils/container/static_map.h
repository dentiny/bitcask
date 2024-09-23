// A compile-time static map, which can be used as a drop-in replacement for
// std::unordered_map and absl::flat_hash_map.

#pragma once

#include <array>
#include <cstdlib>
#include <functional>
#include <utility>

#include "base/logging.h"
#include "utils/container/array.h"

namespace utils::container {

template <typename Key,
          typename Value,
          std::size_t N,
          typename KeyEq = std::equal_to<>
         >
class StaticMap {
 public:
  // Container.
  using size_type = std::size_t;
  using key_type = Key;
  using mapped_type = std::pair<Key, Value>;
  using key_compare = KeyEq;
  using container_type = std::array<mapped_type, N>;

  // Iterator.
  using iterator = typename container_type::const_iterator;
  using const_iterator = iterator;

  using reverse_iterator = typename container_type::const_reverse_iterator;
  using const_reverse_iterator = reverse_iterator;

  // Iterator traits.
  using iterator_traits = std::iterator_traits<iterator>;
  using difference_type = typename iterator_traits::difference_type;
  using reference = typename iterator_traits::reference;
  using pointer = typename iterator_traits::pointer;
  using value_type = typename iterator_traits::value_type;
  using iterator_category = typename iterator_traits::iterator_category;

  constexpr StaticMap(container_type data, KeyEq key_cmp)
      : data_(std::move(data)), key_cmp_(std::move(key_cmp)) {
    for (int ii = 0; ii < N; ++ii) {
      for (int jj = ii + 1; jj < N; ++jj) {
        if (key_cmp_(data_[ii].first, data_[jj].first)) {
          throw std::domain_error("Doesn't allow duplicate keys");
        }
      }
    }
  }

  constexpr size_type size() const noexcept {
    return data_.size();
  }

  constexpr size_type max_size() const noexcept {
    return data_.max_size();
  }

  constexpr bool empty() const noexcept { return data_.empty(); }

  constexpr const_iterator begin() const noexcept {
    return data_.cbegin();
  }
  constexpr const_iterator end() const noexcept {
    return data_.cend();
  }

  constexpr const_iterator cbegin() const noexcept {
    return data_.cbegin();
  }
  constexpr const_iterator cend() const noexcept {
    return data_.cend();
  }

  constexpr const_iterator rbegin() const noexcept {
    return data_.crbegin();
  }
  constexpr const_iterator rend() const noexcept {
    return data_.crend();
  }

  constexpr const_reverse_iterator crbegin() const noexcept {
    return data_.crbegin();
  }
  constexpr const_reverse_iterator crend() const noexcept {
    return data_.crend();
  }

  template <typename KeyLike>
  constexpr const_iterator find(
      const KeyLike& key) const noexcept {
    for (auto iter = cbegin(); iter != cend(); ++iter) {
      if (key_cmp_(iter->first, key)) {
        return iter;
      }
    }
    return data_.cend();
  }

  template <typename KeyLike>
  constexpr bool contains(const KeyLike& key) const noexcept {
    return find(key) != cend();
  }
  template <typename KeyLike>
  constexpr size_type count(const KeyLike& key) const noexcept {
    return contains(key) ? 1 : 0;
  }

 private:
  const container_type data_;
  [[no_unique_address]] const KeyEq key_cmp_;
};

template <typename Key, typename Value, typename KeyEq = std::equal_to<>,
          std::size_t N>
[[nodiscard]] constexpr auto MakeStaticMap(
    // NOLINTNEXTLINE
    typename StaticMap<Key, Value, N, KeyEq>::value_type(&&data)[N],
    KeyEq&& key_cmp = {}) {
  return StaticMap<Key, Value, N, KeyEq>(to_array(std::move(data)),
                                          std::forward<KeyEq>(key_cmp));
}

}  // namespace utils::container
