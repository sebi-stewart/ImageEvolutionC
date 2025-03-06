//
// Created by Sebastian Stewart on 01/03/2025.
//
#define PPM_TESTING
#include "test_PPMImage.h"


MunitTest tests[] = {
        {
                "/test-image-init-success",
                test_image_init,
                test_setup,
                test_tear_down,
                MUNIT_TEST_OPTION_NONE,
                NULL
        },
        {
                "/test-image-init-width-height-err",
                test_image_init_width_height_err,
                test_setup,
                test_tear_down,
                MUNIT_TEST_OPTION_NONE,
                NULL
        },
        { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite suite = {
        "/PPMImage-tests", /* name */
        tests, /* tests */
        NULL, /* suites */
        1, /* iterations */
        MUNIT_SUITE_OPTION_NONE /* options */
};

static void*
test_setup(const MunitParameter params[], void* user_data) {
    clear_call_file();
    return ppm_image_init(8, 8, 0, 0, 0);
}

static void
test_tear_down(void* fixture) {
    ppm_image_del(fixture);
}

MunitResult are_images_equal(PPMImage* image1, PPMImage* image2){
    if (image1 == NULL && image2 == NULL){
        return MUNIT_OK;
    } if (image1 == NULL || image2 == NULL){
        return MUNIT_FAIL;
    }
    munit_assert_memory_equal(sizeof(PPMImage), image1, image2);

    return MUNIT_OK;
}

MunitResult are_all_pixels_equal(PPMImage* image, PPMPixel* comparison){
    if (image == NULL || comparison == NULL){
        return MUNIT_FAIL;
    }

    if (image->x < 1 || image->y < 1){
        return MUNIT_FAIL;
    }

    const int lim = (int) (image->x * image->y);
    for(int i = 0; i < lim; i++){
        munit_assert_memory_equal(sizeof(PPMPixel), &image->data[i], comparison);
    }
    return MUNIT_OK;
}

MunitResult test_image_init(const MunitParameter params[], void* fixture){

    PPMImage* image = ppm_image_init(8, 8, 0, 0, 0);
    PPMPixel* pixel = ppm_pixel_new(0, 0, 0);

    MunitResult result = are_all_pixels_equal(image, pixel);

    ppm_image_del(image);
    free(pixel);
    return result;
}

MunitResult test_image_init_width_height_err(const MunitParameter params[], void* fixture){

    PPMImage* image = ppm_image_init(0, 0, 0, 0, 0);
    munit_assert_null(image);
    bool result = retrieve_err_call_and_compare((CallData){.count=1, .function_name="ppm_image_init"});

    return result ? MUNIT_OK : MUNIT_FAIL;
}

int main(void) {
    return munit_suite_main(&suite, NULL, 0, NULL);
}
