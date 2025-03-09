//
// Created by Sebastian Stewart on 23/02/2025.
//

#include "test_PPMPixel.h"
#include "../PPMPixel.h"
#include "munit/munit.h"

MunitTest ppm_pixel_tests[] = {
        { "/test-pixel-set-overflow", test_pixel_set_overflow, test_pixel_setup, test_pixel_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-pixel-set-negative", test_pixel_set_negative, test_pixel_setup, test_pixel_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-pixel-set-regular", test_pixel_set_regular, test_pixel_setup, test_pixel_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-pixel-copy-regular", test_pixel_copy_regular, test_pixel_setup, test_pixel_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-pixel-copy-no-first-pointer", test_pixel_copy_no_first_pointer, test_pixel_setup, test_pixel_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-pixel-copy-no-second-pointer", test_pixel_copy_no_second_pointer, test_pixel_setup, test_pixel_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-pixel-copy-both-pointers-equal", test_pixel_copy_both_pointer_equal, test_pixel_setup, test_pixel_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-pixel-copy-both-pointers-null", test_pixel_copy_both_pointer_null, test_pixel_setup, test_pixel_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-pixel-compare-one-pointer-is-null", test_pixel_compare_one_pointer_is_null, test_pixel_setup, test_pixel_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-pixel-compare-regular", test_pixel_compare_regular, test_pixel_setup, test_pixel_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-pixel-to-string-regular", test_pixel_to_string_regular, test_pixel_setup, test_pixel_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-pixel-to-string-regular-second", test_pixel_to_string_regular_second, test_pixel_setup, test_pixel_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-pixel-to-string-error-no-pointer", test_pixel_to_string_error_no_pointer, test_pixel_setup, test_pixel_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite ppm_pixel_suite = {"/PPMPixel-tests", ppm_pixel_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE};

static void*
test_pixel_setup(const MunitParameter params[], void* user_data) {
    return ppm_pixel_new(0, 0, 0);
}

static void
test_pixel_tear_down(void* fixture) {
    free(fixture);
}

MunitResult test_pixel_set_overflow(const MunitParameter params[], void* fixture){
    PPMPixel* pixel = (PPMPixel*) fixture;
    ppm_pixel_set(pixel, 500, 500, 500);
    munit_assert_int(pixel->R, ==, 255);
    munit_assert_int(pixel->G, ==, 255);
    munit_assert_int(pixel->B, ==, 255);
    return MUNIT_OK;
}

MunitResult test_pixel_set_negative(const MunitParameter params[], void* fixture){
    PPMPixel* pixel = (PPMPixel*) fixture;
    ppm_pixel_set(pixel, -1, -1, -1);
    munit_assert_int(pixel->R, ==, 0);
    munit_assert_int(pixel->G, ==, 0);
    munit_assert_int(pixel->B, ==, 0);
    return MUNIT_OK;
}

MunitResult test_pixel_set_regular(const MunitParameter params[], void* fixture){
    PPMPixel* pixel = (PPMPixel*) fixture;
    ppm_pixel_set(pixel, 128, 128, 128);
    munit_assert_int(pixel->R, ==, 128);
    munit_assert_int(pixel->G, ==, 128);
    munit_assert_int(pixel->B, ==, 128);
    return MUNIT_OK;
}

MunitResult test_pixel_copy_regular(const MunitParameter params[], void* fixture){
    PPMPixel* pixel = (PPMPixel*) fixture;
    ppm_pixel_set(pixel, 128, 128, 128);
    PPMPixel* pixel2 = (PPMPixel*) malloc(sizeof(PPMPixel));
    PPMPixel* pixel3 = ppm_pixel_copy(pixel, pixel2);
    munit_assert_int(pixel3->R, ==, 128);
    munit_assert_int(pixel3->G, ==, 128);
    munit_assert_int(pixel3->B, ==, 128);
    munit_assert_memory_equal(sizeof(PPMPixel), pixel, pixel3);
    munit_assert_ptr_not_equal(pixel, pixel3);
    munit_assert_ptr_equal(pixel2, pixel3);


    free(pixel2);
    return MUNIT_OK;
}

MunitResult test_pixel_copy_no_first_pointer(const MunitParameter params[], void* fixture){
    PPMPixel* pixel = (PPMPixel*) fixture;
    ppm_pixel_set(pixel, 128, 128, 128);
    PPMPixel* pixel2 = ppm_pixel_copy(NULL, pixel);
    munit_assert_int(pixel2->R, ==, 128);
    munit_assert_int(pixel2->G, ==, 128);
    munit_assert_int(pixel2->B, ==, 128);
    munit_assert_memory_equal(sizeof(PPMPixel), pixel, pixel2);
    munit_assert_ptr_not_equal(pixel, pixel2);

    free(pixel2);
    return MUNIT_OK;
}

MunitResult test_pixel_copy_no_second_pointer(const MunitParameter params[], void* fixture){
    PPMPixel* pixel = (PPMPixel*) fixture;
    ppm_pixel_set(pixel, 128, 128, 128);
    PPMPixel* pixel2 = ppm_pixel_copy(pixel, NULL);
    munit_assert_int(pixel2->R, ==, 128);
    munit_assert_int(pixel2->G, ==, 128);
    munit_assert_int(pixel2->B, ==, 128);
    munit_assert_memory_equal(sizeof(PPMPixel), pixel, pixel2);
    munit_assert_ptr_not_equal(pixel, pixel2);

    free(pixel2);
    return MUNIT_OK;
}

MunitResult test_pixel_copy_both_pointer_equal(const MunitParameter params[], void* fixture){
    PPMPixel* pixel = (PPMPixel*) fixture;
    ppm_pixel_set(pixel, 128, 128, 128);
    PPMPixel* pixel2 = ppm_pixel_copy(pixel, pixel);
    munit_assert_int(pixel2->R, ==, 128);
    munit_assert_int(pixel2->G, ==, 128);
    munit_assert_int(pixel2->B, ==, 128);
    munit_assert_memory_equal(sizeof(PPMPixel), pixel, pixel2);
    munit_assert_ptr_equal(pixel, pixel2);
    return MUNIT_OK;
}

MunitResult test_pixel_copy_both_pointer_null(const MunitParameter params[], void* fixture){
    PPMPixel* pixel2 = ppm_pixel_copy(NULL, NULL);
    munit_assert_null(pixel2);
    return MUNIT_OK;
}

MunitResult test_pixel_compare_one_pointer_is_null(const MunitParameter params[], void* fixture){
    PPMPixel* pixel = (PPMPixel*) fixture;
    PPMPixel* pixel2 = NULL;
    int result = ppm_pixel_compare(pixel, pixel2);
    munit_assert_int(result, ==, -1);
    return MUNIT_OK;
}

MunitResult test_pixel_compare_regular(const MunitParameter params[], void* fixture){
    PPMPixel* pixel = (PPMPixel*) fixture;
    PPMPixel* pixel2 = ppm_pixel_new(50, 50, 0);
    int result = ppm_pixel_compare(pixel, pixel2);
    munit_assert_int(result, ==, 100);
    result = ppm_pixel_compare(pixel2, pixel);
    munit_assert_int(result, ==, 100);
    return MUNIT_OK;
}

MunitResult test_pixel_to_string_regular(const MunitParameter params[], void* fixture){
    PPMPixel* pixel = (PPMPixel*) fixture;
    char* result = ppm_pixel_to_string(pixel);
    munit_assert_string_equal(result, "000 - 000 - 000");
    return MUNIT_OK;
}

MunitResult test_pixel_to_string_regular_second(const MunitParameter params[], void* fixture){
    PPMPixel* pixel = (PPMPixel*) fixture;
    ppm_pixel_set(pixel, 128, 256, 80);
    char* result = ppm_pixel_to_string(pixel);
    munit_assert_string_equal(result, "128 - 255 - 080");
    return MUNIT_OK;
}

MunitResult test_pixel_to_string_error_no_pointer(const MunitParameter params[], void* fixture){
    char* result = ppm_pixel_to_string(NULL);
    munit_assert_string_equal(result, "");
    return MUNIT_OK;
}



int main(void) {
    return munit_suite_main(&ppm_pixel_suite, NULL, 0, NULL);
}
