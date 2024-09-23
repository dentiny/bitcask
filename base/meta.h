// A few util macros.

#pragma once

// Need to have two macro invocation to allow [x] and [y] to be replaced.
#define __GHOST_CONCAT(x, y) x##y

#define GHOST_CONCAT(x, y) __GHOST_CONCAT(x, y)

// Macros which gets unique variable name.
#define GHOST_UNIQUE_VARIABLE(base) GHOST_CONCAT(base, __LINE__)
