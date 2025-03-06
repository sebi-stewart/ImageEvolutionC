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

#endif //IMAGE_EVOLUTION_TEST_PPMIMAGE_H
