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

int find_max_y(Corner* corners) {
    if (corners == NULL) {
        fprintf(stderr, "find_max_y: Polygon corners are NULL\n");
        exit(1);
    }

    int max_y = corners->y;
    Corner* current = corners;
    while (current != NULL) { // Loop through the circular linked list
        if (current->y > max_y) {
            max_y = current->y;
        }
        current = current->next;
    }

    return max_y;
}

EdgeTable* generate_global_edge_table(Polygon* p_polygon){
    if (p_polygon == NULL){
        fprintf(stderr, "generate_global_edge_table: Polygon was empty\n");
        exit(1);
    }

    // Count the corners and locate the head/tail of the polygon
    Corner* head = p_polygon->corners;
    int y_max = find_max_y(head);


    // Allocate memory for the global edge table
    EdgeTable* global_edge_table = (EdgeTable*)malloc(sizeof(EdgeTable));
    global_edge_table->rows = (EdgeRow*)calloc(y_max + 1, sizeof(EdgeRow));

    global_edge_table->max_y = 0;

    int y_min;
    float x_at_ymin;
    float dx_dy;

    Corner* current = head;
    do {
        Corner* next = current->next ? current->next : head; // Wrap around to head

        if (current->y == next->y) { // Ignore horizontal edges
            current = next;
            continue;
        }

        y_min = (current->y < next->y) ? current->y : next->y;
        y_max = (current->y > next->y) ? current->y : next->y;

        // Calculate x at ymin
        x_at_ymin = (float) ((current->y < next->y) ? current->x : next->x);

        // Calculate inverse slope (dx/dy)
        dx_dy = (float) (next->x - current->x) / (float)(next->y - current->y);

        // Update max_y in the table
        if (y_max > global_edge_table->max_y){
            global_edge_table->max_y = y_max;
        }

        // Add the edge to the global edge table
        Edge* new_edge = (Edge*)malloc(sizeof(Edge));

        new_edge->x = x_at_ymin;
        new_edge->y_max = y_max;
        new_edge->dx_dy = dx_dy;
        new_edge->next = global_edge_table->rows[y_min].edges;
        global_edge_table->rows[y_min].edges = new_edge; // Prepend to list

        printf("Adding edge at y_min %d: x: %f y_max: %d dx_dy: %f\n", y_min, x_at_ymin, y_max, dx_dy);
        printf("Edge between Corner 1: %d, %d & Corner 2: %d, %d\n", current->x, current->y, next->x, next->y);

        current = next;
    } while (current != head);

    return global_edge_table;
}

void traverse_global_edge_table(EdgeTable* edge_table) {
    if (edge_table == NULL) {
        printf("Edge table is NULL\n");
        return;
    }
    printf("Iterating to %d\n", edge_table->max_y);
    // Iterate through each row (ymin value) in the edge table
    for (int ymin = 0; ymin <= edge_table->max_y; ymin++) {
        EdgeRow* row = &edge_table->rows[ymin]; // Get the EdgeRow for this ymin
        Edge* edge = row->edges;
        if (edge == NULL){
            continue;
        }
        while (edge != NULL) {
            printf("Edge at ymin = %d:  x = %.2f, ymax = %d, dx/dy = %.2f\n",
                   ymin,
                   edge->x,
                   edge->y_max,
                   edge->dx_dy);
            edge = edge->next; // Move to the next edge
        }
        printf("\n");
    }
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

int main(void){
    Polygon* poly = polygon_init(100, 100, 100);
    push_corner(poly, corner_init(10, 10));
    push_corner(poly, corner_init(10, 20));
    push_corner(poly, corner_init(20, 15));

    EdgeTable* edgeTable = generate_global_edge_table(poly);

    printf("Traversing Global Edge Table:\n");
    traverse_global_edge_table(edgeTable);

    pop_all_corners(poly);
    free(poly);


    return 0;
}