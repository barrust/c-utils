#include <stdlib.h>
#include <stdbool.h>
#include "../src/graph.h"
#include "minunit.h"


graph_t g;

void test_setup(void) {
    g = g_init();
}

void test_teardown(void) {
    g_free(g);
    g = NULL;
}

/* private functions */
static char* __str_duplicate(const char* s);
static void  __add_verticies(graph_t g, int num);
static void  __add_edge(graph_t g, unsigned int src, unsigned int dest, int val);


/*******************************************************************************
*   Test the setup
*******************************************************************************/
MU_TEST(test_default_setup) {
    mu_assert_int_eq(0, g_num_edges(g));
    mu_assert_int_eq(0, g_num_vertices(g));
}


/*******************************************************************************
*   Test adding and removing verticies
*******************************************************************************/
MU_TEST(test_add_verticies) {
    mu_assert_int_eq(0, g_num_vertices(g));
    vertex_t v = g_vertex_add(g, __str_duplicate("this is a test"));
    mu_assert_int_eq(1, g_num_vertices(g));
    mu_assert_int_eq(0, g_vertex_id(v));
    mu_assert_string_eq("this is a test", g_vertex_metadata(v));

    g_vertex_add(g, __str_duplicate("d3-football"));
    g_vertex_add(g, __str_duplicate("college hoops"));
    mu_assert_int_eq(3, g_num_vertices(g));

    v = g_vertex_get(g, 1);
    mu_assert_int_eq(1, g_vertex_id(v));
    mu_assert_string_eq("d3-football", g_vertex_metadata(v));
}

MU_TEST(test_remove_verticies) {
    mu_assert_int_eq(0, g_num_vertices(g));
    g_vertex_add(g, __str_duplicate("this is a test"));
    g_vertex_add(g, __str_duplicate("d3-football"));
    g_vertex_add(g, __str_duplicate("college hoops"));

    mu_assert_int_eq(3, g_num_vertices(g));

    vertex_t v = g_vertex_remove(g, 0);
    mu_assert_int_eq(0, g_vertex_id(v));
    mu_assert_string_eq("this is a test", g_vertex_metadata(v));
    mu_assert_int_eq(2, g_num_vertices(g));
    g_vertex_free(v);

    v = g_vertex_remove(g, 2);
    mu_assert_int_eq(2, g_vertex_id(v));
    mu_assert_string_eq("college hoops", g_vertex_metadata(v));
    mu_assert_int_eq(1, g_num_vertices(g));
    g_vertex_free(v);

    /* check that something removed is clean! */
    v = g_vertex_remove(g, 0);
    mu_assert_string_eq(NULL, (char*)v);
    mu_assert_int_eq(1, g_num_vertices(g));  /* it shouldn't change the num verticies */
}

MU_TEST(test_verticies_growth) {
    __add_verticies(g, 4000); /* add 4000 verticies! */
    mu_assert_int_eq(4000, g_num_vertices(g));
}

MU_TEST(test_updating_vertex_metadata) {
    __add_verticies(g, 5);
    vertex_t v = g_vertex_get(g, 0);
    int* metadata = g_vertex_metadata(v);
    mu_assert_int_eq(0, *metadata);
    *metadata = 255;
    g_vertex_metadata_update(v, metadata);
    v = g_vertex_get(g, 0);
    mu_assert_int_eq(255, *(int*)g_vertex_metadata(v));
}


/*******************************************************************************
*   Test adding and removing edges
*******************************************************************************/
MU_TEST(test_add_edges) {
    __add_verticies(g, 15);
    mu_assert_int_eq(15, g_num_vertices(g));

    g_edge_add(g, 0, 1, __str_duplicate("1"));
    g_edge_add(g, 0, 2, __str_duplicate("2"));
    g_edge_add(g, 0, 3, __str_duplicate("3"));
    g_edge_add(g, 0, 4, __str_duplicate("4"));
    g_edge_add(g, 0, 5, __str_duplicate("5"));
    g_edge_add(g, 0, 6, __str_duplicate("6"));
    mu_assert_int_eq(6, g_num_edges(g));

    edge_t e = g_edge_get(g, 5);
    mu_assert_int_eq(0, g_edge_src(e));
    mu_assert_int_eq(6, g_edge_dest(e));
    mu_assert_string_eq("6", g_edge_metadata(e));
}

MU_TEST(test_remove_edges) {
    __add_verticies(g, 15);
    mu_assert_int_eq(15, g_num_vertices(g));

    g_edge_add(g, 0, 1, __str_duplicate("1"));
    g_edge_add(g, 0, 2, __str_duplicate("2"));
    g_edge_add(g, 0, 3, __str_duplicate("3"));
    g_edge_add(g, 0, 4, __str_duplicate("4"));
    g_edge_add(g, 0, 5, __str_duplicate("5"));
    g_edge_add(g, 0, 6, __str_duplicate("6"));

    mu_assert_int_eq(6, g_num_edges(g));
    edge_t e = g_edge_remove(g, 0);
    mu_assert_int_eq(0, g_edge_id(e));
    mu_assert_int_eq(0, g_edge_src(e));
    mu_assert_int_eq(1, g_edge_dest(e));
    mu_assert_int_eq(5, g_num_edges(g));
    g_edge_free(e);
}

