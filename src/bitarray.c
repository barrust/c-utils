
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bitarray.h"

#define CHECK_BIT(A, k)       (A[((k) / 8)] &  (1 << ((k) % 8)))
#define SET_BIT(A,k)          (A[((k) / 8)] |=  (1 << ((k) % 8)))
#define CLEAR_BIT(A,k)        (A[((k) / 8)] &= ~(1 << ((k) % 8)))


typedef struct __bitarray {
    unsigned char* arr;
    size_t num_bits;
    size_t num_chars;
} __bitarray;


bitarray_t ba_init(size_t bits) {
    bitarray_t ba = calloc(1, sizeof(bitarray));
    ba->num_bits = bits;
    size_t num_chars = ceil(bits / 8.0);
    ba->num_chars = num_chars;
    ba->arr = calloc(num_chars, sizeof(unsigned char));

    return ba;
}


size_t ba_array_size(bitarray_t ba) {
    return ba->num_chars;
}


size_t ba_number_bits(bitarray_t ba) {
    return ba->num_bits;
}


const unsigned char* const ba_get_bitarray(bitarray_t ba) {
    return ba->arr;
}


void ba_free(bitarray_t ba) {
    free(ba->arr);
    ba->num_bits = 0;
    ba->num_chars = 0;
    free(ba);
    ba = NULL;
}


int ba_set_bit(bitarray_t ba, size_t bit) {
    if (bit >= ba->num_bits)
        return BITARRAY_INDEX_ERROR;
    SET_BIT(ba->arr, bit);
    return BIT_SET;  // no reason this should ever fail...
}


int ba_check_bit(bitarray* ba, size_t bit) {
    if (bit >= ba->num_bits)
        return BITARRAY_INDEX_ERROR;
    return (CHECK_BIT(ba->arr, bit) != 0) ? BIT_SET : BIT_NOT_SET;
}


int ba_clear_bit(bitarray* ba, size_t bit) {
    if (bit >= ba->num_bits)
        return BITARRAY_INDEX_ERROR;
    CLEAR_BIT(ba->arr, bit);
    return BIT_NOT_SET;
}


int ba_reset_bitarray(bitarray* ba) {
    for (size_t i = 0; i < ba->num_chars; i++) {
        ba->arr[i] = 0;
    }
    return BIT_SET;
}
