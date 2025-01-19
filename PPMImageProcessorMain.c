//
// Created by Sebastian Stewart on 20/12/2024.
// Used to Run PPMImageProcessor

#include "PPMImageProcessor.h"
#include <stdlib.h>


int main(void){
    Polygon* poly = polygon_init(100, 100, 100);

    push_corner(poly, corner_init(10, 10));
    push_corner(poly, corner_init(20, 15));
    push_corner(poly, corner_init(10, 20));
    push_corner(poly, corner_init(30, 30));
    push_corner(poly, corner_init(40, 10));

    print_polygon(poly);

    Corner** del_corner = &poly->corners;
    pop_corner(del_corner);

    print_polygon(poly);

    push_corner(poly, corner_init(30, 30));
    del_corner = &poly->corners->next;
    pop_corner(del_corner);

    print_polygon(poly);

    pop_all_corners(poly);
    free(poly);

    return 0;
}
