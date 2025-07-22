/*******************************************************************************
***  Author: Tyler Barrus
***  email:  barrust@gmail.com
***  License: MIT 2020
*******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "graph.h"


#if defined (_OPENMP)
    #include <omp.h>  /* not sure this is actually needed! */
    #define ATOMIC _Pragma          ("omp atomic")
    #define CRITICAL _Pragma        ("omp critical (graph_t_critical)")
    #define CRITICAL_EDGE _Pragma   ("omp critical (graph_t_edge_critical)")
    #define ATOMIC_ADD_FETCH(i)     (__atomic_add_fetch(&(i), 1, __ATOMIC_SEQ_CST))
    #define ATOMIC_FETCH_ADD(i)     (__atomic_fetch_add(&(i), 1, __ATOMIC_SEQ_CST))
    #define ATOMIC_SUB_FETCH(i)     (__atomic_sub_fetch(&(i), 1, __ATOMIC_SEQ_CST))
#else
    #define ATOMIC
    #define CRITICAL
    #define CRITICAL_EDGE
    #define ATOMIC_ADD_FETCH(i)     (++(i))
    #define ATOMIC_FETCH_ADD(i)     ((i)++)
    #define ATOMIC_SUB_FETCH(i)     (--(i))
#endif

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
    unsigned int _max_edges;
    void* metadata;  /* use this to hold name, other wanted information, etc */
    edge_t* edges;
} Vertex;

typedef struct __edge_node{
    unsigned int id;
    unsigned int src;
    unsigned int dest;
    void* metadata;
} Edge;

/* private functions */
static void __traverse_depth_first(graph_t g, unsigned int* res, char* bitarray, unsigned int* size);
static void __graph_vertices_grow(graph_t g, unsigned int id);
static void __graph_edges_grow(graph_t g, unsigned int id);
static void __vertex_edges_grow(vertex_t v_src, unsigned int outs);

/*******************************************************************************
*   Graph Properties / Functions
*******************************************************************************/
graph_t g_init(void) {
    return g_init_alt(1024);
}

graph_t g_init_alt(unsigned int size) {
    graph_t g = (graph_t)calloc(1, sizeof(Graph));
    if (g == NULL)
        return NULL;

    g->num_edges = 0;
    g->num_verts = 0;
    g->_prev_vert_id = 0;
    g->_prev_edge_id = 0;
    g->_max_verts = size;
    g->_max_edges = size;

    g->verts = (vertex_t*)calloc(g->_max_verts, sizeof(vertex_t));
    g->edges = (edge_t*)calloc(g->_max_edges, sizeof(edge_t));

    unsigned int i;
    for (i = 0; i < g->_max_edges; ++i) {
        g->verts[i] = NULL;
        g->edges[i] = NULL;
    }

    return g;
}

void g_free(graph_t g) {
    g_free_alt(g, true);
}

