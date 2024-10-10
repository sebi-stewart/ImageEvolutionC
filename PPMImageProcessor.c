#include "PPMImageProcessor.h"
#include <stdio.h>
#include <stdlib.h>

void push_corner(Polygon* poly, Corner* p_corner){
    if (poly == NULL){
        fprintf(stderr, "push_corner: Polygon was NULL\n");
        exit(1);
    }
    if (p_corner == NULL){
        fprintf(stderr, "push_corner: Corner was NULL\n");
        exit(1);
    }
    if (p_corner->next != NULL){
        fprintf(stderr, "push_corner: Corner contained next element\n");
        exit(1);
    }
    
    p_corner->next = poly->corners;
    poly->corners = p_corner;
}

void pop_corner(Polygon* poly){
    if (poly == NULL) {
        fprintf(stderr, "pop_corner: Polygon was NULL\n");
        exit(1);
    }
    if (poly->corners == NULL) {
        fprintf(stderr, "pop_corner: No corners to pop \n");
        exit(1);
    }

    Corner* temp = poly->corners;
    poly->corners = poly->corners->next;
    free(temp);
}

void push_polygon(PPMImageProcessor* proc, Polygon* p_polygon) {
    if (proc == NULL) {
        fprintf(stderr, "push_polygon: Image Processor was NULL\n");
        exit(1);
    }
    if (p_polygon == NULL) {
        fprintf(stderr, "push_polygon: Polygon was NULL\n");
        exit(1);
    }
    if (p_polygon->next != NULL) {
        fprintf(stderr, "push_polygon: Polygon contained next element\n");
        exit(1);
    }

    p_polygon->next = proc->polygons;
    proc->polygons = p_polygon;
}

void pop_polygon(PPMImageProcessor* proc) {
    if (proc == NULL) {
        fprintf(stderr, "pop_polygon: Image Processor was NULL\n");
        exit(1);
    }

    if (proc->polygons == NULL) {
        fprintf(stderr, "pop_polygon: No polygons to pop \n");
        exit(1);
    }

    Polygon* temp = proc->polygons;
    proc->polygons = proc->polygons->next;
    free(temp);
}