#ifndef _PPMIMAGE_PROCESSER_H
#define _PPMIMAGE_PROCESSER_H

#include "PPMImage.h"

typedef struct {
    int x, y;
} Corner;

typedef struct {
    PPMPixel color;
    Corner* corners;
    size_t corner_count;
} Polygon;

typedef struct {
    PPMPixel background;
    Polygon* polygons;
    size_t polygon_count;
} PPMImageProcessor;


#endif
