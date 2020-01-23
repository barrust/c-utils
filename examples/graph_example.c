
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "../src/graph.h"


typedef struct city {
    char* name;
    double latitude;
    double longitude;
} city;

typedef struct highway {
    int miles;
} highway;


/* Private Functions */
static void     __add_city_as_vertex(graph_t g, char* name, double latitude, double longitude);  /* we get to always make N and W.*/
static void     __print_city(city* c);
static void     __free_city(city* c);
static void     __add_highway_as_edge(graph_t g, int src, int dest, int miles);
static char*    __str_duplicate(const char* s);

unsigned int*   __breadth_first_search(graph_t g, vertex_t start);



int main(int argc, char const *argv[]) {
    bool verbose = false;

    if (argc == 2 && strcmp(argv[1], "-v") == 0) {
        verbose = true;
    }

    graph_t g = g_init();
    vertex_t v;
    unsigned int i;

    /* add the vertices */
    __add_city_as_vertex(g, "New York", 40.7128, 74.0060);
    __add_city_as_vertex(g, "Washington D.C.", 38.9072, 77.0369);
    __add_city_as_vertex(g, "Cleveland", 41.4993, 81.6944);
    __add_city_as_vertex(g, "Detroit", 42.3314, 83.0458);
    __add_city_as_vertex(g, "Atlanta", 33.7490, 84.3880);
    __add_city_as_vertex(g, "St. Louis", 38.6270, 90.1994);
    __add_city_as_vertex(g, "Dallas", 32.7767, 96.7970);
    __add_city_as_vertex(g, "Salt Lake", 40.7608, 111.8910);
    __add_city_as_vertex(g, "Pheonix", 33.4484, 112.0740);
    __add_city_as_vertex(g, "Las Vegas", 36.1699, 115.1398);
    __add_city_as_vertex(g, "Los Angeles", 34.0522, 118.2437);
    __add_city_as_vertex(g, "San Fransisco", 37.7749, 122.4194);

    /* add edges between the vertices */
    __add_highway_as_edge(g, 0, 1, 227);    /* NY - DC */
    __add_highway_as_edge(g, 1, 0, 227);    /* DC - NY */
    __add_highway_as_edge(g, 1, 2, 371);    /* DC - Cleveland */
    __add_highway_as_edge(g, 1, 4, 639);    /* DC - Atlanta */
    __add_highway_as_edge(g, 4, 1, 639);    /* Atlanta - DC */
    __add_highway_as_edge(g, 2, 3, 168);    /* Cleveland - Detroit */
    __add_highway_as_edge(g, 2, 4, 557);    /* Cleveland - St. Louis */
    __add_highway_as_edge(g, 4, 5, 630);    /* St. Louis - Dallas */
    __add_highway_as_edge(g, 5, 7, 1064);   /* Dallas - Pheonix */
    __add_highway_as_edge(g, 7, 8, 300);    /* Pheonix - LV */
    __add_highway_as_edge(g, 7, 9, 372);    /* Pheonix - LA */
    __add_highway_as_edge(g, 9, 7, 372);    /* LA - Pheonix */
    __add_highway_as_edge(g, 9, 10, 381);   /* LA - SF */
    __add_highway_as_edge(g, 8, 6, 420);    /* LV - SLC */
    __add_highway_as_edge(g, 6, 10, 735);   /* SLC - SF */

    printf("Breadth First Search; starting at New York: \n");
    unsigned int size;
    unsigned int* bfs = g_breadth_first_traverse(g, g_vertex_get(g, 0), &size);
    printf("Size: %u\n", size);
    for (i = 0; i < size; i++) {
        printf("%d, ", bfs[i]);
    }
    printf("\n\n\n");

    for (i = 0; i < size; i++) {
        v = g_vertex_get(g, bfs[i]);
        city* c = g_vertex_metadata(v);
        __print_city(c);
    }

    /* free things */
    g_iterate_vertices(g, v, i) {
        city* c = g_vertex_metadata(v);
        /*__print_city(c); */
        __free_city(c);
        g_vertex_metadata_update(v, NULL);
    }
    g_free(g);
    free(bfs);

    if (verbose == true)
        printf("Completed!\n");

    return 0;
}


/* PRIVATE FUNCTIONS */
static void __add_city_as_vertex(graph_t g, char* name, double latitude, double longitude) {
    city* c = calloc(1, sizeof(city));

    c->name = __str_duplicate(name);
    c->latitude = latitude;
    c->longitude = longitude;
    g_vertex_add(g, c);
}

static void __print_city(city* c) {
    printf("City Name: %s\tLatitude: %f°N\tLongitude: %f°W\n", c->name, c->latitude, c->longitude);
}

static void __free_city(city* c) {
    free(c->name);
    c->latitude = 0.0;
    c->longitude = 0.0;
    free(c);
}

static void __add_highway_as_edge(graph_t g, int src, int dest, int miles) {
    highway* h = calloc(1, sizeof(highway));
    h->miles = miles;
    g_edge_add(g, src, dest, h);
}

static char* __str_duplicate(const char* s) {
    size_t len = strlen(s);
    char* buf = malloc((len + 1) * sizeof(char));
    if (buf == NULL)
        return NULL;
    strcpy(buf, s);
    buf[len] = '\0';
    return buf;
}
