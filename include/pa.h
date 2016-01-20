#pragma once

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// PLIABLE ARRAY (also known as a stretchy buffer)

#define pa_free(a) ((a) ? free(pa___raw(a)), 0 : 0)
#define pa_push(a, v) (pa___maybegrow(a, 1), (a)[pa___n(a)++] = (v))
#define pa_count(a) ((a) ? pa___n(a) : 0)
#define pa_add(a, n) \
    (pa___maybegrow(a, n), pa___n(a) += (n), & (a)[pa___n(a) - (n)])
#define pa_last(a) ((a)[pa___n(a) - 1])
#define pa_end(a) (a + pa_count(a))

#define pa___raw(a) ((int*) (a) -2)
#define pa___m(a) pa___raw(a)[0]
#define pa___n(a) pa___raw(a)[1]
#define pa___needgrow(a, n) ((a) == 0 || pa___n(a) + (n) >= pa___m(a))
#define pa___maybegrow(a, n) (pa___needgrow(a, (n)) ? pa___grow(a, n) : 0)
#define pa___grow(a, n) ((a) = pa___growf((a), (n), sizeof(*(a))))

#define pa_count2(a) (pa_count(a) / 2)
#define pa_count3(a) (pa_count(a) / 3)

#define pa_push2(a, x, y) \
    pa_push(a, x);           \
    pa_push(a, y);           \

#define pa_push3(a, x, y, z) \
    pa_push(a, x);           \
    pa_push(a, y);           \
    pa_push(a, z)

#define pa_remove3(a, i) \
    memmove(a + i * 3, a + i * 3 + 3, \
        sizeof(*(a)) * (pa_count(a) - i * 3 - 3)); \
    pa___n(a) -= 3

static void* pa___growf(void* arr, int increment, int itemsize)
{
    int dbl_cur = arr ? 2 * pa___m(arr) : 0;
    int min_needed = pa_count(arr) + increment;
    int m = dbl_cur > min_needed ? dbl_cur : min_needed;
    int* p =
        (int*) realloc(arr ? pa___raw(arr) : 0, itemsize * m + sizeof(int) * 2);
    if (p) {
        if (!arr) {
            p[1] = 0;
        }
        p[0] = m;
        return p + 2;
    } else {
        return (void*) (2 * sizeof(int));
    }
}

#ifdef __cplusplus
}
#endif
