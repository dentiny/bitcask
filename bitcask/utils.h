// Utils for bitcask.

#pragma once

#include <string>

namespace bitcask {

// Get a random filename under the given [folder]
std::string GetRandomFilename(const std::string& folder);

}  // namespace bitcask
