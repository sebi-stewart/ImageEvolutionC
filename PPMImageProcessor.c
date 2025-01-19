#include "PPMImageProcessor.h"
#include <stdio.h>
#include <stdlib.h>

Corner* corner_init(const int x, const int y) {
    if ((x < 0) || (y < 0)) {
        fprintf(stderr, "corner_init: Corner out of bounds\n");
        exit(1);
    }
    Corner* corner = malloc(sizeof(Corner));
    if (corner == NULL){
        fprintf(stderr, "corner_init: Memory allocation for corner failed\n");
        exit(1);
    }
    corner->x = x;
    corner->y = y;
    return corner;
}

void push_one_corner(Polygon* poly, Corner* p_corner){
    if (poly == NULL){
        fprintf(stderr, "push_one_corner: Polygon was NULL\n");
        exit(1);
    }
    if (p_corner == NULL){
        fprintf(stderr, "push_one_corner: Corner was NULL\n");
        exit(1);
    }
    if (p_corner->next != NULL){
        fprintf(stderr, "push_one_corner: Corner contained next element\n");
        exit(1);
    }

    p_corner->next = poly->corners;
    poly->corners = p_corner;
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
    
    p_corner->next = poly->corners;
    poly->corners = p_corner;
}

void pop_corner(Corner** head){
    if (head == NULL || *head == NULL) {
        fprintf(stderr, "pop_corner: Corner was NULL\n");
        exit(1);
    }

    Corner* next = (*head)->next;
    free(*head);
    *head = next;
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

void print_corner(Corner* corner){
    printf("[%d, %d]", corner->x, corner->y);
}

Polygon* polygon_init(const unsigned char R, const unsigned char G, const unsigned char B) {
    PPMPixel* pixel = ppm_pixel_new(R, G, B);
    Polygon* poly = malloc(sizeof(Polygon));
    if (poly == NULL){
        fprintf(stderr, "polygon_init: Memory allocation for polygon failed\n");
        exit(1);
    }

    poly->color = pixel;
    return poly;
}

void push_one_polygon(PPMImageProcessor* proc, Polygon* p_polygon) {
    if (proc == NULL) {
        fprintf(stderr, "push_one_polygon: Image Processor was NULL\n");
        exit(1);
    }
    if (p_polygon == NULL) {
        fprintf(stderr, "push_one_polygon: Polygon was NULL\n");
        exit(1);
    }
    if (p_polygon->next != NULL) {
        fprintf(stderr, "push_one_polygon: Polygon contained next element\n");
        exit(1);
    }

    p_polygon->next = proc->polygons;
    proc->polygons = p_polygon;
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

    p_polygon->next = proc->polygons;
    proc->polygons = p_polygon;
}

void pop_polygon(Polygon** head) {
    if (head == NULL || *head == NULL) {
        fprintf(stderr, "pop_polygon: No polygons to pop \n");
        exit(1);
    }

    Polygon* next = (*head)->next;
    free(*head);
    *head = next;
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

void print_polygon(Polygon* polygon){
    printf("Polygon: ");
    Corner* current_corner = polygon->corners;
    while(current_corner->next != NULL){
        print_corner(current_corner);
        current_corner = current_corner->next;
        printf(" -> ");
    }
    print_corner(current_corner);
    printf("\n");
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
        printf("Edge at ymin = %d -> ", ymin);
        while (edge != NULL) {
            printf("x = %.2f, ymax = %d, dx/dy = %.2f -> ",
                   edge->x,
                   edge->y_max,
                   edge->dx_dy);
            edge = edge->next; // Move to the next edge
        }
        printf("\n");
    }
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
    if (global_edge_table == NULL){
        fprintf(stderr, "generate_global_edge_table: Memory allocation for global_edge_table failed\n");
        exit(1);
    }
    global_edge_table->rows = (EdgeRow*)calloc(y_max + 1, sizeof(EdgeRow));
    if (global_edge_table->rows == NULL){
        fprintf(stderr, "generate_global_edge_table: Memory allocation for global_edge_table->rows failed\n");
        exit(1);
    }

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

        #ifdef DEBUG
        printf("Adding edge at y_min %d: x: %f y_max: %d dx_dy: %f\n", y_min, x_at_ymin, y_max, dx_dy);
        printf("Edge between Corner 1: %d, %d & Corner 2: %d, %d\n", current->x, current->y, next->x, next->y);
        #endif

        current = next;
    } while (current != head);

    #ifdef DEBUG
    traverse_global_edge_table(global_edge_table);
    #endif

    return global_edge_table;
}

Edge* sorted_insert(Edge* head, Edge* new_edge) {
    if (head == NULL || new_edge->x < head->x ||
        (new_edge->x == head->x && new_edge->dx_dy < head->dx_dy)) {
        new_edge->next = head;
        return new_edge;
    }
    Edge* current = head;
    while (current->next != NULL &&
           (current->next->x < new_edge->x ||
            (current->next->x == new_edge->x && current->next->dx_dy < new_edge->dx_dy))) {
        current = current->next;
    }
    new_edge->next = current->next;
    current->next = new_edge;
    return head;
}

