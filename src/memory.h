#ifndef BARRUST_MEMORY_H__
#define BARRUST_MEMORY_H__

#include <stdlib.h>
#include <stdbool.h>

bool __safealloc_alloc_n (void *ptrptr, size_t _size, size_t count);
bool __safealloc_alloc_uninitialized_n(void* ptr2ptr, size_t _size, size_t count);
bool __safealloc_realloc_n (void *ptrptr, size_t _size, size_t count);

/* Allocate an object and zero out the memory */
#define ALLOC(ptr)                          __safealloc_alloc_n(&(ptr), sizeof (*(ptr)), 1)

/* Allocate an array of `count` elements and zero out the memory */
#define ALLOC_N(ptr, count)                 __safealloc_alloc_n(&(ptr), sizeof (*(ptr)), (count))

/* Allocate an array of `count` elements and leave the memory uninitialized */
#define ALLOC_N_UNINITIALIZED(ptr, count)   __safealloc_alloc_uninitialized_n(&(ptr), sizeof (*(ptr)), (count))

/* Reallocate an array to `count` elements */
#define REALLOC_N(ptr, count)               __safealloc_realloc_n(&(ptr), sizeof (*(ptr)), (count))


#define FREE(ptr) do {                      \
    free (ptr);                             \
    (ptr) = NULL;                           \
} while (0)



#endif
