/*******************************************************************************
*   Demonstrate that the use of a linked list as a stack works the same
*   since this example is identical to the linked list version.
*
*   Use -v to include more debugging information
*******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "../src/stack.h"

typedef struct point {
    int x;
    int y;
    int z;
} _point;


#define NUM_ELEMENTS   9000


int main(int argc, char const *argv[]) {
    bool verbose = false;

    if (argc == 2 && strcmp(argv[1], "-v") == 0) {
        verbose = true;
    }

    time_t t;
    srand((unsigned) time(&t));

    stack_t stk = stk_init();

    /* build out a list of points and add them to the stack*/
    int i;
    for (i = 0; i < NUM_ELEMENTS; ++i) {
        _point* p = malloc(sizeof(_point));
        p->x = i;
        p->y = rand() % 50;
        p->z = i + 1;
        stk_push(stk, p);  /* for a stack, everything must be up front */
    }

    /* traverse the list and make sure that each point's x is less than the previous */
    int cnt = 0;
    int prev = NUM_ELEMENTS + 1;
    stack_node* n;
    stk_traverse(stk, n) { /* this is a macro to simplify n = n->next type loops */
        _point* q = (_point*) n->data;
        assert(prev > q->x);
        prev = q->x;
    }

    /* for a stack, we need to "pop" or remove the first element each time */
    for (i = 1; i <= NUM_ELEMENTS; ++i) {
        _point* w = (_point*) stk_pop(stk);
        assert(NUM_ELEMENTS - i == w->x);
        assert((unsigned int)(NUM_ELEMENTS - i) == stk_num_elements(stk));
        if (verbose) {
            printf("point: %d\tx: %d\ty: %d\tz: %d\n", cnt++, w->x, w->y, w->z);
        }
        free(w); /* it was pop'd but not yet free'd */
    }

    assert(0 == stk_num_elements(stk));

    stk_free_alt(stk, true);
    printf("Completed successfully!\n");
    return 0;
}
