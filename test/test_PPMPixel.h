//
// Created by Sebastian Stewart on 23/02/2025.
//

#ifndef IMAGE_EVOLUTION_TEST_PPMPIXEL_H
#define IMAGE_EVOLUTION_TEST_PPMPIXEL_H

#include "munit/munit.h"

static void*
test_setup(const MunitParameter params[], void* user_data);
static void
test_tear_down(void* fixture);

MunitResult test_pixel_set_overflow(const MunitParameter params[], void* fixture);
MunitResult test_pixel_set_negative(const MunitParameter params[], void* fixture);
MunitResult test_pixel_set_regular(const MunitParameter params[], void* fixture);
MunitResult test_pixel_copy_regular(const MunitParameter params[], void* fixture);
MunitResult test_pixel_copy_no_first_pointer(const MunitParameter params[], void* fixture);
MunitResult test_pixel_copy_no_second_pointer(const MunitParameter params[], void* fixture);
MunitResult test_pixel_copy_both_pointer_equal(const MunitParameter params[], void* fixture);
MunitResult test_pixel_copy_both_pointer_null(const MunitParameter params[], void* fixture);
MunitResult test_pixel_compare_one_pointer_is_null(const MunitParameter params[], void* fixture);
MunitResult test_pixel_compare_regular(const MunitParameter params[], void* fixture);
MunitResult test_pixel_to_string_regular(const MunitParameter params[], void* fixture);
MunitResult test_pixel_to_string_regular_second(const MunitParameter params[], void* fixture);
MunitResult test_pixel_to_string_error_no_pointer(const MunitParameter params[], void* fixture);

#endif //IMAGE_EVOLUTION_TEST_PPMPIXEL_H
