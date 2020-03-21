#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/minunit.h"
#include "../src/dllist.h"


/* private test function - uncomment if needed! */
/*
static void printlist(dllist_t l) {
    int i = 0;
    dll_node* n = dll_first_node(l);
    printf("\n");
    while (n != NULL) {
        int* val = (int*) n->data;
        printf("idx: %d\tval: %d\n", i++, *val);
        n = n->next;
    }
}
*/


dllist_t l;

void test_setup(void) {
    l = dll_init();
}

void test_teardown(void) {
    dll_free_alt(l, true);
    l = NULL;
}


/*******************************************************************************
*   Test the setup
*******************************************************************************/
MU_TEST(test_default_setup) {
    mu_assert_int_eq(0, dll_num_elements(l));
    mu_assert_string_eq(NULL, (void*)dll_first_node(l));
    mu_assert_string_eq(NULL, (void*)dll_last_node(l));
}


/*******************************************************************************
*   Test Free-ing
*******************************************************************************/
MU_TEST(test_freeing) {
    dllist_t a = dll_init();
    mu_assert_int_eq(0, dll_num_elements(l));
    dll_free(a);
}


/*******************************************************************************
*   Test Traversing the list using macros
*******************************************************************************/
MU_TEST(test_traverse) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }
    mu_assert_int_eq(5, dll_num_elements(l));

    int j = 0;
    dll_node* n;
    /* forward traverse */
    dll_traverse(l, n) {
        int* val = (int*)n->data;
        mu_assert_int_eq(j++, *val);
    }

    j = 4;
    /* reverse traverse */
    dll_reverse_traverse(l, n) {
        int* val = (int*)n->data;
        mu_assert_int_eq(j--, *val);
    }
}


/*******************************************************************************
*   Test appending elements
*******************************************************************************/
MU_TEST(test_append) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }

    int j = 0;
    dll_node* n = dll_first_node(l);
    while (n != NULL) {
        int* val = (int*)n->data;
        mu_assert_int_eq(j++, *val);
        n = n->next;
    }

    /* test in reverse */
    j = 4;
    n = dll_last_node(l);
    while (n != NULL) {
        int* val = (int*)n->data;
        mu_assert_int_eq(j--, *val);
        n = n->prev;
    }
}


/*******************************************************************************
*   Test inserting elements
*******************************************************************************/
MU_TEST(test_insert_beginning) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }

    int* q = calloc(1, sizeof(int));
    *q = 15;
    dll_insert(l, q, 0); // insert at the beginning

    dll_node* n = dll_first_node(l);
    mu_assert_int_eq(15, *(int*)n->data);
    mu_assert_int_eq(6, dll_num_elements(l));
    /* ensure pointing correct */
    mu_assert_int_eq(0, *(int*)n->next->data);
    mu_assert_int_eq(15, *(int*)n->next->prev->data);
    mu_assert_string_eq(NULL, (void*)n->prev);

    int w = 0;
    n = n->next;
    while(n != NULL) {
        int* val = (int*)n->data;
        mu_assert_int_eq(w++, *val);
        n = n->next;
    }
}

MU_TEST(test_insert_end) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }
    int* q = calloc(1, sizeof(int));
    *q = 15;
    dll_insert(l, q, 5); /* insert at the end of the list (like append) */
    dll_node* n = dll_last_node(l);
    mu_assert_int_eq(15, *(int*)n->data);
    mu_assert_string_eq(NULL, (void*)n->next);
}

MU_TEST(test_insert_first_half) {
    int i;
    for (i = 0; i < 10; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }

    int* q = calloc(1, sizeof(int));
    *q = 15;
    dll_insert(l, q, 2); /* insert at the first half */

    dll_node* n = dll_first_node(l);
    for (i = 0; i < 2; i++)
        n = n->next;
    mu_assert_int_eq(15, *(int*)n->data);
    mu_assert_int_eq(1, *(int*)n->prev->data);
    mu_assert_int_eq(2, *(int*)n->next->data);

    /* ensure that everything points where it is needed */
    mu_assert_int_eq(15, *(int*)n->prev->next->data);
    mu_assert_int_eq(15, *(int*)n->next->prev->data);
}

