//
// Created by Sebastian Stewart on 20/12/2024.
// Used to Run PPMImageProcessor

#include "PPMImageProcessor.h"

int main(void){
    Polygon* poly;
    PPMImageProcessor* new_proc = ppm_image_processor_init(152, 109, 97, 200, 200);

    poly = polygon_init(104, 112, 163);
    push_corner(poly, corner_init(149, 84));
    push_corner(poly, corner_init(29, 12));
    push_corner(poly, corner_init(173, 124));
    push_one_polygon(new_proc, poly);

    poly = polygon_init(24, 88, 16);
    push_corner(poly, corner_init(5, 67));
    push_corner(poly, corner_init(40, 101));
    push_corner(poly, corner_init(146, 0));
    push_one_polygon(new_proc, poly);

    poly = polygon_init(147, 153, 130);
    push_corner(poly, corner_init(71, 102));
    push_corner(poly, corner_init(71, 5));
    push_corner(poly, corner_init(145, 111));
    push_one_polygon(new_proc, poly);

    poly = polygon_init(152, 1, 135);
    push_corner(poly, corner_init(88, 72));
    push_corner(poly, corner_init(143, 146));
    push_corner(poly, corner_init(22, 161));
    push_one_polygon(new_proc, poly);

    PPMImage* alt_image = ppm_image_processor_draw_polygons_alt(new_proc, true);
    print_image_processor(new_proc);

    ppm_image_save(alt_image, "test2.ppm");

    ppm_image_processor_free(new_proc);
    ppm_image_del(alt_image);

    return 0;
}
