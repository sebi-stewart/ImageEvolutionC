//
// Created by Sebastian Stewart on 01/03/2025.
//
#define PPM_TESTING
#include "test_PPMImage.h"


MunitTest tests[] = {
        { "/test-image-init-success", test_image_init, test_setup, test_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-init-width-height-err", test_image_init_width_height_err, test_setup, test_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-new-blank", test_image_new_blank, test_setup, test_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-set-pixel", test_image_set_pixel, test_setup, test_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-set-pixel-image-null", test_image_set_pixel_image_null, test_setup, test_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-set-pixel-image-data-null", test_image_set_pixel_image_data_null, test_setup, test_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-pixel-location-out-of-bounds", test_image_set_pixel_location_out_of_bounds, test_setup, test_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-pixel-colour-out-of-bounds", test_image_set_pixel_colour_out_of_bounds, test_setup, test_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-has-ppm-extension", test_has_ppm_extension, test_setup, test_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-has-ppm-extension-null", test_has_ppm_extension_null, test_setup, test_tear_down, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-load-valid-file", test_image_load_valid_file, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-load-invalid-extension", test_image_load_invalid_extension, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-load-nonexistent-file", test_image_load_nonexistent_file, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-load-invalid-format", test_image_load_invalid_format, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-load-invalid-size", test_image_load_invalid_size, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-load-invalid-rgb-component", test_image_load_invalid_rgb_component, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-compare-identical-images", test_image_compare_identical_images, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-compare-different-images", test_image_compare_different_images, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-save-valid-image", test_image_save_valid_image, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-save-null-image", test_image_save_null_image, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-save-null-filepath", test_image_save_null_filepath, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-save-invalid-extension", test_image_save_invalid_extension, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-del-valid-image", test_image_del_valid_image, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
        { "/test-image-del-null-image", test_image_del_null_image, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
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
    bool result = retrieve_err_call_and_compare((CallData){.count=1, .function_name="ppm_image_init@9"});

    return result ? MUNIT_OK : MUNIT_FAIL;
}

MunitResult test_image_new_blank(const MunitParameter params[], void* fixture){
    PPMImage* image = ppm_image_new_blank(8, 8);
    PPMPixel* pixel = ppm_pixel_new(0, 0, 0);

    MunitResult result = are_all_pixels_equal(image, pixel);

    ppm_image_del(image);
    free(pixel);
    return result;
}

MunitResult test_image_set_pixel(const MunitParameter params[], void* fixture){
    PPMImage* image = (PPMImage*) fixture;
    image = ppm_image_set_pixel(image, 0, 0, 255, 255, 255);
    if (image->data[0].R != 255) return MUNIT_FAIL;
    if (image->data[0].G != 255) return MUNIT_FAIL;
    if (image->data[0].B != 255) return MUNIT_FAIL;

    return MUNIT_OK;
}

MunitResult test_image_set_pixel_image_null(const MunitParameter params[], void* fixture){
    PPMImage* image = ppm_image_set_pixel(NULL, 0, 0, 255, 255, 255);
    munit_assert_null(image);
    bool result = retrieve_err_call_and_compare((CallData){.count=1, .function_name="ppm_image_set_pixel@37"});

    return result ? MUNIT_OK : MUNIT_FAIL;
}

MunitResult test_image_set_pixel_image_data_null(const MunitParameter params[], void* fixture){
    PPMImage* image = ppm_image_init(8, 8, 0, 0, 0);
    PPMPixel* pixels = image->data;
    image->data = NULL;
    PPMImage* return_val = ppm_image_set_pixel(image, 0, 0, 255, 255, 255);
    munit_assert_null(return_val);
    bool result = retrieve_err_call_and_compare((CallData){.count=1, .function_name="ppm_image_set_pixel@41"});

    image->data = pixels;
    ppm_image_del(image);
    return result ? MUNIT_OK : MUNIT_FAIL;
}

MunitResult test_image_set_pixel_location_out_of_bounds(const MunitParameter params[], void* fixture){
    PPMImage* image = (PPMImage*) fixture;
    ppm_image_set_pixel(image, 8, 0, 255, 255, 255);
    ppm_image_set_pixel(image, 0, 8, 255, 255, 255);
    ppm_image_set_pixel(image, -1, 0, 255, 255, 255);
    ppm_image_set_pixel(image, 0, -1, 255, 255, 255);

    bool result = retrieve_err_call_and_compare((CallData){.count=4, .function_name="ppm_image_set_pixel@46"});

    return result ? MUNIT_OK : MUNIT_FAIL;
}

MunitResult test_image_set_pixel_colour_out_of_bounds(const MunitParameter params[], void* fixture){
    PPMImage* image = (PPMImage*) fixture;
    ppm_image_set_pixel(image, 0, 0, -1, 0, 0);
    ppm_image_set_pixel(image, 0, 0, 0, -1, 0);
    ppm_image_set_pixel(image, 0, 0, 0, 0, -1);
    ppm_image_set_pixel(image, 0, 0, 256, 0, 0);
    ppm_image_set_pixel(image, 0, 0, 0, 256, 0);
    ppm_image_set_pixel(image, 0, 0, 0, 0, 256);

    bool result = retrieve_err_call_and_compare((CallData){.count=6, .function_name="ppm_image_set_pixel@54"});

    return result ? MUNIT_OK : MUNIT_FAIL;
}

MunitResult test_has_ppm_extension(const MunitParameter params[], void* fixture) {
    munit_assert_true(has_ppm_extension("test.ppm"));
    munit_assert_true(has_ppm_extension("test.PPM"));
    munit_assert_false(has_ppm_extension("test.png"));
    munit_assert_false(has_ppm_extension("test"));
    munit_assert_false(has_ppm_extension(".ppm"));
    return MUNIT_OK;
}

MunitResult test_has_ppm_extension_null(const MunitParameter params[], void* fixture) {
    has_ppm_extension(NULL);
    bool result = retrieve_err_call_and_compare((CallData){.count=1, .function_name="has_ppm_extension@65"});
    return result ? MUNIT_OK : MUNIT_FAIL;
}

MunitResult test_image_load_valid_file(const MunitParameter params[], void* fixture) {
    PPMImage* img = ppm_image_load("./test/images/test.ppm");
    munit_assert_not_null(img);
    munit_assert_int(img->x, >, 0);
    munit_assert_int(img->y, >, 0);
    munit_assert_not_null(img->data);
    ppm_image_del(img);
    return MUNIT_OK;
}

MunitResult test_image_load_invalid_extension(const MunitParameter params[], void* fixture) {
    PPMImage* img = ppm_image_load("./test/images/invalid_image.txt");
    munit_assert_null(img);
    bool result = retrieve_err_call_and_compare((CallData){.count=1, .function_name="ppm_image_load@84"});
    return result ? MUNIT_OK : MUNIT_FAIL;
}

MunitResult test_image_load_nonexistent_file(const MunitParameter params[], void* fixture) {
    PPMImage* img = ppm_image_load("./test/images/nonexistent_image.ppm");
    munit_assert_null(img);
    bool result = retrieve_err_call_and_compare((CallData){.count=1, .function_name="ppm_image_load@90"});
    return result ? MUNIT_OK : MUNIT_FAIL;
}

MunitResult test_image_load_invalid_format(const MunitParameter params[], void* fixture) {
    PPMImage* img = ppm_image_load("./test/images/invalid_format.ppm");
    munit_assert_null(img);
    bool result = retrieve_err_call_and_compare((CallData){.count=1, .function_name="ppm_image_load@101"});
    return result ? MUNIT_OK : MUNIT_FAIL;
}

MunitResult test_image_load_invalid_size(const MunitParameter params[], void* fixture) {
    PPMImage* img = ppm_image_load("./test/images/invalid_size.ppm");
    munit_assert_null(img);
    bool result = retrieve_err_call_and_compare((CallData){.count=1, .function_name="ppm_image_load@116"});
    return result ? MUNIT_OK : MUNIT_FAIL;
}

MunitResult test_image_load_invalid_rgb_component(const MunitParameter params[], void* fixture) {
    PPMImage* img1 = ppm_image_load("./test/images/invalid_rgb_component1.ppm");
    bool result = retrieve_err_call_and_compare((CallData){.count=1, .function_name="ppm_image_load@122"});
    munit_assert_null(img1);

    PPMImage* img2 = ppm_image_load("./test/images/invalid_rgb_component2.ppm");

    munit_assert_null(img2);
    result &= retrieve_err_call_and_compare((CallData){.count=1, .function_name="ppm_image_load@128"});
    return result ? MUNIT_OK : MUNIT_FAIL;
}

MunitResult test_image_compare_identical_images(const MunitParameter params[], void* fixture) {
    PPMImage* img1 = ppm_image_load("test.ppm");
    PPMImage* img2 = ppm_image_load("test.ppm");
    int diff = ppm_image_compare(img1, img2);
    munit_assert_int(diff, ==, 0);
    ppm_image_del(img1);
    ppm_image_del(img2);
    return MUNIT_OK;
}

MunitResult test_image_compare_different_images(const MunitParameter params[], void* fixture) {
    PPMImage* img1 = ppm_image_load("test.ppm");
    PPMImage* img2 = ppm_image_load("best.ppm");
    int diff = ppm_image_compare(img1, img2);
    munit_assert_int(diff, >, 0);
    ppm_image_del(img1);
    ppm_image_del(img2);
    return MUNIT_OK;
}

MunitResult test_image_save_valid_image(const MunitParameter params[], void* fixture) {
    PPMImage* img = ppm_image_new_blank(10, 10);
    ppm_image_save(img, "valid_image.ppm");
    FILE* file = fopen("valid_image.ppm", "rb");
    munit_assert_not_null(file);
    fclose(file);
    ppm_image_del(img);
    remove("valid_image.ppm");
    return MUNIT_OK;
}

MunitResult test_image_save_null_image(const MunitParameter params[], void* fixture) {
    ppm_image_save(NULL, "null_image.ppm");
    FILE* file = fopen("null_image.ppm", "rb");
    munit_assert_null(file);
    bool result = retrieve_err_call_and_compare((CallData){.count=1, .function_name="ppm_image_save@169"});

    return result ? MUNIT_OK : MUNIT_FAIL;
}

MunitResult test_image_save_null_filepath(const MunitParameter params[], void* fixture) {
    PPMImage* img = ppm_image_new_blank(10, 10);
    ppm_image_save(img, NULL);
    ppm_image_del(img);
    bool result = retrieve_err_call_and_compare((CallData){.count=1, .function_name="ppm_image_save@174"});

    return result ? MUNIT_OK : MUNIT_FAIL;
}

MunitResult test_image_save_invalid_extension(const MunitParameter params[], void* fixture) {
    PPMImage* img = ppm_image_new_blank(10, 10);
    ppm_image_save(img, "invalid_image.txt");
    FILE* file = fopen("invalid_image.txt", "rb");
    munit_assert_null(file);
    ppm_image_del(img);
    bool result = retrieve_err_call_and_compare((CallData){.count=1, .function_name="ppm_image_save@179"});

    return result ? MUNIT_OK : MUNIT_FAIL;
}

MunitResult test_image_del_valid_image(const MunitParameter params[], void* fixture) {
    PPMImage* img = ppm_image_new_blank(10, 10);
    ppm_image_del(img);
    munit_assert_null(img->data);

    return MUNIT_OK;
}

MunitResult test_image_del_null_image(const MunitParameter params[], void* fixture) {
    ppm_image_del(NULL);
    bool result = retrieve_err_call_and_compare((CallData){.count=1, .function_name="ppm_image_del@202"});

    return result ? MUNIT_OK : MUNIT_FAIL;
}

int main(void) {
    return munit_suite_main(&suite, NULL, 0, NULL);
}
