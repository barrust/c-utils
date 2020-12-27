#include <stdlib.h>
#include <stdbool.h>
#if defined (_OPENMP)
    #include <omp.h>
#endif
#include "../src/minunit.h"
#include "../src/graph.h"


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
static void  __add_vertices(graph_t g, int num);
static void  __add_edge(graph_t g, unsigned int src, unsigned int dest, int val);


/*******************************************************************************
*   Test the setup
*******************************************************************************/
MU_TEST(test_default_setup) {
    mu_assert_int_eq(0, g_num_edges(g));
    mu_assert_int_eq(0, g_num_vertices(g));
}


/*******************************************************************************
*   Test adding and removing vertices
*******************************************************************************/
MU_TEST(test_add_vertices) {
    mu_assert_int_eq(0, g_num_vertices(g));
    vertex_t v = g_vertex_add(g, __str_duplicate("this is a test"));
    mu_assert_int_eq(1, g_num_vertices(g));
    mu_assert_int_eq(0, g_vertex_id(v));
    mu_assert_string_eq("this is a test", (char*)g_vertex_metadata(v));

    g_vertex_add(g, __str_duplicate("d3-football"));
    g_vertex_add(g, __str_duplicate("college hoops"));
    mu_assert_int_eq(3, g_num_vertices(g));

    v = g_vertex_get(g, 1);
    mu_assert_int_eq(1, g_vertex_id(v));
    mu_assert_string_eq("d3-football", (char*)g_vertex_metadata(v));
}

MU_TEST(test_add_verticies_idx_error) {
    vertex_t v = g_vertex_add(g, __str_duplicate("we are here..."));
    v = g_vertex_add_alt(g, 0, NULL); // this should be an error!
    mu_assert_null(v);
}

MU_TEST(test_add_vertex_large_idx) {
    vertex_t v = g_vertex_add_alt(g, 4097, __str_duplicate("we are here..."));
    mu_assert_int_eq(1, g_num_vertices(g));
    mu_assert_int_eq(4097, g_vertex_id(v));

    vertex_t q = g_vertex_get(g, 4097);
    mu_assert_int_eq(4097, g_vertex_id(q));
}

MU_TEST(test_remove_vertices) {
    mu_assert_int_eq(0, g_num_vertices(g));
    g_vertex_add(g, __str_duplicate("this is a test"));
    g_vertex_add(g, __str_duplicate("d3-football"));
    g_vertex_add(g, __str_duplicate("college hoops"));

    mu_assert_int_eq(3, g_num_vertices(g));

    vertex_t v = g_vertex_remove(g, 0);
    mu_assert_int_eq(0, g_vertex_id(v));
    mu_assert_string_eq("this is a test", (char*)g_vertex_metadata(v));
    mu_assert_int_eq(2, g_num_vertices(g));
    g_vertex_free(v);

    v = g_vertex_remove(g, 2);
    mu_assert_int_eq(2, g_vertex_id(v));
    mu_assert_string_eq("college hoops", (char*)g_vertex_metadata(v));
    mu_assert_int_eq(1, g_num_vertices(g));
    g_vertex_free(v);

    /* check that something removed is clean! */
    v = g_vertex_remove(g, 0);
    mu_assert_null(v);
    mu_assert_int_eq(1, g_num_vertices(g));  /* it shouldn't change the num vertices */
}

MU_TEST(test_vertices_growth) {
    __add_vertices(g, 4000); /* add 4000 vertices! */
    unsigned int i;
    for (i = 0; i < 4000; i++) {  /* good for checking order when using openmp; only "check" if would fail */
        vertex_t v = g_vertex_get(g, i);
        if (g_vertex_id(v) != i) {
            mu_assert_int_eq(g_vertex_id(v), i);
        }
    }
    mu_assert_int_eq(4000, g_num_vertices(g));
}

MU_TEST(test_updating_vertex_metadata) {
    __add_vertices(g, 5);
    vertex_t v = g_vertex_get(g, 0);
    int* metadata = (int*)g_vertex_metadata(v);
    mu_assert_int_eq(0, *metadata);
    *metadata = 255;
    g_vertex_metadata_update(v, metadata);
    v = g_vertex_get(g, 0);
    mu_assert_int_eq(255, *(int*)g_vertex_metadata(v));
}

MU_TEST(test_get_vertex_errors) {
    __add_vertices(g, 5);
    mu_assert_null(g_vertex_get(g, 10));
}


