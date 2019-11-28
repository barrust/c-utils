#include <stdlib.h>
#include <stdio.h>
#include "../src/bitarray.h"
// #include "minunit.h"

int main(int argc, char const *argv[]) {

    // bitarray ba;
    // ba_initialize(&ba, 125);
    bitarray_t ba = ba_init(125);

    printf("num_chars: %lu\n", ba_array_size(ba));
    printf("num_bits: %lu\n", ba_number_bits(ba));

    printf("set bit: %d\n", ba_set_bit(ba, 125));
    ba_free(ba);
    return 0;
}
