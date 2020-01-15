#ifndef BARRUST_GRAPH_H__
#define BARRUST_GRAPH_H__

typedef struct __graph* graph_t;
typedef struct __vertex_node* vertex_t;
typedef struct __edge_node* edge_t;


#define GRAPH_SUCCESS    0
#define GRAPH_FAILURE   -1

/*  Initialize the graph either using the default start size or based on the
    passed in size parameter */
graph_t g_init();
graph_t g_init_alt(unsigned int size);

/*  Free the graph and all edges & verticies; defaults to free'ing the metadata
    property for both. Use the g_free_alt version if the metadata is not
    malloc'd memory */
int g_free(graph_t g);
int g_free_alt(graph_t g, bool free_metadata);

/*  Return the number of verticies currently in the graph */
unsigned int g_num_vertices(graph_t g);

/*  Return the total number of verticies ever inserted into the graph
    NOTE: likely only needed if writing ones own iteration loop */
unsigned int g_verticies_inserted(graph_t g);

/*  Return the number of edges in the graph */
unsigned int g_num_edges(graph_t g);


/*  Insert a new vertex intot he graph with the provided metadata; the
    vertex will be assigned an id (based on the order it is added) that can be
    used to quickly retrieve it */
vertex_t g_vertex_add(graph_t g, void* metadata);

/*  Remove the vertex from the graph, returning it
    NOTE: It is up to the caller to free the memory using g_vertex_free()
    NOTE: Default is to free all memory of those edges attached; use the
          alt version if the memory is not alloc'd */
vertex_t g_vertex_remove(graph_t g, unsigned int id);
vertex_t g_vertex_remove_alt(graph_t g, unsigned int id, bool free_edge_metadata);

/*  Retrieve a vertex based on it's assigned identifier */
vertex_t g_vertex_get(graph_t g, unsigned int id);

/*  Add an edge between the source (src) vertex to the destination vertex
    (dest) with the provided metadata. The edge is assigned an id for quick
    retrieval */
edge_t g_edge_add(graph_t g, unsigned int src, unsigned int dest, void* metadata);

/*  Remove an edge from the graph based on it's assigned identifier */
edge_t g_edge_remove(graph_t g, unsigned int id);

/*  Retrieve an edge based on it's assigned identifier */
edge_t g_edge_get(graph_t g, unsigned int id);


/*******************************************************************************
*   Vertex Properties / Functions
*******************************************************************************/

/*  Get the id of the provided vertex */
unsigned int g_vertex_id(vertex_t v);

/*  Get the number of edges into the provided vertex */
unsigned int g_vertex_num_edges_in(vertex_t v);

/*  Get the number of edges out of the provided vertex */
unsigned int g_vertex_num_edges_out(vertex_t v);

/*  Get the number metadata of the provided vertex */
void* g_vertex_metadata(vertex_t v);

/*  Free the provided vertex; defaults to calling free on the metadata; use
    the g_vertex_free_alt() and set free_metadata to false to not */
void g_vertex_free(vertex_t v);
void g_vertex_free_alt(vertex_t v, bool free_metadata);

/*  Get edge idx from for the provided vertex; this is useful when one needs
    to iterate over the edges that have the vertex as its source */
edge_t g_vertex_edge(vertex_t v, unsigned int idx);

/*******************************************************************************
*   Edge Properties / Functions
*******************************************************************************/

/*  Get the assigned id for the provided edge */
unsigned int g_edge_id(edge_t e);

/* Get the source vertex id for the provided edge */
unsigned int g_edge_src(edge_t e);

/* Get the destination vertex id for the provided edge */
unsigned int g_edge_dest(edge_t e);

/* Get the metadata for the provided edge */
void* g_edge_metadata(edge_t e);

/*  Free the provided edge; defaults to calling free on the metadata; use
    the g_edge_free_alt() and set free_metadata to false to not */
void g_edge_free(edge_t e);
void g_edge_free_alt(edge_t e, bool free_metadata);

/*  Macro to easily iterate over all the verticies in the graph
    NOTE:
        g   -   The graph
        v   -   A vertex_t pointer that will hold the next vertex
        i   -   An unsigned int that will be modified for the loop */
#define g_iterate_verticies(g, v, i)    for (i = 0; i < g_verticies_inserted(g); i++) if ((v = g_vertex_get(g, i)) != NULL)

/*  Macro to easily iterate over the edges from a vertex
    NOTE:
        v   -   The vertex
        e   -   An edge_t pointer that will hold the edges in the loop
        i   -   An unsigned int that will be modified during the loop */
#define g_iterate_edges(v, e, i)        for (i = 0; i < g_vertex_num_edges_out(v); i++) if ((e = g_vertex_edge(v, i)) != NULL)


#endif  /* BARRUST_GRAPH_H__ */