MU_TEST(test_remove_edges_src) {
    /*  this tests removing a vertex and ensuring that all the edges it was
        attached to are also removed */
    __add_verticies(g, 15);
    mu_assert_int_eq(15, g_num_vertices(g));

    g_edge_add(g, 0, 1, __str_duplicate("0-1"));
    g_edge_add(g, 0, 2, __str_duplicate("0-2"));
    g_edge_add(g, 0, 3, __str_duplicate("0-3"));
    g_edge_add(g, 0, 4, __str_duplicate("0-4"));
    g_edge_add(g, 0, 5, __str_duplicate("0-5"));
    g_edge_add(g, 0, 6, __str_duplicate("0-6"));
    g_edge_add(g, 14, 0, __str_duplicate("15-0"));

    /* not attached edges */
    g_edge_add(g, 1, 10, __str_duplicate("1-10"));

    mu_assert_int_eq(8, g_num_edges(g));
    vertex_t v = g_vertex_get(g, 0);

    mu_assert_int_eq(6, g_vertex_num_edges_out(v));
    mu_assert_int_eq(1, g_vertex_num_edges_in(v));

    v = g_vertex_remove(g, 0);
    mu_assert_int_eq(14, g_num_vertices(g));
    mu_assert_int_eq(1, g_num_edges(g));
    g_vertex_free(v);
}

MU_TEST(test_edges_growth) {
    __add_verticies(g, 4000); /* add 4000 verticies! */
    unsigned int i;
    for (i = 1; i < g_num_vertices(g); i++) {
        g_edge_add(g, i - 1, i, NULL);
    }
    mu_assert_int_eq(3999, g_num_edges(g));
}

MU_TEST(test_updating_edge_metadata) {
    __add_verticies(g, 15);
    g_edge_add(g, 0, 1, __str_duplicate("0-1"));
    g_edge_add(g, 0, 2, __str_duplicate("0-2"));
    g_edge_add(g, 0, 3, __str_duplicate("0-3"));

    edge_t e = g_edge_get(g, 0);
    char* metadata = (char*)g_edge_metadata(e);
    mu_assert_string_eq("0-1", metadata);
    metadata[0] = '-';
    mu_assert_string_eq("--1", (char*)g_edge_metadata(e));
}

MU_TEST(test_edge_add_error) {
    __add_verticies(g, 5);
    edge_t e = g_edge_add(g, 0, 5, NULL); /* this should return NULL since dest is too large*/
    mu_assert_string_eq(NULL, (char*)e);
    e = g_edge_add(g, 6, 0, NULL);
    mu_assert_string_eq(NULL, (char*)e);

    /* now remove a vertex and try to add an edge to it */
    vertex_t v = g_vertex_remove(g, 0);
    g_vertex_free(v);
    e = g_edge_add(g, 0, 1, NULL);
    mu_assert_string_eq(NULL, (char*)e);
    e = g_edge_add(g, 1, 0, NULL);
    mu_assert_string_eq(NULL, (char*)e);
}

MU_TEST(test_edge_remove_error) {
    __add_verticies(g, 5);
    g_edge_add(g, 0, 4, NULL);
    g_edge_add(g, 1, 4, NULL);
    g_edge_add(g, 2, 4, NULL);
    g_edge_add(g, 3, 4, NULL);

    edge_t e = g_edge_remove(g, 5);
    mu_assert_string_eq(NULL, (char*)e);

    e = g_edge_remove(g, 3); /* this should be fine */
    g_edge_free(e);
    e = g_edge_remove(g, 3); /* now we should get a NULL back */
    mu_assert_string_eq(NULL, (char*)e);
}


/*******************************************************************************
*   Test iterating over the verticies
*******************************************************************************/
MU_TEST(test_iterate_verticies_all_there) {
    __add_verticies(g, 5);
    unsigned int i;
    vertex_t v;
    g_iterate_verticies(g,v,i) {
        mu_assert_int_eq(i, *(int*)g_vertex_metadata(v));
    }
}

MU_TEST(test_iterate_verticies_some_removed) {
    __add_verticies(g, 5);

    vertex_t t = g_vertex_remove(g, 2);
    g_vertex_free(t);

    unsigned int i;
    vertex_t v;
    g_iterate_verticies(g,v,i) {
        mu_assert_int_eq(i, *(int*)g_vertex_metadata(v));
    }
}