void g_free_alt(graph_t g, bool free_metadata) {
    unsigned int i;
    for (i = 0; i < g->_max_edges; ++i) {
        edge_t e = g->edges[i];
        if (e == NULL)
            continue;
        g_edge_free_alt(e, free_metadata);
    }
    free(g->edges);

    for (i = 0; i < g->_max_verts; ++i) {
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
    g->verts = NULL;
    g->edges = NULL;

    free(g);
}

unsigned int g_num_vertices(graph_t g) {
    return g->num_verts;
}

unsigned int g_num_edges(graph_t g) {
    return g->num_edges;
}

unsigned int g_vertices_inserted(graph_t g) {
    return g->_prev_vert_id;
}

vertex_t g_vertex_get(graph_t g, unsigned int id) {
    if (id > g->_prev_vert_id)
        return NULL;
    return g->verts[id];
}

edge_t g_edge_get(graph_t g, unsigned int id) {
    if (id > g->_prev_edge_id)
        return NULL;
    return g->edges[id];
}

vertex_t g_vertex_add(graph_t g, void* metadata) {
    unsigned int id = ATOMIC_FETCH_ADD(g->_prev_vert_id);
    return g_vertex_add_alt(g, id, metadata);
}

vertex_t g_vertex_add_alt(graph_t g, unsigned int id, void* metadata) {
    /* check if mixed adding by id and add and clobbered the other */
    if (id >= g->_max_verts) {
        CRITICAL
        {
            __graph_vertices_grow(g, id);
        }
    } else if (/*id < g->_max_verts &&*/ g->verts[id] != NULL) {
        return NULL;
    }

    CRITICAL
    {
        if (id > g->_prev_vert_id) {
            g->_prev_vert_id = id + 1;
        }
    }

    vertex_t v = (vertex_t)calloc(1, sizeof(Vertex));
    if (v == NULL)
        return NULL;

    v->id = id;
    v->metadata = metadata;
    v->_max_edges = 16;  /* some starting point */
    v->edges = (edge_t*)calloc(v->_max_edges, sizeof(edge_t));
    v->num_edges_out = 0;
    v->num_edges_in = 0;
    g->verts[id] = v;
    ATOMIC_ADD_FETCH(g->num_verts);
    return v;
}

vertex_t g_vertex_remove(graph_t g, unsigned int id) {
    return g_vertex_remove_alt(g, id, true);
}

vertex_t g_vertex_remove_alt(graph_t g, unsigned int id, bool free_edge_metadata) {
    if (id > g->_prev_vert_id || g->verts[id] == NULL)
        return NULL;
    vertex_t v = g->verts[id];

    /* need to find all the edges that include this vertex and remove them */
    unsigned int i;
    for (i = 0; i <= g->_prev_edge_id; ++i) {
        edge_t e = g->edges[i];
        if (e == NULL)
            continue;
        if (e->src == id || e->dest == id) {
            e = g_edge_remove(g, i);
            g_edge_free_alt(e, free_edge_metadata);
        }
    }

    /* remove the vertex from the graph */
    g->verts[id] = NULL;
    ATOMIC_SUB_FETCH(g->num_verts);

    return v;
}

edge_t g_edge_add(graph_t g, unsigned int src, unsigned int dest, void* metadata) {
    /*  some tests to make sure src and dest are valid;
        need to make sure they are still present too */
    if (src > g->_prev_vert_id || dest > g->_prev_vert_id || g->verts[src] == NULL || g->verts[dest] == NULL)
        return NULL;

    edge_t e = (edge_t)calloc(1, sizeof(Edge));
    if (e == NULL)
        return NULL;

    unsigned int id = ATOMIC_FETCH_ADD(g->_prev_edge_id);
    if (id >= g->_max_edges) {
        CRITICAL_EDGE
        {
            __graph_edges_grow(g, id);
        }
    }
    e->id = id;
    e->src = src;
    e->dest = dest;
    e->metadata = metadata;

    /* need to increment the in and out information for the vertices */
    vertex_t v_src = g->verts[src];
    unsigned int outs = ATOMIC_FETCH_ADD(v_src->num_edges_out);
    if (outs >= v_src->_max_edges) {
        CRITICAL_EDGE
        {
            __vertex_edges_grow(v_src, outs);
        }
    }

    v_src->edges[outs] = e;
    ATOMIC_ADD_FETCH(g->verts[dest]->num_edges_in);
    ATOMIC_ADD_FETCH(g->num_edges);
    g->edges[id] = e;

    return e;
}

edge_t g_edge_remove(graph_t g, unsigned int id) {
    if (id > g->_prev_edge_id || g->edges[id] == NULL)
        return NULL;
    edge_t e = g->edges[id];
    g->edges[id] = NULL;
    ATOMIC_SUB_FETCH(g->num_edges);

    /*  find the correct location in the src and set to NULL
        but move the last one to fill it's spot */
    vertex_t v = g->verts[e->src];
    CRITICAL_EDGE
    {
        unsigned int i;
        for (i = 0; i < v->num_edges_out; ++i) {
            if (e == v->edges[i]) {
                v->edges[i] = v->edges[v->num_edges_out - 1];
                v->edges[v->num_edges_out - 1] = NULL;
                break;
            }
        }
    }
    ATOMIC_SUB_FETCH(v->num_edges_out);
    ATOMIC_SUB_FETCH((g->verts[e->dest])->num_edges_in);
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

void g_vertex_metadata_update(vertex_t v, void* metadata) {
    v->metadata = metadata;
}

edge_t g_vertex_edge(vertex_t v, unsigned int idx) {
    if (idx >= v->_max_edges)
        return NULL;
    return v->edges[idx];
}

void g_vertex_free(vertex_t v) {
    g_vertex_free_alt(v, true);
}

void g_vertex_free_alt(vertex_t v, bool free_metadata) {
    v->id = 0;
    v->num_edges_in = 0;
    v->num_edges_out = 0;
    v->_max_edges = 0;
    free(v->edges);
    if (free_metadata == true)
        free(v->metadata);

    v->metadata = NULL;
    v->edges = NULL;
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

void g_edge_metadata_update(edge_t e, void* metadata) {
    e->metadata = metadata;
}

void g_edge_free(edge_t e) {
    g_edge_free_alt(e, true);
}

void g_edge_free_alt(edge_t e, bool free_metadata) {
    if (e == NULL)
        return;
    e->id = 0;
    e->src = 0;
    e->dest = 0;
    if (free_metadata == true)
        free(e->metadata);
    e->metadata = NULL;
    free(e);
}


/*******************************************************************************
*   traversals
*******************************************************************************/
#define SET_BIT(A,k)        (A[((k) / 8)] |=  (1 << ((k) % 8)))
#define CHECK_BIT(A, k)     (A[((k) / 8)] &   (1 << ((k) % 8)))
#define CEILING(n,d)  ((n / d) + (n % d > 0))
unsigned int* g_breadth_first_traverse(graph_t g, vertex_t v, unsigned int* size) {
    *size = 0;
    unsigned int* ret = (unsigned int*)calloc(g_num_vertices(g), sizeof(unsigned int));
    /* we will use a bitarray to track which vertices have been visited */
    char* bitarray = (char*)calloc(CEILING(g_vertices_inserted(g), 8), sizeof(char));
    unsigned int id = g_vertex_id(v);
    SET_BIT(bitarray, id);

    int cur_pos = 0, pos = 0;
    ret[pos++] = id;

    edge_t e;
    unsigned int i;
    while (cur_pos != pos) {  /* this signifies that we have hit the end */
        vertex_t ver = g_vertex_get(g, ret[cur_pos]);
        g_iterate_edges(ver, e, i) {
            id = g_edge_dest(e);
            if (CHECK_BIT(bitarray, id) != 0)
                continue;  /* already visited */
            SET_BIT(bitarray, id);
            ret[pos++] = id;
        }
        ++cur_pos;
    }

    free(bitarray);
    *size = pos;
    return ret;
}


unsigned int* g_depth_first_traverse(graph_t g, vertex_t v, unsigned int* size) {
    *size = 0;
    unsigned int* ret = (unsigned int*)calloc(g_num_vertices(g), sizeof(unsigned int));
    char* bitarray = (char*)calloc(CEILING(g_vertices_inserted(g), 8), sizeof(char));
    unsigned int id = g_vertex_id(v);
    SET_BIT(bitarray, id);
    ret[0] = id;
    *size = *size + 1;
    __traverse_depth_first(g, ret, bitarray, size);
    free(bitarray);
    return ret;
}


static void __traverse_depth_first(graph_t g, unsigned int* res, char* bitarray, unsigned int* size) {
    vertex_t v = g_vertex_get(g, res[*size - 1]);
    // cppcheck-suppress variableScope
    edge_t e;
    unsigned int i, id;
    g_iterate_edges(v, e, i) {
        id = g_edge_dest(e);
        if (CHECK_BIT(bitarray, id) != 0)
            continue;  /* already visited */
        SET_BIT(bitarray, id);
        res[*size] = id;
        *size = *size + 1;
        __traverse_depth_first(g, res, bitarray, size);
    }
}

static void __graph_vertices_grow(graph_t g, unsigned int id) {
    /*  in parallel code, the work may have been done by another thread, so
        it should be checked one more time */
    if (id < g->_max_verts)
        return;

    /*  need to expand the verts!
        NOTE: ensure that we are growing enough to capture idx and
        not just double... */
    unsigned int new_num_verts = g->_max_verts * 2;
    while (new_num_verts < id) {
        new_num_verts *= 2;
    }

    vertex_t* tmp = (vertex_t*)realloc(g->verts, new_num_verts * sizeof(vertex_t));
    g->verts = tmp;

    /* ensure everything in the new memory space is NULL if not used */
    unsigned int i;
    for (i = g->_max_verts; i < new_num_verts; ++i) {
        g->verts[i] = NULL;
    }
    g->_max_verts = new_num_verts;
}

static void __graph_edges_grow(graph_t g, unsigned int id) {
    /*  in parallel code, the work may have been done by another thread, so
        it should be checked one more time */
    if (id < g->_max_edges)
        return;

    /* need to expand the edges! */
    unsigned int new_num_edges = g->_max_edges * 2; /* double */
    edge_t* tmp = (edge_t*)realloc(g->edges, new_num_edges * sizeof(edge_t));
    g->edges = tmp;

    /* ensure everything in the new memory space is NULL if not used */
    unsigned int i;
    for (i = g->_prev_edge_id - 1; i < new_num_edges; ++i) {
        g->edges[i] = NULL;
    }
    g->_max_edges = new_num_edges;
}

static void __vertex_edges_grow(vertex_t v_src, unsigned int outs) {
    if (outs < v_src->_max_edges)
        return;

    unsigned int new_num_edges = v_src->_max_edges * 2;  /* double */
    edge_t* tmp = (edge_t*)realloc(v_src->edges, new_num_edges * sizeof(edge_t));
    unsigned int i;
    for (i = outs; i < new_num_edges; ++i)
        tmp[i] = NULL;
    v_src->_max_edges = new_num_edges;
    v_src->edges = tmp;
    tmp = NULL;
}
