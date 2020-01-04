/*******************************************************************************
*   Demonstrate some of the uses of the linked list datastructure by
*   adding points as a stack! We will make a point struct, to show it being
*   used with a non-standard datatype.
*
*   NOTE: One can make a stake using the data structure by making a macro for
*   pop and push to ensure that one always pushes to the front (0) and pops
*   from the front. A stack (LIFO) is best implemented using a singly linked
*   list that always does all work from the head node.
*
*   Use -v to include more debugging information
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "../src/llist.h"

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

    llist_t l = ll_init();

    /* build out a list of points and add them to the stack*/
    int i;
    for (i = 0; i < NUM_ELEMENTS; i++) {
        _point* p = malloc(sizeof(_point));
        p->x = i;
        p->y = rand() % 50;
        p->z = i + 1;
        ll_insert(l, p, 0);  /* for a stack, everything must be up front */
    }

    /* traverse the list and make sure that each point's x is less than the previous */
    int cnt = 0;
    int prev = NUM_ELEMENTS + 1;
    ll_node* n;
    ll_traverse(l, n) { /* this is a macro to simplify n = n->next type loops */
        _point* q = (_point*) n->data;
        assert(prev > q->x);
        prev = q->x;
    }

    /* for a stack, we need to "pop" or remove the first element each time */
    for (i = 1; i <= NUM_ELEMENTS; i++) {
        _point* w = (_point*) ll_remove(l, 0);
        assert(NUM_ELEMENTS - i == w->x);
        assert((unsigned int)(NUM_ELEMENTS - i) == ll_num_elements(l));
        if (verbose) {
            printf("point: %d\tx: %d\ty: %d\tz: %d\n", cnt++, w->x, w->y, w->z);
        }
        free(w); /* it was pop'd but not yet free'd */
    }

    assert(0 == ll_num_elements(l));

    ll_free_alt(l, true);
    printf("Completed successfully!\n");
    return 0;
}
