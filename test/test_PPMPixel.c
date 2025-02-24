//
// Created by Sebastian Stewart on 23/02/2025.
//

#include "test_PPMPixel.h"
#include "../PPMPixel.h"
#include "munit/munit.h"

MunitTest tests[] = {
        {
                "/test-pixel-set-overflow",
                test_pixel_set_overflow,
                test_setup,
                test_tear_down,
                MUNIT_TEST_OPTION_NONE,
                NULL
        },
        {
                "/test-pixel-set-negative",
                test_pixel_set_negative,
                test_setup,
                test_tear_down,
                MUNIT_TEST_OPTION_NONE,
                NULL
        },
        {
                "/test-pixel-set-regular",
                test_pixel_set_regular,
                test_setup,
                test_tear_down,
                MUNIT_TEST_OPTION_NONE,
                NULL
        },
    {
                "/test-pixel-copy-regular",
                test_pixel_copy_regular,
                test_setup,
                test_tear_down,
                MUNIT_TEST_OPTION_NONE,
                NULL
        },
    {
                "/test-pixel-copy-no-second-pointer",
                test_pixel_copy_no_second_pointer,
                test_setup,
                test_tear_down,
                MUNIT_TEST_OPTION_NONE,
                NULL
        },
        { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite suite = {
        "/my-tests", /* name */
        tests, /* tests */
        NULL, /* suites */
        1, /* iterations */
        MUNIT_SUITE_OPTION_NONE /* options */
};

static void*
test_setup(const MunitParameter params[], void* user_data) {
    return ppm_pixel_new(0, 0, 0);
}

static void
test_tear_down(void* fixture) {
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
    PPMPixel* pixel2 = ppm_pixel_copy(pixel, (PPMPixel*) malloc(sizeof(PPMPixel)));
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



int main(void) {
    return munit_suite_main(&suite, NULL, 0, NULL);
}