/*******************************************************************************
*   Test adding and removing edges
*******************************************************************************/
MU_TEST(test_add_edges) {
    __add_vertices(g, 15);
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
    mu_assert_string_eq("6", (char*)g_edge_metadata(e));
}

MU_TEST(test_remove_edges) {
    __add_vertices(g, 15);
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
    __add_vertices(g, 15);
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
    __add_vertices(g, 4000); /* add 4000 vertices! */
    mu_assert_int_eq(4000, g_num_vertices(g));
    unsigned int i;
    for (i = 0; i < 4000; i++) {
        vertex_t v = g_vertex_get(g, i);
        if (v == NULL)
            printf("Vertex id=%d == NULL\n", i);
        // else
        //     mu_assert_int_eq(i, g_vertex_id(v));
    }
    for (i = 1; i < g_num_vertices(g); i++) {
        edge_t e = g_edge_add(g, i - 1, i, NULL);
        if (e == NULL) {
            printf("e was NULL: %d\n", i);
        }
    }
    mu_assert_int_eq(3999, g_num_edges(g));
}

MU_TEST(test_updating_edge_metadata) {
    __add_vertices(g, 15);
    g_edge_add(g, 0, 1, __str_duplicate("0-1"));
    g_edge_add(g, 0, 2, __str_duplicate("0-2"));
    g_edge_add(g, 0, 3, __str_duplicate("0-3"));

    edge_t e = g_edge_get(g, 0);
    char* metadata = (char*)g_edge_metadata(e);
    mu_assert_string_eq("0-1", metadata);
    metadata[0] = '-';
    g_edge_metadata_update(e, metadata);

    char* val = (char*)g_edge_metadata(e);
    mu_assert_string_eq("--1", val);
}

MU_TEST(test_edge_add_error) {
    __add_vertices(g, 5);
    edge_t e = g_edge_add(g, 0, 5, NULL); /* this should return NULL since dest is too large*/
    mu_assert_null(e);
    e = g_edge_add(g, 6, 0, NULL);
    mu_assert_null(e);

    /* now remove a vertex and try to add an edge to it */
    vertex_t v = g_vertex_remove(g, 0);
    g_vertex_free(v);
    e = g_edge_add(g, 0, 1, NULL);
    mu_assert_null(e);
    e = g_edge_add(g, 1, 0, NULL);
    mu_assert_null(e);
}

MU_TEST(test_edge_remove_error) {
    __add_vertices(g, 5);
    g_edge_add(g, 0, 4, NULL);
    g_edge_add(g, 1, 4, NULL);
    g_edge_add(g, 2, 4, NULL);
    g_edge_add(g, 3, 4, NULL);

    edge_t e = g_edge_remove(g, 5);
    mu_assert_null(e);

    e = g_edge_remove(g, 3); /* this should be fine */
    g_edge_free(e);
    e = g_edge_remove(g, 3); /* now we should get a NULL back */
    mu_assert_null(e);
}

MU_TEST(test_edge_get_error) {
    __add_vertices(g, 5);
    g_edge_add(g, 0, 4, NULL);
    mu_assert_null(g_edge_get(g, 2));
}


/*******************************************************************************
*   Test iterating over the vertices
*******************************************************************************/
MU_TEST(test_iterate_vertices_all_there) {
    __add_vertices(g, 5);
    unsigned int i;
    vertex_t v;
    g_iterate_vertices(g,v,i) {
        mu_assert_int_eq(i, *(int*)g_vertex_metadata(v));
    }
}

MU_TEST(test_iterate_vertices_some_removed) {
    __add_vertices(g, 5);

    vertex_t t = g_vertex_remove(g, 2);
    g_vertex_free(t);

    unsigned int i;
    vertex_t v;
    g_iterate_vertices(g,v,i) {
        mu_assert_int_eq(i, *(int*)g_vertex_metadata(v));
    }
}

