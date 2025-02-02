//
// Created by Sebastian Stewart on 20/12/2024.
// Used to Run PPMImageProcessor

#include "PPMImageProcessor.h"

int main(void){
    Polygon* poly = polygon_init(100, 100, 100);

    push_corner(poly, corner_init(0, 0));
    push_corner(poly, corner_init(4, 0));
    push_corner(poly, corner_init(4, 4));

    print_polygon(poly);

    Polygon* poly2 = polygon_init(255, 0, 0);

    push_corner(poly2, corner_init(7, 7));
    push_corner(poly2, corner_init(7, 0));
    push_corner(poly2, corner_init(2, 4));

    print_polygon(poly2);

    Polygon* poly3 = polygon_init(255, 255, 0);

    push_corner(poly3, corner_init(0, 8));
    push_corner(poly3, corner_init(8, 8));
    push_corner(poly3, corner_init(0, 3));

    print_polygon(poly3);

    PPMImageProcessor* new_proc = ppm_image_processor_init(0, 0, 0, 8, 8);
    push_one_polygon(new_proc, poly);
    push_one_polygon(new_proc, poly2);
    push_one_polygon(new_proc, poly3);
    PPMImage* new_image = ppm_image_processor_draw_polygons(new_proc);
    ppm_image_set_pixel(new_image, 1, 7, 100, 100, 100);
    ppm_image_set_pixel(new_image, 3, 7, 100, 100, 100);
    ppm_image_set_pixel(new_image, 5, 7, 100, 100, 100);
    ppm_image_set_pixel(new_image, 7, 7, 100, 100, 100);
    ppm_image_save(new_image, "test.ppm");

    ppm_image_processor_free(new_proc);
    ppm_image_del(new_image);

    return 0;
}
