#include "PPMImageProcessor.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    poly->next = NULL;
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
    proc->polygon_count++;
}

void pop_polygon(PPMImageProcessor* proc, Polygon* target) {

    if (proc == NULL || proc->polygons == NULL || target == NULL) {
        fprintf(stderr, "pop_polygon: No polygons to pop \n");
        return;
    }

    if (proc->polygons == target){
        Polygon* next = proc->polygons->next;
        if (proc->polygons->corners != NULL){
            fprintf(stderr, "pop_polygon: Polygon had corners, freeing would cause a memory leak\n");
            return;
        }
        free(proc->polygons);
        proc->polygons = next;
        proc->polygon_count--;
        return;
    }

    Polygon* prev = proc->polygons;
    while(prev->next != target){
        prev = prev->next;

        if (prev == NULL) {
            fprintf(stderr, "pop_polygon: Target polygon not found in the list\n");
            return;
        }
    }

    prev->next = target->next;
    if (target->corners != NULL) {
        fprintf(stderr, "pop_polygon: Polygon had corners, freeing would cause a memory leak\n");
        return;
    }
    free(target);
    proc->polygon_count--;
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
    proc->polygon_count = 0;
}

void print_polygon(Polygon* polygon){
    printf("Polygon (%d, %d, %d): ", polygon->color->R, polygon->color->G, polygon->color->B);
    Corner* current_corner = polygon->corners;
    while(current_corner->next != NULL){
        print_corner(current_corner);
        current_corner = current_corner->next;
        printf(" -> ");
    }
    print_corner(current_corner);
    printf("\n");
}

void print_image_processor(PPMImageProcessor* processor){
    printf("Processor: \n");
    Polygon* cur_polygon = processor->polygons;
    int count = 1;
    while (cur_polygon != NULL){
        printf("%d:  \t", count++);
        print_polygon(cur_polygon);
        cur_polygon = cur_polygon->next;
    }
}

PPMImageProcessor* ppm_image_processor_init(const unsigned char R, const unsigned char G, const unsigned char B,
                                            const unsigned int width, const unsigned int height) {
    PPMPixel* bg = ppm_pixel_new(R, G, B);
    PPMImageProcessor* proc = (PPMImageProcessor*)malloc(sizeof(PPMImageProcessor));
    proc->background = bg;
    proc->width = width;
    proc->height = height;
    proc->polygon_count = 0;

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
    printf("Iterating Y from %d to %d\n", edge_table->min_y, edge_table->max_y);
    printf("Iterating X from %d to %d\n", edge_table->min_x, edge_table->max_x);
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
    int y_max, y_min, x_max, x_min;
    y_max = find_max_y(head);


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

    global_edge_table->max_y = head->y;
    global_edge_table->min_y = head->y;
    global_edge_table->max_x = head->x;
    global_edge_table->min_x = head->x;

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

        x_min = (current->x < next->x) ? current->x : next->x;
        x_max = (current->x > next->x) ? current->x : next->x;

        // Calculate x at ymin
        x_at_ymin = (float) ((current->y < next->y) ? current->x : next->x);

        // Calculate inverse slope (dx/dy)
        dx_dy = (float) (next->x - current->x) / (float)(next->y - current->y);

        // Update max_y in the table
        if (y_max > global_edge_table->max_y){
            global_edge_table->max_y = y_max;
        }
        // Update min_y in the table
        if (y_min < global_edge_table->min_y){
            global_edge_table->min_y = y_min;
        }
        // Update max_x in the table
        if (x_max > global_edge_table->max_x){
            global_edge_table->max_x = x_max;
        }
        // Update min_x in the table
        if (x_min < global_edge_table->min_x){
            global_edge_table->min_x = x_min;
        }

        // Add the edge to the global edge table
        Edge* new_edge = (Edge*)malloc(sizeof(Edge));

        new_edge->x = x_at_ymin;
        new_edge->comp_x_val = (int)(x_at_ymin + (dx_dy / 2));
        new_edge->y_max = y_max;
        new_edge->dx_dy = dx_dy;
        new_edge->next = global_edge_table->rows[y_min].edges;
        global_edge_table->rows[y_min].edges = new_edge; // Prepend to list

        #ifdef DEBUG_IMAGE_SAVE
        printf("Adding edge at y_min %d: x: %f y_max: %d dx_dy: %f\n", y_min, x_at_ymin, y_max, dx_dy);
        printf("Edge between Corner 1: %d, %d & Corner 2: %d, %d\n", current->x, current->y, next->x, next->y);
        #endif

        current = next;
    } while (current != head);

    #ifdef DEBUG_IMAGE_SAVE
    traverse_global_edge_table(global_edge_table);
    #endif

    return global_edge_table;
}

