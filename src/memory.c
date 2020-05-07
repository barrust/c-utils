

#include <stddef.h>
#include <errno.h>

#include "memory.h"

#define safe_alloc_oversized(n, s)  ((size_t) (sizeof (ptrdiff_t) <= sizeof (size_t) ? -1 : -2) / (s) < (n))

#define test_results(ptr)           (((*(void**) ptr) != NULL) ? true : false)

bool __safealloc_alloc_n(void* ptr2ptr, size_t _size, size_t count) {
    if (_size == 0 || count == 0) {
        *(void**) ptr2ptr = NULL;
        return true;
    }

    if (safe_alloc_oversized (count, _size)) {
        errno = ENOMEM;
        return false;
    }

    *(void**) ptr2ptr = calloc(count, _size);

    return test_results(ptr2ptr);
}


bool __safealloc_alloc_uninitialized_n(void* ptr2ptr, size_t _size, size_t count) {
    if (_size == 0 || count == 0) {
        *(void**) ptr2ptr = NULL;
        return true;
    }

    if (safe_alloc_oversized (count, _size)) {
        errno = ENOMEM;
        return false;
    }

    *(void**) ptr2ptr = malloc(count * _size);

    return test_results(ptr2ptr);
}

bool __safealloc_realloc_n(void *ptr2ptr, size_t size, size_t count) {
    void *tmp;
    if (size == 0 || count == 0) {
        free(*(void**) ptr2ptr);
        *(void**) ptr2ptr = NULL;
        return true;
    }
    if (safe_alloc_oversized (count, size)) {
        errno = ENOMEM;
        return false;
    }
    tmp = realloc(*(void**) ptr2ptr, size * count);
    if (!tmp)
        return false;
    *(void**) ptr2ptr = tmp;
    return true;
}
