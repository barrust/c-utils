
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
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
    return g_init_alt(1024);
}

graph_t g_init_alt(unsigned int size) {
    graph_t g = calloc(1, sizeof(Graph));
    if (g == NULL)
        return NULL;

    g->num_edges = 0;
    g->num_verts = 0;
    g->_prev_vert_id = 0;
    g->_prev_edge_id = 0;
    g->_max_verts = size;
    g->_max_edges = size;

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
    return g_free_alt(g, true);
}

int g_free_alt(graph_t g, bool free_metadata) {
    unsigned int i;
    for (i = 0; i < g->_max_edges; i++) {
        edge_t e = g->edges[i];
        if (e == NULL)
            continue;
        g_edge_free_alt(e, free_metadata);
    }
    free(g->edges);

    for (i = 0; i < g->_max_verts; i++) {
        vertex_t v = g->verts[i];
        if (v == NULL)
            continue;
        g_vertex_free_alt(v, free_metadata);
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

unsigned int g_num_vertices(graph_t g) {
    return g->num_verts;
}

unsigned int g_num_edges(graph_t g) {
    return g->num_edges;
}

vertex_t g_get_vertex(graph_t g, unsigned int id) {
    if (id > g->_prev_vert_id)
        return NULL;
    return g->verts[id];
}

edge_t g_get_edge(graph_t g, unsigned int id) {
    if (id > g->_prev_edge_id)
        return NULL;
    return g->edges[id];
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
    return g_remove_vertex_alt(g, id, true);
}

vertex_t g_remove_vertex_alt(graph_t g, unsigned int id, bool free_edge_metadata) {
    if (id > g->_prev_vert_id || g->verts[id] == NULL)
        return NULL;
    vertex_t v = g->verts[id];

    /* need to find all the edges that include this vertex and remove them */
    unsigned int i;
    for (i = 0; i <= g->_prev_edge_id; i++) {
        edge_t e = g->edges[i];
        if (e == NULL)
            continue;
        if (e->src == id || e->dest == id) {
            e = g_remove_edge(g, i);
            g_edge_free_alt(e, free_edge_metadata);
        }
    }

    /* remove the vertex from the graph */
    g->verts[id] = NULL;
    --(g->num_verts);

    return v;
}

edge_t g_add_edge(graph_t g, unsigned int src, unsigned int dest, void* metadata) {
    /*  some tests to make sure src and dest are valid;
        need to make sure they are still present too */
    if (src > g->_prev_vert_id || dest > g->_prev_vert_id || g->verts[src] == NULL || g->verts[dest] == NULL)
        return NULL;

    edge_t e = calloc(1, sizeof(Edge));
    if (e == NULL)
        return NULL;
    unsigned int id = (g->_prev_edge_id)++;
    if (id > g->_max_edges) {
        /* need to expand the edges! */
        unsigned int new_num_edges = g->_max_edges * 2; /* double */
        edge_t* tmp = realloc(g->edges, new_num_edges * sizeof(edge_t));
        g->_max_edges = new_num_edges;
        g->edges = tmp;
    }
    e->id = id;
    e->src = src;
    e->dest = dest;
    e->metadata = metadata;

    /* need to increment the in and out information for the verticies */
    ++(g->verts[src]->num_edges_out);
    ++(g->verts[dest]->num_edges_in);
    ++(g->num_edges);
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

void* g_vertex_metadata(vertex_t v) {
    return v->metadata;
}

void g_vertex_free(vertex_t v) {
    g_vertex_free_alt(v, true);
}
void g_vertex_free_alt(vertex_t v, bool free_metadata) {
    v->id = 0;
    v->num_edges_in = 0;
    v->num_edges_out = 0;
    if (free_metadata == true)
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

void* g_edge_metadata(edge_t e) {
    return e->metadata;
}

void g_edge_free(edge_t e) {
    g_edge_free_alt(e, true);
}

void g_edge_free_alt(edge_t e, bool free_metadata) {
    e->id = 0;
    e->src = 0;
    e->dest = 0;
    if (free_metadata == true)
        free(e->metadata);
    free(e);
}
