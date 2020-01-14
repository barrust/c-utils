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
    vertex_t v = g_add_vertex(g, __str_duplicate("this is a test"));
    mu_assert_int_eq(1, g_num_vertices(g));
    mu_assert_int_eq(0, g_vertex_id(v));
    mu_assert_string_eq("this is a test", g_vertex_metadata(v));

    g_add_vertex(g, __str_duplicate("d3-football"));
    g_add_vertex(g, __str_duplicate("college hoops"));
    mu_assert_int_eq(3, g_num_vertices(g));

    v = g_get_vertex(g, 1);
    mu_assert_int_eq(1, g_vertex_id(v));
    mu_assert_string_eq("d3-football", g_vertex_metadata(v));
}

MU_TEST(test_remove_verticies) {
    mu_assert_int_eq(0, g_num_vertices(g));
    g_add_vertex(g, __str_duplicate("this is a test"));
    g_add_vertex(g, __str_duplicate("d3-football"));
    g_add_vertex(g, __str_duplicate("college hoops"));

    mu_assert_int_eq(3, g_num_vertices(g));

    vertex_t v = g_remove_vertex(g, 0);
    mu_assert_int_eq(0, g_vertex_id(v));
    mu_assert_string_eq("this is a test", g_vertex_metadata(v));
    mu_assert_int_eq(2, g_num_vertices(g));
    g_vertex_free(v);

    v = g_remove_vertex(g, 2);
    mu_assert_int_eq(2, g_vertex_id(v));
    mu_assert_string_eq("college hoops", g_vertex_metadata(v));
    mu_assert_int_eq(1, g_num_vertices(g));
    g_vertex_free(v);

    /* check that something removed is clean! */
    v = g_remove_vertex(g, 0);
    mu_assert_string_eq(NULL, (char*)v);
    mu_assert_int_eq(1, g_num_vertices(g));  /* it shouldn't change the num verticies */
}

MU_TEST(test_verticies_growth) {
    __add_verticies(g, 4000); /* add 4000 verticies! */
    mu_assert_int_eq(4000, g_num_vertices(g));
}


/*******************************************************************************
*   Test adding and removing edges
*******************************************************************************/
MU_TEST(test_add_edges) {
    __add_verticies(g, 15);
    mu_assert_int_eq(15, g_num_vertices(g));

    g_add_edge(g, 0, 1, __str_duplicate("1"));
    g_add_edge(g, 0, 2, __str_duplicate("2"));
    g_add_edge(g, 0, 3, __str_duplicate("3"));
    g_add_edge(g, 0, 4, __str_duplicate("4"));
    g_add_edge(g, 0, 5, __str_duplicate("5"));
    g_add_edge(g, 0, 6, __str_duplicate("6"));
    mu_assert_int_eq(6, g_num_edges(g));

    edge_t e = g_get_edge(g, 5);
    mu_assert_int_eq(0, g_edge_src(e));
    mu_assert_int_eq(6, g_edge_dest(e));
    mu_assert_string_eq("6", g_edge_metadata(e));
}

MU_TEST(test_remove_edges) {
    __add_verticies(g, 15);
    mu_assert_int_eq(15, g_num_vertices(g));

    g_add_edge(g, 0, 1, __str_duplicate("1"));
    g_add_edge(g, 0, 2, __str_duplicate("2"));
    g_add_edge(g, 0, 3, __str_duplicate("3"));
    g_add_edge(g, 0, 4, __str_duplicate("4"));
    g_add_edge(g, 0, 5, __str_duplicate("5"));
    g_add_edge(g, 0, 6, __str_duplicate("6"));

    mu_assert_int_eq(6, g_num_edges(g));
    edge_t e = g_remove_edge(g, 0);
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

    g_add_edge(g, 0, 1, __str_duplicate("0-1"));
    g_add_edge(g, 0, 2, __str_duplicate("0-2"));
    g_add_edge(g, 0, 3, __str_duplicate("0-3"));
    g_add_edge(g, 0, 4, __str_duplicate("0-4"));
    g_add_edge(g, 0, 5, __str_duplicate("0-5"));
    g_add_edge(g, 0, 6, __str_duplicate("0-6"));
    g_add_edge(g, 14, 0, __str_duplicate("15-0"));

    /* not attached edges */
    g_add_edge(g, 1, 10, __str_duplicate("1-10"));

    mu_assert_int_eq(8, g_num_edges(g));
    vertex_t v = g_get_vertex(g, 0);

    mu_assert_int_eq(6, g_vertex_num_edges_out(v));
    mu_assert_int_eq(1, g_vertex_num_edges_in(v));

    v = g_remove_vertex(g, 0);
    mu_assert_int_eq(14, g_num_vertices(g));
    mu_assert_int_eq(1, g_num_edges(g));
    g_vertex_free(v);
}

MU_TEST(test_edges_growth) {
    __add_verticies(g, 4000); /* add 4000 verticies! */
    unsigned int i;
    for (i = 1; i < g_num_vertices(g); i++) {
        g_add_edge(g, i - 1, i, NULL);
    }
    mu_assert_int_eq(3999, g_num_edges(g));
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

    /* add & remove edges */
    MU_RUN_TEST(test_add_edges);
    MU_RUN_TEST(test_remove_edges);
    MU_RUN_TEST(test_remove_edges_src);
    MU_RUN_TEST(test_edges_growth);
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
        g_add_vertex(g, q);
    }
}
