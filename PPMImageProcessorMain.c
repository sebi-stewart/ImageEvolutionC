//
// Created by Sebastian Stewart on 20/12/2024.
// Used to Run PPMImageProcessor

#include "PPMImageProcessorMain.h"
#include <stdio.h>
#include <stdlib.h>


int main(void){
    Polygon* poly = polygon_init(100, 100, 100);

    push_corner(poly, corner_init(10, 10));
    push_corner(poly, corner_init(20, 15));
    push_corner(poly, corner_init(10, 20));

    EdgeTable* edgeTable = generate_global_edge_table(poly);

    printf("Traversing Global Edge Table:\n");
    traverse_global_edge_table(edgeTable);

    pop_all_corners(poly);
    free(poly);


    return 0;
}
