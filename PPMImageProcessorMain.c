//
// Created by Sebastian Stewart on 20/12/2024.
// Used to Run PPMImageProcessor

#include "PPMImageProcessor.h"

int main(void){
    Polygon* poly;
    PPMImageProcessor* new_proc = ppm_image_processor_init(0, 0, 0, 8, 8);

    poly = polygon_init(0, 0, 255);


    push_corner(poly, corner_init(8, 0));
    push_corner(poly, corner_init(0, 4));
    push_corner(poly, corner_init(8, 8));

    print_polygon(poly);
    push_one_polygon(new_proc, poly);

    poly = polygon_init(255, 0, 0);


    push_corner(poly, corner_init(0, 0));
    push_corner(poly, corner_init(0, 4));
    push_corner(poly, corner_init(8, 0));

    print_polygon(poly);
    push_one_polygon(new_proc, poly);
    
    poly = polygon_init(0, 255, 0);

    push_corner(poly, corner_init(0, 4));
    push_corner(poly, corner_init(0, 8));
    push_corner(poly, corner_init(8, 8));

    print_polygon(poly);
    push_one_polygon(new_proc, poly);

    PPMImage* org_image = ppm_image_processor_draw_polygons(new_proc);
    ppm_image_set_pixel_overlay(org_image, 1, 7, 255, 255, 255, 150);
    ppm_image_set_pixel_overlay(org_image, 3, 7, 255, 255, 255, 150);
    ppm_image_set_pixel_overlay(org_image, 5, 7, 255, 255, 255, 150);
    ppm_image_set_pixel_overlay(org_image, 7, 7, 255, 255, 255, 150);
    ppm_image_save(org_image, "test.ppm");

    PPMImage* alt_image = ppm_image_processor_draw_polygons_alt(new_proc);
    ppm_image_set_pixel_overlay(alt_image, 1, 7, 255, 255, 255, 150);
    ppm_image_set_pixel_overlay(alt_image, 3, 7, 255, 255, 255, 150);
    ppm_image_set_pixel_overlay(alt_image, 5, 7, 255, 255, 255, 150);
    ppm_image_set_pixel_overlay(alt_image, 7, 7, 255, 255, 255, 150);
    ppm_image_save(alt_image, "test2.ppm");

    ppm_image_processor_free(new_proc);
    ppm_image_del(org_image);
    ppm_image_del(alt_image);

    return 0;
}
