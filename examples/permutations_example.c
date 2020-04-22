
#include <inttypes.h>
#include <stdio.h>

#include "../src/permutations.h"

// static void print_permutation(const char* cur, unsigned int len);





int main() {

    permutations_t p = perm_init(10, "0123456789");

    printf("%s\n", perm_to_string(p));
    for (uint64_t i = 0; i < 100; ++i) {
        perm_inc(p);
        printf("%s\n", perm_to_string(p));
        // print_permutation(perm_to_string(p), 10);
    }

    perm_free(p);

}



// static void print_permutation(const short* cur, unsigned int len) {
//     unsigned int i;
//     for (i = 0; i < len; ++i)
//         printf("%d", cur[i]);
//     printf("\n");
// }
