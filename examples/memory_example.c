
#include <errno.h>
#include <stdio.h>
#include "../src/memory.h"

int main() {
    errno = 0;
    char* s;
    if (ALLOC_N(s, 5000) == false)
        printf("What happened here???\n");
    else
        printf("We allocated 5000 bytes\n");

    FREE(s);
    if (ALLOC_N(s, 100000000000) == false) {
        printf("Failed to allocated  100000000000 bytes\n");
        printf("Errno: %d\n", errno);
    }

    FREE(s);
    return 0;
}