MU_TEST(test_insert_second_half) {
    int i;
    for (i = 0; i < 10; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }

    int* q = calloc(1, sizeof(int));
    *q = 15;
    dll_insert(l, q, 8); /* insert at the second half */

    dll_node* n = dll_first_node(l);
    for (i = 0; i < 8; i++)
        n = n->next;
    mu_assert_int_eq(15, *(int*)n->data);
    mu_assert_int_eq(7, *(int*)n->prev->data);
    mu_assert_int_eq(8, *(int*)n->next->data);

    /* ensure that everything points where it is needed */
    mu_assert_int_eq(15, *(int*)n->prev->next->data);
    mu_assert_int_eq(15, *(int*)n->next->prev->data);
}

MU_TEST(test_insert_negative) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }

    int* q = calloc(1, sizeof(int));
    *q = 15;
    dll_insert(l, q, -1); /* insert in the second half, using a negatvie index */

    dll_node* n = dll_last_node(l);
    mu_assert_int_eq(15, *(int*)n->data);
    mu_assert_string_eq(NULL, (void*)n->next);
}

MU_TEST(test_insert_negative_mid) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }

    int* q = calloc(1, sizeof(int));
    *q = 15;
    dll_insert(l, q, -2); /* insert in the second half, using a negatvie index */

    dll_node* n = dll_last_node(l)->prev;
    mu_assert_int_eq(15, *(int*)n->data);
    mu_assert_string_eq((void*)dll_last_node(l), (void*)n->next);
}

MU_TEST(test_insert_error) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }

    int* q = calloc(1, sizeof(int));
    *q = 15;
    int res = dll_insert(l, q, -6);
    free(q);
    mu_assert_int_eq(DLL_FAILURE, res);
}


/*******************************************************************************
*   Test removing elements
*******************************************************************************/
MU_TEST(test_remove_last_element) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }

    int* val = (int*)dll_remove(l, 4);  /* 0 based */
    mu_assert_int_eq(4, *val);
    mu_assert_int_eq(4, dll_num_elements(l));
    free(val);

    int w = 0;
    dll_node* n = dll_first_node(l);
    while (n != NULL) {
        val = (int*)n->data;
        mu_assert_int_eq(w++, *val);
        n = n->next;
    }

    w = 3;
    n = dll_last_node(l);
    while (n != NULL) {
        val = (int*)n->data;
        mu_assert_int_eq(w--, *val);
        n = n->prev;
    }
}

MU_TEST(test_remove_first_element) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }
    int* val = (int*)dll_remove(l, 0);  /* 0 based */
    mu_assert_int_eq(0, *val);
    mu_assert_int_eq(4, dll_num_elements(l));
    free(val);

    int w = 1;
    dll_node* n = dll_first_node(l);
    while (n != NULL) {
        val = (int*)n->data;
        mu_assert_int_eq(w++, *val);
        n = n->next;
    }

    w = 4;
    n = dll_last_node(l);
    while (n != NULL) {
        val = (int*)n->data;
        mu_assert_int_eq(w--, *val);
        n = n->prev;
    }
}

MU_TEST(test_remove_early_element) {
    int i;
    for (i = 0; i < 10; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }

    int* val = (int*)dll_remove(l, 3);  /* 0 based */
    mu_assert_int_eq(2, *val);
    mu_assert_int_eq(9, dll_num_elements(l));
    free(val);

    int w = 0;
    dll_node* n = dll_first_node(l);
    while (n != NULL) {
        val = (int*)n->data;
        if (w == 2)
            ++w;
        mu_assert_int_eq(w++, *val);
        n = n->next;
    }

    n = dll_last_node(l);
    w = 9;
    while (n != NULL) {
        val = (int*)n->data;
        if (w == 2)
            --w;
        mu_assert_int_eq(w--, *val);
        n = n->prev;
    }
}

MU_TEST(test_remove_late_element) {
    int i;
    for (i = 0; i < 10; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }

    int* val = (int*)dll_remove(l, 8);  /* 0 based */
    mu_assert_int_eq(7, *val);
    mu_assert_int_eq(9, dll_num_elements(l));
    free(val);

    int w = 0;
    dll_node* n = dll_first_node(l);
    while (n != NULL) {
        val = (int*)n->data;
        if (w == 7)
            ++w;
        mu_assert_int_eq(w++, *val);
        n = n->next;
    }

    n = dll_last_node(l);
    w = 9;
    while (n != NULL) {
        val = (int*)n->data;
        if (w == 7)
            --w;
        mu_assert_int_eq(w--, *val);
        n = n->prev;
    }
}