MU_TEST(test_iterate_edges) {
    __add_verticies(g, 5);
    __add_edge(g, 0, 1, 0);
    __add_edge(g, 0, 2, 1);
    __add_edge(g, 0, 3, 2);
    __add_edge(g, 0, 4, 3);

    unsigned int i, j = 0;
    edge_t e;
    vertex_t v = g_vertex_get(g, 0);
    g_iterate_edges(v, e, i) {
        mu_assert_int_eq(i, *(int*)g_edge_metadata(e));
        ++j;
    }
    mu_assert_int_eq(4, j);
}

MU_TEST(test_iterate_edges_large) {
    __add_verticies(g, 5);
    unsigned int i, j = 0;
    for (i = 0; i < 65; i++) { /* add 64 edges to the same vertex */
        __add_edge(g, 0, i % 5, i);
    }


    mu_assert_int_eq(65, g_num_edges(g));

    edge_t e;
    vertex_t v = g_vertex_get(g, 0);
    g_iterate_edges(v, e, i) {
        // mu_assert_int_eq(i, *(int*)g_edge_metadata(e));
        ++j;
    }
    mu_assert_int_eq(65, j);
}

MU_TEST(test_iterate_edges_some_removed) {
    __add_verticies(g, 5);
    __add_edge(g, 0, 1, 0);
    __add_edge(g, 0, 2, 1);
    __add_edge(g, 0, 3, 2);
    __add_edge(g, 0, 4, 3);

    edge_t t = g_edge_remove(g, 1);
    g_edge_free(t);
    t = g_edge_remove(g, 2);
    g_edge_free(t);
    unsigned int i, j = 0;
    edge_t e;
    vertex_t v = g_vertex_get(g, 0);
    /* wierd test, but should be true since we should have vals 0 and 3 left */
    g_iterate_edges(v, e, i) {
        mu_assert_int_eq(i * 3, *(int*)g_edge_metadata(e));
        ++j;
    }
    mu_assert_int_eq(2, j);
}

MU_TEST(test_iterate_edges_some_removed_add_back) {
    __add_verticies(g, 5);
    __add_edge(g, 0, 1, 0);
    __add_edge(g, 0, 2, 1);
    __add_edge(g, 0, 3, 2);
    __add_edge(g, 0, 4, 3);

    edge_t t = g_edge_remove(g, 1);
    g_edge_free(t);
    t = g_edge_remove(g, 2);
    g_edge_free(t);

    __add_edge(g, 0, 4, 6); /* this function just turns the last int into a pointer for metadata */
    unsigned int i, j = 0;
    edge_t e;
    vertex_t v = g_vertex_get(g, 0);
    /* wierd test, but should be true since we should have vals 0, 3, and 6 left */
    g_iterate_edges(v, e, i) {
        mu_assert_int_eq(i * 3, *(int*)g_edge_metadata(e));
        ++j;
    }
    mu_assert_int_eq(3, j);
}


/*******************************************************************************
*    Test Suite Setup
*******************************************************************************/
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_default_setup);

    /* add & remove verticies */
    MU_RUN_TEST(test_add_verticies);
    MU_RUN_TEST(test_remove_verticies);
    MU_RUN_TEST(test_verticies_growth);
    MU_RUN_TEST(test_updating_vertex_metadata);

    /* add & remove edges */
    MU_RUN_TEST(test_add_edges);
    MU_RUN_TEST(test_remove_edges);
    MU_RUN_TEST(test_remove_edges_src);
    MU_RUN_TEST(test_edges_growth);
    MU_RUN_TEST(test_edge_add_error);
    MU_RUN_TEST(test_edge_remove_error);

    /* Iteration tests */
    MU_RUN_TEST(test_iterate_verticies_all_there);
    MU_RUN_TEST(test_iterate_verticies_some_removed);
    MU_RUN_TEST(test_iterate_edges);
    MU_RUN_TEST(test_iterate_edges_large);
    MU_RUN_TEST(test_updating_edge_metadata);
    MU_RUN_TEST(test_iterate_edges_some_removed);
    MU_RUN_TEST(test_iterate_edges_some_removed_add_back);
}



int main() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    printf("Number failed tests: %d\n", minunit_fail);
    return minunit_fail;
}


/* Private Functions */
static char* __str_duplicate(const char* s) {
    size_t len = strlen(s);
    char* buf = malloc((len + 1) * sizeof(char));
    if (buf == NULL)
        return NULL;
    strcpy(buf, s);
    buf[len] = '\0';
    return buf;
}

static void  __add_verticies(graph_t g, int num) {
    int i;
    for (i = 0; i < num; i++) {
        int* q = calloc(1, sizeof(int));
        *q = i;
        g_vertex_add(g, q);
    }
}

static void __add_edge(graph_t g, unsigned int src, unsigned int dest, int val) {
    int* q = calloc(1, sizeof(int));
    *q = val;
    g_edge_add(g, src, dest, q);
}