MU_TEST(test_iterate_edges) {
    __add_vertices(g, 5);
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
    __add_vertices(g, 5);
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
    __add_vertices(g, 5);
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
    __add_vertices(g, 5);
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

MU_TEST(test_g_vertex_edge_error) {
    __add_vertices(g, 5);
    __add_edge(g, 0, 1, 0);
    vertex_t v = g_vertex_get(g, 0);
    mu_assert_null(g_vertex_edge(v, 1));
    mu_assert_null(g_vertex_edge(v, 17));
}

/*******************************************************************************
*   Test iterating with breadth and depth
*******************************************************************************/
MU_TEST(test_g_breadth_first_traverse) {
    __add_vertices(g, 15);
    __add_edge(g, 0, 1, 0);
    __add_edge(g, 0, 3, 0);
    __add_edge(g, 0, 2, 0);
    __add_edge(g, 1, 4, 0);
    __add_edge(g, 1, 5, 0);
    __add_edge(g, 2, 9, 0);
    __add_edge(g, 3, 10, 0);
    __add_edge(g, 10, 6, 0);
    __add_edge(g, 4, 8, 0);
    __add_edge(g, 4, 12, 0);
    __add_edge(g, 6, 14, 0);
    __add_edge(g, 9, 13, 0);
    __add_edge(g, 9, 1, 0);

    int answers[] = {0, 1, 3, 2, 4, 5, 10, 9, 8, 12, 6, 13, 14};
    unsigned int len, i;
    unsigned int* res = g_breadth_first_traverse(g, g_vertex_get(g, 0), &len);

    mu_assert_int_eq(13, len);
    for (i = 0; i < len; i++) {
        mu_assert_int_eq(answers[i], res[i]);
    }
    free(res);
}


MU_TEST(test_g_depth_first_traverse) {
    __add_vertices(g, 15);
    __add_edge(g, 0, 1, 0);
    __add_edge(g, 0, 3, 0);
    __add_edge(g, 0, 2, 0);
    __add_edge(g, 1, 4, 0);
    __add_edge(g, 1, 5, 0);
    __add_edge(g, 2, 9, 0);
    __add_edge(g, 3, 10, 0);
    __add_edge(g, 10, 6, 0);
    __add_edge(g, 4, 8, 0);
    __add_edge(g, 4, 12, 0);
    __add_edge(g, 6, 14, 0);
    __add_edge(g, 9, 13, 0);
    __add_edge(g, 9, 1, 0);

    int answers[] = {0, 1, 4, 8, 12, 5, 3, 10, 6, 14, 2, 9, 13};
    unsigned int len, i;
    unsigned int* res = g_depth_first_traverse(g, g_vertex_get(g, 0), &len);
    mu_assert_int_eq(13, len);
    for (i = 0; i < len; i++) {
        mu_assert_int_eq(answers[i], res[i]);
    }
    free(res);
}

/*******************************************************************************
*    Test Suite Setup
*******************************************************************************/
MU_TEST_SUITE(test_suite) {
    MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

    MU_RUN_TEST(test_default_setup);

    /* add & remove vertices */
    MU_RUN_TEST(test_add_vertices);
    MU_RUN_TEST(test_add_verticies_idx_error);
    MU_RUN_TEST(test_add_vertex_large_idx);
    MU_RUN_TEST(test_remove_vertices);
    MU_RUN_TEST(test_vertices_growth);
    MU_RUN_TEST(test_updating_vertex_metadata);
    MU_RUN_TEST(test_get_vertex_errors);

    /* add & remove edges */
    MU_RUN_TEST(test_add_edges);
    MU_RUN_TEST(test_remove_edges);
    MU_RUN_TEST(test_remove_edges_src);
    MU_RUN_TEST(test_edges_growth);
    MU_RUN_TEST(test_edge_add_error);
    MU_RUN_TEST(test_edge_remove_error);
    MU_RUN_TEST(test_edge_get_error);
    MU_RUN_TEST(test_g_vertex_edge_error);

    /* Iteration tests */
    MU_RUN_TEST(test_iterate_vertices_all_there);
    MU_RUN_TEST(test_iterate_vertices_some_removed);
    MU_RUN_TEST(test_iterate_edges);
    MU_RUN_TEST(test_iterate_edges_large);
    MU_RUN_TEST(test_updating_edge_metadata);
    MU_RUN_TEST(test_iterate_edges_some_removed);
    MU_RUN_TEST(test_iterate_edges_some_removed_add_back);

    /* Traversals */
    MU_RUN_TEST(test_g_breadth_first_traverse);
    MU_RUN_TEST(test_g_depth_first_traverse);
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
    char* buf = (char*)malloc((len + 1) * sizeof(char));
    if (buf == NULL)
        return NULL;
    strcpy(buf, s);
    buf[len] = '\0';
    return buf;
}

static void  __add_vertices(graph_t g, int num) {
    int i;
    #pragma omp parallel for
    for (i = 0; i < num; i++) {
        int* q = (int*)calloc(1, sizeof(int));
        *q = i;
        g_vertex_add_alt(g, i, q);
    }
}

static void __add_edge(graph_t g, unsigned int src, unsigned int dest, int val) {
    int* q = (int*)calloc(1, sizeof(int));
    *q = val;
    g_edge_add(g, src, dest, q);
}
