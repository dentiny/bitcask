// Human-readable type names.
// https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname

#pragma once

#include <string_view>

namespace ghost {

namespace internal {

template <typename T>
constexpr std::string_view GetFunctionName() {
#if defined(__clang__) || defined(__GNUC__)
  return std::string_view{__PRETTY_FUNCTION__};
#elif defined(__MSC_VER)
  return std::string_view{__FUNCSIG__};
#else
#error Invalid compiler
#endif
}

struct TypeNameInfo {
  static constexpr auto sentinel_function = GetFunctionName<double>();
  static constexpr auto prefix_offset = sentinel_function.find("double");
  // 6 characters for "double"
  static constexpr auto suffix_offset =
      sentinel_function.size() - sentinel_function.rfind("double") - 6;
};

}  // namespace internal

template <typename T>
constexpr std::string_view GetTypeName() {
  constexpr auto function = internal::GetFunctionName<T>();
  constexpr auto start = internal::TypeNameInfo::prefix_offset;
  constexpr auto end = function.size() - internal::TypeNameInfo::suffix_offset;
  return function.substr(start, end - start);
}

}  // namespace ghost