Edge* sorted_insert(Edge* head, Edge* new_edge) {
    if (head == NULL || new_edge->x < head->x ||
        (new_edge->x == head->x && new_edge->dx_dy < head->dx_dy)) {
        new_edge->next = head;
#ifdef DEBUG_IMAGE_SAVE
        Edge* current = new_edge;
        printf("\tsorted_insert - inserting at the start: \n");
        while (current != NULL){
            printf("\t\tx = %f, ymax = %d, dx/dy = %f -> ", current->x, current->y_max, current->dx_dy);
            current = current->next;
        }
        printf("\n");
#endif
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

#ifdef DEBUG_IMAGE_SAVE
    current = head;
    printf("\tsorted_insert - inserting anywhere but the start: \n");
    while (current != NULL){
        printf("\t\tx = %f, ymax = %d, dx/dy = %f -> ", current->x, current->y_max, current->dx_dy);
        current = current->next;
    }
    printf("\n");
#endif
    return head;
}

void ppm_image_set_pixel_in_processor(PPMImage* image, int x, int y, unsigned char R, unsigned char G, unsigned char B){
    ppm_pixel_set_unsafe(&image->data[y * image->x + x], R, G, B);
}

void ppm_image_processor_draw_polygon_alt(PPMImage* canvas, Polygon* p_polygon, int final){
    EdgeTable* global_edge_table = generate_global_edge_table(p_polygon);
//    if (final){
//        traverse_global_edge_table(global_edge_table);
//    }
    if (global_edge_table->max_x > canvas->x){
        fprintf(stderr, "ppm_image_processor_draw_polygon_alt: GET->max_x (%d) was too large for canvas (%d)\n",
                global_edge_table->max_x, canvas->x);
        exit(1);
    }
#ifdef DEBUG_IMAGE_SAVE
    traverse_global_edge_table(global_edge_table);
#endif
    unsigned char R = p_polygon->color->R;
    unsigned char G = p_polygon->color->G;
    unsigned char B = p_polygon->color->B;

    Edge* active_edges = NULL;
    Edge *cur_edge, *temp, *prev;

    // Draw with main scanline algorithm
    for(int ymin = global_edge_table->min_y; ymin <= global_edge_table->max_y; ymin++) {

        // Get a new edge, if it is not null add it to the active edges
        cur_edge = global_edge_table->rows[ymin].edges;
        while(cur_edge != NULL){
            temp = cur_edge;
            cur_edge = cur_edge->next;
            temp->next = NULL;  // Ensure the edge's next pointer is clean
            active_edges = sorted_insert(active_edges, temp);
        }

        // Remove edges at their ymax
        prev = NULL;
        cur_edge = active_edges;
        while (cur_edge != NULL) {
            if (ymin == cur_edge->y_max) { // Remove if at it's max
                Edge *to_free = cur_edge;
                if (prev == NULL) { // If we haven't got previous it means this is the first iteration and cur_edge==active_edges
                    active_edges = cur_edge->next;
                } else { // remove the cur edge from the linked list
                    prev->next = cur_edge->next;
                }
                cur_edge = cur_edge->next;
                free(to_free);

            } else { // Continue to the next edge
                prev = cur_edge;
                cur_edge = cur_edge->next;
            }
        }

        // Loop over all pixels horizontally
        int inside_polygon = false;
        cur_edge = active_edges;
//        if(final) {
//        printf("YMin: %d ", ymin);
//        }
        for(int x = global_edge_table->min_x; x < global_edge_table->max_x && cur_edge!=NULL; x++){
//            if(final){
//                printf("%d", x == cur_edge->comp_x_val);
//            }
            if (x == cur_edge->comp_x_val){ // if the x of our current edge and our iteration match we are on a line
                inside_polygon = !inside_polygon; // We either entered or left a polygon
                if (cur_edge->dx_dy > 0) { // We add this conditional so that regressing lines or straight lines don't get printed outside of their boundaries
                    ppm_image_set_pixel(canvas, x, ymin, R, G, B);
                }
                cur_edge = cur_edge->next; // Go to the next edge

                // What about a case where two edges have basically the same x value, therefore we would never trigger leaving the polygon
                while(cur_edge != NULL && x == cur_edge->comp_x_val){ // Handle the case of two or more edges converging on one spot
                    inside_polygon = !inside_polygon; // Either leave or enter the polygon again
                    ppm_image_set_pixel(canvas, x, ymin, R, G, B); // In this case we always print the pixel on the converging edges
                    cur_edge = cur_edge->next; // Skip the current point
                }
            }


            if (inside_polygon){ // If we are inside a polygon, draw that pixel
                ppm_image_set_pixel(canvas, x, ymin, R, G, B);
//                if(final){
//                    printf("\t\tEdge [%d | %d] on canvas [%d | %d] \n", x, ymin, canvas->x, canvas->y);
//                }
            }
        }

//        if(final) {
//            printf("YMin: %d\n", ymin);
//        }
        // Update active edge table (increment / decrement x)
        for(cur_edge = active_edges; cur_edge != NULL; cur_edge = cur_edge->next){
            cur_edge->x += cur_edge->dx_dy;
            cur_edge->comp_x_val = (int)cur_edge->x;
        }
    }

    free(global_edge_table->rows);
    free(global_edge_table);
}

void ppm_image_processor_draw_polygon(PPMImage* canvas, Polygon* p_polygon) {
#ifdef DEBUG_IMAGE_SAVE
    printf("ppm_image_processor_draw_polygon - start:\n");
#endif
    EdgeTable* global_edge_table = generate_global_edge_table(p_polygon);
    if (global_edge_table->max_x > canvas->x){
        fprintf(stderr, "ppm_image_processor_draw_polygon: GET->max_x (%d) was too large for canvas (%d)\n",
                global_edge_table->max_x, canvas->x);
        exit(1);
    }

    unsigned char R = p_polygon->color->R;
    unsigned char G = p_polygon->color->G;
    unsigned char B = p_polygon->color->B;


    Edge* active_edges = NULL;
    Edge *temp, *cur_edge, *prev;
    int parity = 0, x = 0;

    // Draw with main scanline algorithm
    for(int ymin = global_edge_table->min_y; ymin <= global_edge_table->max_y; ymin++){
        parity = 0;
        cur_edge = global_edge_table->rows[ymin].edges;

        // Populate active edge table with new edges
        while (cur_edge != NULL) {
            temp = cur_edge;
            cur_edge = cur_edge->next;
            temp->next = NULL;  // Ensure the edge's next pointer is clean
            active_edges = sorted_insert(active_edges, temp);
        }

#ifdef DEBUG_IMAGE_SAVE
        if(ymin == global_edge_table->max_y){
            cur_edge = active_edges;
            printf("\tEdges at the end of the table: \n");
            while (cur_edge != NULL){
                printf("\t\tx = %f, ymax = %d, dx/dy = %f -> ", cur_edge->x, cur_edge->y_max, cur_edge->dx_dy);
                cur_edge = cur_edge->next;
            }
            printf("\n");
        }
#endif

        // Remove edges where ymin is >= ymax,
        prev = NULL;
        cur_edge = active_edges;
        while (cur_edge != NULL) {
            // Remove edge from active edges
            if (ymin >= cur_edge->y_max) {
                Edge *to_free = cur_edge;
                if (prev == NULL) {
                    active_edges = cur_edge->next;
                } else {
                    prev->next = cur_edge->next;
                }
                cur_edge = cur_edge->next;
                free(to_free);

                // Check if active edges is null now
#ifdef DEBUG_IMAGE_SAVE
                if (active_edges == NULL){
                    printf("\tActive edge was null\n");
                }
#endif
            } else {
                prev = cur_edge;
                cur_edge = cur_edge->next;
            }
        }

        // Draw to canvas from active edge table
        cur_edge = active_edges;
        for (x = global_edge_table->min_x; x < global_edge_table->max_x; x++){
            if (cur_edge == NULL){
                break;
            }

            if (x ==  cur_edge->comp_x_val){ //
                if (cur_edge->next != NULL //check if we have a next edge
                && cur_edge->y_max == cur_edge->next->y_max //
                && cur_edge->comp_x_val == cur_edge->next->comp_x_val) {
                    // Skip one parity toggle for converging edges
                    cur_edge = cur_edge->next->next;
                    // Draw pixel for converging edges
                    ppm_image_set_pixel(canvas, x, ymin, R, G, B);
#ifdef DEBUG_IMAGE_SAVE
                    printf("\t\tSkipping parity on converging edge [%d | %d]\n", x, ymin);
#endif
                    continue;
                } else {
#ifdef DEBUG_IMAGE_SAVE
                    printf("\tToggling Parity %s\n", !parity ? "on" : "off");
#endif
                    parity = !parity;  // Toggle parity normally
                    cur_edge = cur_edge->next;
                }
            }

            if(!parity){ // Skip the current edge if parity is 0 aka false
                continue;
            }
#ifdef DEBUG_IMAGE_SAVE
            printf("\t\tEdge [%d | %d] on canvas [%d | %d] \n", x, ymin, canvas->x, canvas->y);
#endif
            ppm_image_set_pixel(canvas, x, ymin, R, G, B);
        }

        // Update active edge table (increment / decrement x)
        for(cur_edge = active_edges; cur_edge != NULL; cur_edge = cur_edge->next){
            cur_edge->x += cur_edge->dx_dy;
            cur_edge->comp_x_val = (int)rintf(cur_edge->x + (cur_edge->dx_dy / 2));
        }


#ifdef DEBUG_IMAGE_SAVE
        if (ymin+1 <= global_edge_table->max_y) {
            printf("\tNext Line at y=%d \n", ymin + 1);
        }
#endif
    }


    // Free all of our leftover scanline algorithm resources
    while (active_edges != NULL){
        temp = active_edges;
        active_edges = active_edges->next;
        free(temp);
    }
    free(global_edge_table->rows);
    free(global_edge_table);
#ifdef DEBUG_IMAGE_SAVE
    printf("ppm_image_processor_draw_polygon - end:\n");
#endif
}

PPMImage* ppm_image_processor_draw_polygons(PPMImageProcessor* proc) {
    if (proc == NULL) {
        fprintf(stderr, "ppm_processor_draw_polygons: Image Processor was NULL\n");
        exit(1);
    }
    PPMPixel* bg = proc->background;
    PPMImage* canvas = ppm_image_init(proc->width, proc->height, bg->R, bg->G, bg->B);

    if (proc->polygons == NULL) {
#ifdef DEBUG_VERBOSE
        fprintf(stdout, "ppm_processor_draw_polygons: No polygons to draw \n");
#endif
        return canvas;
    }

    Polygon* poly_head = proc->polygons;
    while (poly_head != NULL) {
        ppm_image_processor_draw_polygon(canvas, poly_head);
        poly_head = poly_head->next;
    }
    return canvas;
}

PPMImage* ppm_image_processor_draw_polygons_alt(PPMImageProcessor* proc, int final) {
    if (proc == NULL) {
        fprintf(stderr, "ppm_processor_draw_polygons: Image Processor was NULL\n");
        exit(1);
    }
    PPMPixel* bg = proc->background;
    PPMImage* canvas = ppm_image_init(proc->width, proc->height, bg->R, bg->G, bg->B);

    if (proc->polygons == NULL) {
#ifdef DEBUG_VERBOSE
        fprintf(stdout, "ppm_processor_draw_polygons: No polygons to draw \n");
#endif
        return canvas;
    }

    Polygon* cur_polygon = proc->polygons;
    int i = 1;
    while (cur_polygon != NULL) {
        if (final){
            printf("Drawing %d: ", i++);
            print_polygon(cur_polygon);
        }
        ppm_image_processor_draw_polygon_alt(canvas, cur_polygon, final);
        cur_polygon = cur_polygon->next;
    }
    if (final){
        printf("\n Total Polygons %d\n", proc->polygon_count);
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
    new_proc->polygon_count = org_proc->polygon_count;

    new_proc->polygons = ppm_image_processor_copy_polygons(org_proc->polygons);
    if (new_proc->polygons == NULL && org_proc->polygons != NULL) {
        fprintf(stderr, "ppm_image_processor_copy: Failed to copy polygons\n");
        exit(1);
    }

    return new_proc;
}