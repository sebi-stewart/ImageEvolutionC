#ifndef _PPMIMAGE_PROCESSOR_H
#define _PPMIMAGE_PROCESSOR_H

#include "PPMImage.h"

typedef struct Corner{
    int x, y;
    struct Corner* next;
} Corner;

typedef struct Polygon{
    PPMPixel color;
    Corner* corners;
    struct Polygon* next;
} Polygon;

typedef struct {
    PPMPixel background;
    Polygon* polygons;
} PPMImageProcessor;


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
 * */
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
void pop_corner(Polygon* poly);


/*
 * This function will add a polygon to the start of a processor
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
 * */
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
void pop_polygon(PPMImageProcessor* proc);
#endif
