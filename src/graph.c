
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"


typedef struct __graph {
    unsigned int num_verts;
    unsigned int num_edges;
    unsigned int _max_verts;
    unsigned int _max_edges;
    unsigned int _prev_vert_id;
    unsigned int _prev_edge_id;
    vertex_t* verts;
    edge_t* edges;
} Graph;

typedef struct __vertex_node {
    unsigned int id;
    unsigned int num_edges_in;
    unsigned int num_edges_out;
    void* metadata; /* use this to hold name, other wanted information, etc */
} Vertex;

typedef struct __edge_node{
    unsigned int id;
    unsigned int src;
    unsigned int dest;
    void* metadata;
} Edge;


/*******************************************************************************
*   Graph Properties / Functions
*******************************************************************************/
graph_t g_init() {
    graph_t g = calloc(1, sizeof(Graph));
    if (g == NULL)
        return NULL;

    g->num_edges = 0;
    g->num_verts = 0;
    g->_prev_vert_id = 0;
    g->_prev_edge_id = 0;
    g->_max_verts = 1024;
    g->_max_edges = 1024;

    g->verts = calloc(g->_max_verts, sizeof(Vertex));
    g->edges = calloc(g->_max_edges, sizeof(Edge));

    unsigned int i;
    for (i = 0; i < g->_max_edges; i++) {
        g->verts[i] = NULL;
        g->edges[i] = NULL;
    }

    return g;
}

int g_free(graph_t g) {

    unsigned int i;
    for (i = 0; i < g->_max_edges; i++) {
        edge_t e = g->edges[i];
        if (e == NULL)
            continue;
        g_edge_free(e);
    }
    free(g->edges);

    for (i = 0; i < g->_max_verts; i++) {
        vertex_t v = g->verts[i];
        if (v == NULL)
            continue;
        g_vertex_free(v);
    }
    free(g->verts);

    g->num_verts = 0;
    g->num_edges = 0;
    g->_max_verts = 0;
    g->_max_edges = 0;
    g->_prev_vert_id = 0;
    g->_prev_edge_id = 0;

    free(g);
    return GRAPH_SUCCESS;
}

vertex_t g_add_vertex(graph_t g, void* metadata) {
    vertex_t v = calloc(1, sizeof(Vertex));
    if (v == NULL)
        return NULL;
    unsigned int id = (g->_prev_vert_id)++;
    if (id > g->_max_verts) {
        /* need to expand the verts! */
        unsigned int new_num_verts = g->_max_verts * 2; /* double */
        vertex_t* tmp = realloc(g->verts, new_num_verts * sizeof(vertex_t));
        if (tmp == NULL)
            return NULL;  /* undefined errors! */
        g->_max_verts = new_num_verts;
        g->verts = tmp;
    }
    v->id = id;
    v->metadata = metadata;
    g->verts[id] = v;
    ++(g->num_verts);
    return v;
}

vertex_t g_remove_vertex(graph_t g, unsigned int id) {
    if (id > g->_prev_vert_id || g->verts[id] == NULL)
        return NULL;
    vertex_t v = g->verts[id];

    /* need to find all the edges that include this vertex and remove them */
    unsigned int i;
    for (i = 0; i <= g->_prev_edge_id; i++) {
        edge_t e = g->edges[i];
        if (e == NULL)
            continue;
        if (e->src == id || e->dest == id)
            g_remove_edge(g, e->id);
    }

    /* remove the vertex from the graph */
    g->verts[id] = NULL;
    --(g->num_verts);

    return v;
}

edge_t g_add_edge(graph_t g, unsigned int src, unsigned int dest, void* metadata) {

    /* some tests to make sure src and dest are valid */
    if (src > g->_prev_vert_id || dest > g->_prev_vert_id)
        return NULL;
    edge_t e = calloc(1, sizeof(Edge));
    if (e == NULL)
        return NULL;
    unsigned int id = (g->_prev_edge_id)++;
    if (id > g->_max_edges) {
        /* need to expand the verts! */
        unsigned int new_num_edges = g->_max_edges * 2; /* double */
        edge_t* tmp = realloc(g->edges, new_num_edges * sizeof(edge_t));
        if (tmp == NULL)
            return NULL;  /* undefined errors! */
        g->_max_edges = new_num_edges;
        g->edges = tmp;
    }
    e->id = id;
    e->src = src;
    e->dest = dest;
    e->metadata = metadata;

    /* need to increment the in and out information for the verticies */
    g->verts[src]->num_edges_out++;
    g->verts[dest]->num_edges_in++;
    g->edges[id] = e;

    return e;
}

edge_t g_remove_edge(graph_t g, unsigned int id) {
    if (id > g->_prev_edge_id || g->edges[id] == NULL)
        return NULL;
    edge_t e = g->edges[id];
    g->edges[id] = NULL;
    --(g->num_edges);

    return e;
}


/*******************************************************************************
*   Vertex Properties / Functions
*******************************************************************************/
unsigned int g_vertex_id(vertex_t v) {
    return v->id;
}

unsigned int g_vertex_num_edges_in(vertex_t v) {
    return v->num_edges_in;
}

unsigned int g_vertex_num_edges_out(vertex_t v) {
    return v->num_edges_out;
}

void g_vertex_free(vertex_t v) {
    v->id = 0;
    v->num_edges_in = 0;
    v->num_edges_out = 0;
    free(v->metadata);
    free(v);
}

/*******************************************************************************
*   Edge Properties / Functions
*******************************************************************************/

unsigned int g_edge_id(edge_t e) {
    return e->id;
}

unsigned int g_edge_src(edge_t e) {
    return e->src;
}

unsigned int g_edge_dest(edge_t e) {
    return e->dest;
}

void g_edge_free(edge_t e) {
    e->id = 0;
    e->src = 0;
    e->dest = 0;
    free(e->metadata);
    free(e);
}
