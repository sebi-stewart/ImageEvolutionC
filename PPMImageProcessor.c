#include "PPMImageProcessor.h"
#include <stdio.h>
#include <stdlib.h>

Corner* corner_init(const int x, const int y) {
    if ((x < 0) || (y < 0)) {
        fprintf(stderr, "init_corner: Corner out of bounds\n");
    }
    Corner* corner = malloc(sizeof(Corner));
    corner->x = x;
    corner->y = y;
    return corner;
}

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

void pop_all_corners(Polygon* poly) {
    if (poly == NULL) {
        fprintf(stderr, "pop_all_corners: Polygon was NULL\n");
        exit(1);
    }
    if (poly->corners == NULL) {
        fprintf(stderr, "pop_all_corners: No corners to pop \n");
        exit(1);
    }

    do {
        Corner* temp = poly->corners;
        poly->corners = poly->corners->next;
        free(temp);
    } while (poly->corners != NULL);
}

Polygon* polygon_init(const unsigned char R, const unsigned char G, const unsigned char B) {
    PPMPixel* pixel = ppm_pixel_new(R, G, B);
    Polygon* poly = malloc(sizeof(Polygon));

    poly->color = pixel;
    return poly;
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

    if (temp->corners != NULL) {
        pop_all_corners(temp);
    }
    free(temp);
}

void pop_all_polygons(PPMImageProcessor* proc) {
    if (proc == NULL) {
        fprintf(stderr, "pop_all_polygons: Image Processor was NULL\n");
        exit(1);
    }
    if (proc->polygons == NULL) {
        fprintf(stderr, "pop_all_polygons: No polygons to pop \n");
        exit(1);
    }

    do {
        Polygon* temp = proc->polygons;
        proc->polygons = proc->polygons->next;
        if (temp->corners != NULL) {
            pop_all_corners(temp);
        }
        free(temp);
    } while (proc->polygons != NULL);
}

PPMImageProcessor* ppm_image_processor_init(const unsigned char R, const unsigned char G, const unsigned char B,
                                            const unsigned int width, const unsigned int height) {
    PPMPixel* bg = ppm_pixel_new(R, G, B);
    PPMImageProcessor* proc = (PPMImageProcessor*)malloc(sizeof(PPMImageProcessor));
    proc->background = bg;
    proc->width = width;
    proc->height = height;

    return proc;
}

void ppm_image_processor_free(PPMImageProcessor* proc) {
    if (proc == NULL) {
        fprintf(stderr, "ppm_image_processor_free: Image Processor was NULL\n");
        exit(1);
    }
    if (proc->polygons != NULL) {
        pop_all_polygons(proc);
    }
    free(proc);
}

EdgeTable* generate_global_edge_table(Polygon* p_polygon){
    if (p_polygon == NULL){
        fprintf(stderr, "generate_global_edge_table: Polygon was empty\n");
        exit(1);
    }
    int corners = 1;
    Corner* head = p_polygon->corners;
    Corner* tail;
    for(tail = p_polygon->corners; tail->next != NULL; tail = tail->next) {
        corners++;
    }

    int vertices[corners][2];
    int i;
    for(tail = p_polygon->corners; tail->next != NULL; tail = tail->next){
        vertices[i][0] = tail->x - tail->next->x;
        vertices[i][1] = tail->y - tail->next->y;
        i++;
    }
    vertices[corners-1][0] = tail->x - head->x;
    vertices[corners-1][1] = tail->y - head->y;

    EdgeTable* global_edge_table = (EdgeTable*)malloc(sizeof(EdgeTable));
    global_edge_table->rows = (EdgeRow*)malloc(sizeof(EdgeRow) * corners);
    EdgeRow* row = (EdgeRow*)malloc(sizeof(EdgeRow));

    for(i=0; i < corners; i++){
        row->
    }
    return global_edge_table
}


void ppm_image_processor_draw_polygon(PPMImage* canvas, Polygon* p_polygon) {

}

PPMImage* ppm_image_processor_draw_polygons(PPMImageProcessor* proc) {
    if (proc == NULL) {
        fprintf(stderr, "ppm_processor_draw_polygons: Image Processor was NULL\n");
        exit(1);
    }
    if (proc->polygons == NULL) {
        fprintf(stderr, "ppm_processor_draw_polygons: No polygons to draw \n");
        exit(1);
    }
    PPMPixel* bg = proc->background;
    PPMImage* canvas = ppm_image_init(proc->width, proc->height, bg->R, bg->G, bg->B);
    Polygon* poly_head = proc->polygons;
    while (poly_head != NULL) {
        ppm_image_processor_draw_polygon(canvas, poly_head);
        poly_head = poly_head->next;
    }
    return canvas;
}