MU_TEST(test_remove_late_element_neg) {
    int i;
    for (i = 0; i < 10; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }
    int* val = (int*)dll_remove(l, -2);
    // printlist(l);
    mu_assert_int_eq(7, *val);
    mu_assert_int_eq(9, dll_num_elements(l));
    free(val);

    int w = 0;
    dll_node* n = dll_first_node(l);
    while (n != NULL) {
        val = (int*)n->data;
        if (w == 7)
            ++w;
        mu_assert_int_eq(w++, *val);
        n = n->next;
    }

    n = dll_last_node(l);
    w = 9;
    while (n != NULL) {
        val = (int*)n->data;
        if (w == 7)
            --w;
        mu_assert_int_eq(w--, *val);
        n = n->prev;
    }
}

MU_TEST(test_remove_last_neg) {
    int i;
    for (i = 0; i < 10; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }
    int* val = (int*)dll_remove(l, -1);
    // printlist(l);
    mu_assert_int_eq(9, *val);
    mu_assert_int_eq(9, dll_num_elements(l));
    free(val);

    int w = 0;
    dll_node* n = dll_first_node(l);
    while (n != NULL) {
        val = (int*)n->data;
        mu_assert_int_eq(w++, *val);
        n = n->next;
    }

    n = dll_last_node(l);
    w = 8;
    while (n != NULL) {
        val = (int*)n->data;
        mu_assert_int_eq(w--, *val);
        n = n->prev;
    }
}

MU_TEST(test_remove_only_elm) {
    int i;
    for (i = 0; i < 1; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }
    int* val = (int*) dll_remove(l, 0);
    mu_assert_int_eq(0, *val);
    mu_assert_int_eq(0, dll_num_elements(l));
    free(val);

    mu_assert_string_eq(NULL, (void*)dll_first_node(l));
    mu_assert_string_eq(NULL, (void*)dll_last_node(l));
}

MU_TEST(test_remove_alt) {
    /* remove alt free's the memory if desired */
    int i;
    for (i = 0; i < 5; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }
    dll_remove_alt(l, 0, true);  /* remember it is 0 based */
}


MU_TEST(test_remove_error_neg) {
    int i;
    for (i = 0; i < 5; i++) {
        int* t = calloc(1, sizeof(int));
        *t = i;
        dll_append(l, t);
    }
    mu_assert_int_eq(5, dll_num_elements(l));

    void* res = dll_remove(l, -5);  /* remember it is 0 based so this is out of bounds */
    mu_assert_string_eq(NULL, res);
}

/*******************************************************************************
*    Test Suite Setup
*******************************************************************************/
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    /* setup */
    MU_RUN_TEST(test_default_setup);

    /* free */
    MU_RUN_TEST(test_freeing);

    /* traverse */
    MU_RUN_TEST(test_traverse);

    /* append */
    MU_RUN_TEST(test_append);

    /* insert */
    MU_RUN_TEST(test_insert_beginning);
    MU_RUN_TEST(test_insert_end);
    MU_RUN_TEST(test_insert_first_half);
    MU_RUN_TEST(test_insert_second_half);
    MU_RUN_TEST(test_insert_negative);
    MU_RUN_TEST(test_insert_negative_mid);
    MU_RUN_TEST(test_insert_error);

    /* remove */
    MU_RUN_TEST(test_remove_first_element);
    MU_RUN_TEST(test_remove_last_element);
    MU_RUN_TEST(test_remove_early_element);
    MU_RUN_TEST(test_remove_late_element);
    MU_RUN_TEST(test_remove_late_element_neg);
    MU_RUN_TEST(test_remove_last_neg);
    MU_RUN_TEST(test_remove_alt);
    MU_RUN_TEST(test_remove_error_neg);
    MU_RUN_TEST(test_remove_only_elm);
}


int main() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Number failed tests: %d\n", minunit_fail);
    return minunit_fail;
}
