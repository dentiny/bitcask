// Utils to optimize branch prediction.

#pragma once

#include "absl/base/optimization.h"

namespace ghost {

// Instruct the compiler to predict that `value` will be true with high
// probability.
inline constexpr bool Likely(bool value) { return ABSL_PREDICT_TRUE(value); }

// Instruct the compiler to predict that `value` will be false with high
// probability.
inline constexpr bool Unlikely(bool value) { return ABSL_PREDICT_FALSE(value); }

}  // namespace ghost
