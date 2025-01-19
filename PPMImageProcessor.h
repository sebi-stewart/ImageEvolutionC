#ifndef IMAGE_EVOLUTION_PPM_IMAGE_PROCESSOR_H
#define IMAGE_EVOLUTION_PPM_IMAGE_PROCESSOR_H

#include "PPMImage.h"


#define DEBUG
#define DEBUG_VERBOSE

typedef struct Corner{
    int x, y;
    struct Corner* next;
} Corner;

typedef struct Polygon{
    PPMPixel* color;
    Corner* corners;
    struct Polygon* next;
} Polygon;

typedef struct {
    PPMPixel* background;
    Polygon* polygons;
    unsigned int width, height;
} PPMImageProcessor;

typedef struct Edge{
    int y_max;
    float x, dx_dy;
    struct Edge* next;
} Edge;

typedef struct EdgeRow {
    Edge* edges;
} EdgeRow;

typedef struct {
    int max_y;
    EdgeRow* rows;
} EdgeTable;

/*
 * This function will create a corner and return it
 *
 * Parameters:
 * x - X position
 * y - Y position
 *
 * Return:
 * Pointer to the newly created corner
 */
Corner* corner_init(int x, int y);

/*
 * This function will add a corner to the start of a polygon
 *
 * Parameters:
 * poly - Pointer to the polygon
 * p_corner - Pointer to a corner to be added 
 *
 * Return:
 * Void - NULL
 *
 * Errors:
 * head is NULL
 * p_corner is NULL
 * (more than one corner is being pushed)
 * */
void push_one_corner(Polygon* poly, Corner* p_corner);
void push_corner(Polygon* poly, Corner* p_corner);

/*
 * This function will remove a corner from the start of a polygon
 * 
 * Parameters:
 * poly - Pointer to the polygon
 *
 * Return:
 * Void (we won't ever need the popped corner)
 *
 * Errors:
 * head is NULL
 * */
void pop_corner(Corner** head);
void pop_all_corners(Polygon* poly);

/*
 * This function will create a blank polygon
 */
Polygon* polygon_init(unsigned char R, unsigned char G, unsigned char B);

/*
 * This function will add a single polygon to the start of a processor
 *
 * Parameters:
 * proc - Pointer to the pointer of the image processor
 * p_polygon - Pointer of the polygon to be added
 * Return:
 * Void - NULL
 *
 * Errors:
 * head is NULL
 * polygon is NULL
 * (more than one polygon is to be added)
 * */
void push_one_polygon(PPMImageProcessor* proc, Polygon* p_polygon);
void push_polygon(PPMImageProcessor* proc, Polygon* p_polygon);

/*
 * This function will remove a polygon from the start of the processor
 *
 * Parameters:
 * proc - Pointer to the pointer of the image processor
 *
 * Return:
 * Void (we won't ever need the popped polygon)
 *
 * Errors:
 * head is NULL
 * */
void pop_polygon(Polygon** head);
void pop_all_polygons(PPMImageProcessor* proc);
void print_polygon(Polygon* polygon);


PPMImageProcessor* ppm_image_processor_init(unsigned char R, unsigned char G, unsigned char B,
                                            unsigned int width, unsigned int height);

void ppm_image_processor_free(PPMImageProcessor* proc);
int find_max_y(Corner* corners);
void traverse_global_edge_table(EdgeTable* edge_table);
EdgeTable* generate_global_edge_table(Polygon* p_polygon);
Edge* sorted_insert(Edge* head, Edge* new_edge);
void ppm_image_processor_draw_polygon(PPMImage* canvas, Polygon* p_polygon);
PPMImage* ppm_image_processor_draw_polygons(PPMImageProcessor* proc);
PPMImageProcessor* ppm_image_processor_copy(PPMImageProcessor* org_proc);

#endif
