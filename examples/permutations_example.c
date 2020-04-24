
#include <inttypes.h>
#include <stdio.h>

#include "../src/permutations.h"


int main() {

    permutations_t p = perm_init(5, "0123456789");

    printf("%s\n", perm_to_string(p));
    for (uint64_t i = 0; i < 9999; ++i) {
        perm_add(p, 4);
        printf("%s\n", perm_to_string(p));
    }

    printf("----------------------------------------------------------------\n");

    for (uint64_t i = 0; i < 23; ++i) {
        perm_sub(p, 3);
        printf("%s\n", perm_to_string(p));
    }

    perm_free(p);
}
