#include <assert.h>

// This takes two human-readable strings: the key and the metadata.
// The key should not have variation because it is used as a grouping
// key in systems like Sentry.  The metadata can have unique data.

#define par_verify(expr, exception_key, exception_metadata)    \
    if (!expr) {                                               \
        printf("%s: %s\n", exception_key, exception_metadata); \
    }                                                          \
    assert(expr);
