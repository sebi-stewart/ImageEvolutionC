//
// Created by Sebastian Stewart on 01/03/2025.
//

#ifndef IMAGE_EVOLUTION_TEST_PPMIMAGE_H
#define IMAGE_EVOLUTION_TEST_PPMIMAGE_H

#include "munit/munit.h"
#include "../PPMMacros.h"
#include "../PPMImage.h"

static void*
test_setup(const MunitParameter params[], void* user_data);
static void
test_tear_down(void* fixture);

MunitResult are_images_equal(PPMImage* image1, PPMImage* image2);
MunitResult are_all_pixels_equal(PPMImage* image, PPMPixel* comparison);

MunitResult test_image_init(const MunitParameter params[], void* fixture);
MunitResult test_image_init_width_height_err(const MunitParameter params[], void* fixture);
MunitResult test_image_new_blank(const MunitParameter params[], void* fixture);
MunitResult test_image_set_pixel(const MunitParameter params[], void* fixture);
MunitResult test_image_set_pixel_image_null(const MunitParameter params[], void* fixture);
MunitResult test_image_set_pixel_image_data_null(const MunitParameter params[], void* fixture);
MunitResult test_image_set_pixel_location_out_of_bounds(const MunitParameter params[], void* fixture);
MunitResult test_image_set_pixel_colour_out_of_bounds(const MunitParameter params[], void* fixture);
MunitResult test_has_ppm_extension(const MunitParameter params[], void* fixture);
MunitResult test_has_ppm_extension_null(const MunitParameter params[], void* fixture);
MunitResult ppm_image_load_valid_file(const MunitParameter params[], void* fixture);
MunitResult ppm_image_load_invalid_extension(const MunitParameter params[], void* fixture);
MunitResult ppm_image_load_nonexistent_file(const MunitParameter params[], void* fixture);
MunitResult ppm_image_load_invalid_format(const MunitParameter params[], void* fixture);
MunitResult ppm_image_load_invalid_size(const MunitParameter params[], void* fixture);
MunitResult ppm_image_load_invalid_rgb_component(const MunitParameter params[], void* fixture);
MunitResult ppm_image_compare_identical_images(const MunitParameter params[], void* fixture);
MunitResult ppm_image_compare_different_images(const MunitParameter params[], void* fixture);


#endif //IMAGE_EVOLUTION_TEST_PPMIMAGE_H
