
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "../src/graph.h"

int main(int argc, char const *argv[]) {
    bool verbose = false;

    if (argc == 2 && strcmp(argv[1], "-v") == 0) {
        verbose = true;
    }

    graph_t g = g_init();

    /* I don't have any metadata... for now */
    vertex_t v0 = g_add_vertex(g, NULL); assert(g_vertex_id(v0) == 0);
    vertex_t v1 = g_add_vertex(g, NULL); assert(g_vertex_id(v1) == 1);
    vertex_t v2 = g_add_vertex(g, NULL); assert(g_vertex_id(v2) == 2);
    vertex_t v3 = g_add_vertex(g, NULL); assert(g_vertex_id(v3) == 3);
    vertex_t v4 = g_add_vertex(g, NULL); assert(g_vertex_id(v4) == 4);
    vertex_t v5 = g_add_vertex(g, NULL); assert(g_vertex_id(v5) == 5);

    /* set the edges in a directed circle! */
    edge_t e0 = g_add_edge(g, 0, 1, NULL);
    edge_t e1 = g_add_edge(g, 1, 2, NULL);
    edge_t e2 = g_add_edge(g, 2, 3, NULL);
    edge_t e3 = g_add_edge(g, 3, 4, NULL);
    edge_t e4 = g_add_edge(g, 4, 5, NULL);
    edge_t e5 = g_add_edge(g, 5, 0, NULL);  /* this should cause an error */


    /* we could do some tests here... */

    g_free(g);
    if (verbose == true)
        printf("Completed!\n");

    return 0;
}
