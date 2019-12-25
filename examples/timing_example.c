/*******************************************************************************
*   Example of using the timing library to see which version of a ceil function
*   is comparable to the math.h ceil function in speed, assuming -O3 used
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../src/timing.h"


#define ceiling_0(n,d)      ((n%d) == 0 ? (n/d) : (n/d) + 1)
#define ceiling_1(n,d)      ((n/d) + (n%d != 0))
#define ceiling_2(n,d)      ((n/d) + (n%d > 0))

#define ITERATIONS 1000000
#define ARBITRARY_THRESHOLD 15  /* This is just to do something with the result... */


int main() {
    /* set up the tests */
    int* nums = calloc(ITERATIONS, sizeof(int));
    int* denoms = calloc(ITERATIONS, sizeof(int));

    int i;
    for (i = 0; i < ITERATIONS; i++) {
        nums[i] = rand();
        denoms[i] = rand() % ITERATIONS + 1; /* ensure no division by 0 */
    }

    unsigned int a = 0;
    Timing t;
    /* test standard ceil function */
    timing_start(&t);
    for (i = 0; i < ITERATIONS; i++) {
        double res = ceil((double) nums[i] / (double) denoms[i]);
        if (res > ARBITRARY_THRESHOLD)
            ++a;
    }
    timing_end(&t);
    printf("ceils:   \t%d iterations\t%.8f seconds\t%d above threshold\n", ITERATIONS, t.timing_double, a);

    /* test ceiling 0 function */
    timing_start(&t);
    a = 0;
    for (i = 0; i < ITERATIONS; i++) {
        int res = ceiling_0(nums[i], denoms[i]);
        if (res > ARBITRARY_THRESHOLD)
            ++a;
    }
    timing_end(&t);
    printf("ceiling 0:\t%d iterations\t%.8f seconds\t%d above threshold\n", ITERATIONS, t.timing_double, a);

    /* test ceiling 1 function */
    timing_start(&t);
    a = 0;
    for (i = 0; i < ITERATIONS; i++) {
        int res = ceiling_1(nums[i], denoms[i]);
        if (res > ARBITRARY_THRESHOLD)
            ++a;
    }
    timing_end(&t);
    printf("ceiling 1:\t%d iterations\t%.8f seconds\t%d above threshold\n", ITERATIONS, t.timing_double, a);

    /* test ceiling 1 function */
    timing_start(&t);
    a = 0;
    for (i = 0; i < ITERATIONS; i++) {
        int res = ceiling_2(nums[i], denoms[i]);
        if (res > ARBITRARY_THRESHOLD)
            ++a;
    }
    timing_end(&t);
    printf("ceiling 2:\t%d iterations\t%.8f seconds\t%d above threshold\n", ITERATIONS, t.timing_double, a);
    return 0;
}
