/*******************************************************************************
*   Deomonstrate the use of the bitarray library using a simple example
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "../src/bitarray.h"

#define NUMBITS     150

int main() {

    int i;
    char* b_str;

    /* Initialize the bitarray */
    bitarray_t b = ba_init(NUMBITS);

    /* print some summary information */
    printf("Bitarray: Number bits:\t%lu\n", ba_number_bits(b));
    printf("Bitarray: Array Size (chars):\t%lu\n", ba_array_size(b));
    printf("Bitarray: Max Possible Bits:\t%lu\n", ba_array_size(b) * 8);

    /* set every 3rd bit */
    for (i = 0; i < NUMBITS; i += 3) {
        ba_set_bit(b, i);
    }
    b_str = ba_to_string(b);
    printf("%s\n", b_str);
    printf("Bitarray: Number Bits Set:\t%lu\n", ba_number_bits_set(b));
    free(b_str);


    /* toggle every 4th bit */
    for (i = 0; i < NUMBITS; i += 4) {
        ba_toggle_bit(b, i);
    }
    b_str = ba_to_string(b);
    printf("%s\n", b_str);
    printf("Bitarray: Number Bits Set:\t%lu\n", ba_number_bits_set(b));
    free(b_str);

    /* clear every 5th bit */
    for (i = 0; i < NUMBITS; i += 5) {
        ba_clear_bit(b, i);
    }
    b_str = ba_to_string(b);
    printf("%s\n", b_str);
    printf("Bitarray: Number Bits Set:\t%lu\n", ba_number_bits_set(b));
    free(b_str);

    /* reset bit array */
    ba_reset(b);
    b_str = ba_to_string(b);
    printf("%s\n", b_str);
    printf("Bitarray: Number Bits Set:\t%lu\n", ba_number_bits_set(b));
    free(b_str);

    /* free the bitarray when we are done */
    ba_free(b);

    return 0;
}
