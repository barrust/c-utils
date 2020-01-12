#ifndef BARRUST_GRAPH_H__
#define BARRUST_GRAPH_H__

typedef struct __graph* graph_t;
typedef struct __vertex_node* vertex_t;
typedef struct __edge_node* edge_t;


#define GRAPH_SUCCESS    0
#define GRAPH_FAILURE   -1


graph_t g_init();
graph_t g_init_alt(unsigned int size);

int g_free(graph_t g);
int g_free_alt(graph_t g, bool free_metadata);

unsigned int g_num_vertices(graph_t g);

unsigned int g_num_edges(graph_t g);

vertex_t g_add_vertex(graph_t g, void* metadata);
vertex_t g_vertex_add(graph_t g, void* metadata);  /* which one is better? */

vertex_t g_remove_vertex(graph_t g, unsigned int id);
vertex_t g_remove_vertex_alt(graph_t g, unsigned int id, bool free_edge_metadata);

vertex_t g_get_vertex(graph_t g, unsigned int id);

edge_t g_add_edge(graph_t g, unsigned int src, unsigned int dest, void* metadata);

edge_t g_remove_edge(graph_t g, unsigned int id);

edge_t g_get_edge(graph_t g, unsigned int id);


/*******************************************************************************
*   Vertex Properties / Functions
*******************************************************************************/
unsigned int g_vertex_id(vertex_t v);

unsigned int g_vertex_num_edges_in(vertex_t v);

unsigned int g_vertex_num_edges_out(vertex_t v);

void* g_vertex_metadata(vertex_t v);

void g_vertex_free(vertex_t v);
void g_vertex_free_alt(vertex_t v, bool free_metadata);

/*******************************************************************************
*   Edge Properties / Functions
*******************************************************************************/

unsigned int g_edge_id(edge_t e);

unsigned int g_edge_src(edge_t e);

unsigned int g_edge_dest(edge_t e);

void* g_edge_metadata(edge_t e);

void g_edge_free(edge_t e);
void g_edge_free_alt(edge_t e, bool free_metadata);

#endif  /* BARRUST_GRAPH_H__ */