void ppm_image_processor_draw_polygon(PPMImage* canvas, Polygon* p_polygon) {
    EdgeTable* global_edge_table = generate_global_edge_table(p_polygon);

    unsigned char R = p_polygon->color->R;
    unsigned char G = p_polygon->color->G;
    unsigned char B = p_polygon->color->B;


    Edge* active_edges = NULL;
    Edge *temp, *cur_edge;
    int parity = 0, x = 0;

    for(int ymin = 0; ymin <= global_edge_table->max_y; ymin++){
        parity = 0;
        cur_edge = global_edge_table->rows[ymin].edges;

        if (cur_edge == NULL && active_edges == NULL){
            continue;
        }

        // Populate active edge table with new edges
        while (cur_edge != NULL) {
            temp = cur_edge;
            cur_edge = cur_edge->next;
            temp->next = NULL;  // Ensure the edge's next pointer is clean
            active_edges = sorted_insert(active_edges, temp);
        }

        // Draw to canvas from active edge table
        cur_edge = active_edges;
        for (x = 0; x < canvas->x; x++){
            if (cur_edge == NULL){
                break;
            }
            if (x-parity == (int)cur_edge->x){
                if (cur_edge->next != NULL
                && cur_edge->y_max == cur_edge->next->y_max
                && (int)cur_edge->x == (int)cur_edge->next->x) {
                    // Skip one parity toggle for converging edges
                    cur_edge = cur_edge->next->next;
                    // Draw pixel for converging edges
                    ppm_image_set_pixel(canvas, x, ymin, R, G, B);
                    #ifdef DEBUG
                    printf("Skipping parity on converging edge\n");
                    #endif
                } else {
                    #ifdef DEBUG
                    printf("Toggling Parity\n");
                    #endif
                    parity = !parity;  // Toggle parity normally
                    cur_edge = cur_edge->next;
                }
            }

            if(!parity){
                continue;
            }
            #ifdef DEBUG
            printf("Edge [%d | %d] on canvas [%d | %d] \n", x, ymin, canvas->x, canvas->y);
            #endif
            ppm_image_set_pixel(canvas, x, ymin, R, G, B);
        }

        // Update active edge table (increment / decrement x)
        Edge* prev = NULL;
        cur_edge = active_edges;
        while (cur_edge != NULL){
            // Remove edge from active edges
            if (cur_edge->y_max == ymin) {
                Edge *to_free = cur_edge;
                if (prev == NULL) {
                    active_edges = cur_edge->next;
                } else {
                    prev->next = cur_edge->next;
                }
                cur_edge = cur_edge->next;
                free(to_free);
            } else {
                // Update x value
                cur_edge->x += cur_edge->dx_dy;
                prev = cur_edge;
                cur_edge = cur_edge-> next;
            }
        }
    }
    free(global_edge_table->rows);
    free(global_edge_table);
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

Corner* ppm_image_processor_copy_corners(Corner* org_corner){
    if (org_corner == NULL) {
        return NULL; // No corners to copy
    }

    Corner* cur = org_corner;
    Corner* copy = NULL;
    Corner** copy_tail = &copy;

    while(cur != NULL){
        *copy_tail = corner_init(cur->x, cur->y);
        copy_tail = &((*copy_tail)->next);

        cur = cur->next;
    }

    return copy;
}

Polygon* ppm_image_processor_copy_polygons(Polygon* org_poly){
    if (org_poly == NULL){
        return NULL; // No polygons to copy
    }
    Polygon* cur = org_poly;
    Polygon* copy = NULL;
    Polygon** copy_tail = &copy;

    while (cur != NULL){
        *copy_tail = polygon_init(cur->color->R, cur->color->G, cur->color->B);
        (*copy_tail)->corners = ppm_image_processor_copy_corners(cur->corners);

        copy_tail = &((*copy_tail)->next);
        cur = cur->next;

    }

    return copy;
}

PPMImageProcessor* ppm_image_processor_copy(PPMImageProcessor* org_proc){
    if (org_proc == NULL) {
        return NULL; // No processor to copy
    }

    int R = org_proc->background->R;
    int G = org_proc->background->G;
    int B = org_proc->background->B;
    unsigned int width = org_proc->width;
    unsigned int height = org_proc->height;

    PPMImageProcessor* new_proc = ppm_image_processor_init(R,G,B,width, height);

    new_proc->polygons = ppm_image_processor_copy_polygons(org_proc->polygons);
    if (new_proc->polygons == NULL && org_proc->polygons != NULL) {
        fprintf(stderr, "ppm_image_processor_copy: Failed to copy polygons\n");
        exit(1);
    }

    return new_proc;
}