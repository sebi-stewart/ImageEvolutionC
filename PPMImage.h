#ifndef _PPMIMAGE_H
#define _PPMIMAGE_H

#include "PPMPixel.h"


#define PPM_HEADER_FORMAT "P6\n%d %d\n%d\n"
#define PPM_SIZE_FORMAT "%d %d"
#define PPM_RGB_FORMAT "%d"

/* 
 * This is a component that holds all the data of an 
 * image, and holds an array of pixels in the data value
 * as well as the width and height of the image
 * */
typedef struct {
    int x, y;
    PPMPixel* data;
} PPMImage;

/*
 * The function is not supposed to be called directly, but rather called via PPMImage_new or PPMImage_new_blank
 * This function simply creates a new instance of the PPMImage struct
 * The amount of memory allocated for pixels is dependent on the width and height inputs
 *
 * Parameters:
 * width - Pixel width or X value of the image as a positive integer
 * height - Pixel height or Y value of the image as a positive integer
 * R - Red value
 * G - Green value
 * B - Blue value
 *
 * Return:
 * Pointer to a allocated PPMImage object
 * 
 * Errors:
 * Memory was not able to be allocated
 * Width or Height were not greater than 0
 * */
PPMImage* PPMImage_init(int width, int height, int R, int G, int B);


/*
 * This function simply creates a new instance of the PPMImage struct and makes all the Pixeles therein the specified colour
 * The amount of memory allocated for pixels is dependent on the width and height inputs
 * Checks for validation on if RGB values are within bounds and if the Height and Width are within bounds
 *
 * Parameters:
 * width - Pixel width or X value of the image as a positive integer
 * height - Pixel height or Y value of the image as a positive integer
 * R - Red value
 * G - Green value
 * B - Blue value
 *
 * Return:
 * Pointer to a allocated PPMImage object
 * 
 * Errors:
 * Memory was not able to be allocated (via PPMImage_init)
 * Width or Height were not greater than 0 (via PPMImage_init)
 * Pixel values either over 255 or under 0
 * */
PPMImage* PPMImage_new(int width, int height, int R, int G, int B);

/* 
 * This function simply creates a new instance of the PPMImage struct and makes all the Pixels therein black
 * The amount of memory allocated for pixels is dependent on the width and height inputs
 * Checks for validation on if the Height and Width are within bounds
 * Parameters:
 * width - Pixel width or X value of the image as a positive integer
 * height - Pixel height or Y value of the image as a positive integer
 * R - Red value
 * G - Green value
 * B - Blue value
 *
 * Return:
 * Pointer to a allocated PPMImage object
 * 
 * Errors:
 * Memory was not able to be allocated (via PPMImage_init)
 * Width or Height were not greater than 0 (via PPMImage_init)
 * */
PPMImage* PPMImage_new_blank(int width, int height);

/*
 * This function sets the value of the specified pixel to the values inputted
 *
 * Parameters:
 * image - Non NULL pointer to the Image of which the pixel should be editted
 * x - Poistion of the pixel on the X-Axis
 * y - Poistion of the pixel on the Y-Axis
 * R, G, B - Colour values of the pixel
 *
 * Return:
 * Pointer to the modified PPMImage
 *
 * Errors:
 * Image was NULL
 * Image data was NULL
 * X, or Y were out of bounds
 * RGB values were out of bounds
 * */
PPMImage* PPMImage_set_pixel(PPMImage* image, int x, int y, unsigned char R, unsigned char G, unsigned char B);

/*
 * This function sets the value of all the pixels in an image
 *
 * Parameters:
 * image - Non NULL pointer to the Image of which the pixel should be editted
 * R, G, B - Colour values of the pixel
 *
 * Return:
 * Pointer to the modified PPMImage
 *
 * Errors:
 * Image was NULL
 * Image data was NULL
 * RGB values were out of bounds
 * */
PPMImage* PPMImage_set_background(PPMImage* image, unsigned char R, unsigned char G, unsigned char B);

/*
 * This function copies the pixels from one image to another
 * If either image is NULL then this one will be editted
 * If neither image is NULL, then img2 will be editted
 *
 * Parameters:
 * img1 - Pointer to a PPMImage
 * img2 - Pointer to a PPMImage
 *
 * Return:
 * Pointer to the editted PPMImage
 *
 * Errors:
 * Both images were NULL
 * Data of the non NULL image was NULL
 * Data of img1 was NULL
 * */
PPMImage* PPMImage_copy(PPMImage* img1, PPMImage* img2);

/*
 * Check if a given filepath has the ppm extension
 *
 * Parameters:
 * fp - Filepath to the PPM-image
 *
 * Return:
 * True if the filepath ends in ppm
 * False if the filepath doesn't end in ppm
 * False if the filepath is less than or equal to 4 characters long
 *
 * Errors:
 * Filepath was NULL
 * */
bool has_ppm_extension(char* fp);

/*
 * Load a new image into memory from a specific filepath
 *
 * Parameters:
 * fp - Filepath to the PPM-image to be loaded
 *
 * Return:
 * Copy of the image at the filepath
 *
 * Errors:
 * Filepath did not end in .ppm
 * Unable to open file
 * Unable to read file
 * Invalid image format
 * Invalid image size
 * Invalid rgb component
 * File does not have 8-bit components
 * Unable to allocate memory for image
 * Error in loading image
 * */
PPMImage* PPMImage_load(char* fp);

/*
 * Check if two images are equal in either:
 *  instance aka memory location
 *  or pixel values
 *
 * Parameters:
 * img1 - Non NULL pointer to a PPMImage
 * img2 - Non NULL pointer to a PPMImage
 *
 * Return:
 * True if two images are the same in either pixel value or memory address
 * True if both inputs are NULL
 * False if either input is NULL
 * False if both inputs do not equal eachother in pixel value and memory address
 * */
bool PPMImage_is_equal(PPMImage* img1, PPMImage* img2);

/*
 * Simply check if two images have the same dimensions
 *
 * Parameters:
 * img1 - Non NULL pointer to a PPMImage
 * img2 - Non NULL pointer to a PPMImage
 *
 * Return:
 * True if two images are the same height and width
 * True if both inputs are NULL
 * False if either input is NULL
 * False if both inputs do not equal eachother in height or width
 *
 * */
bool PPMImage_equal_dimensions(PPMImage* img1, PPMImage* img2);

/*
 * This function compares the difference in pixel values of the images
 * Relies on integers being 32 bits long
 *
 * Parameters:
 * img1 - Non NULL pointer to a PPMImage
 * img2 - Non NULL pointer to a PPMImage
 *
 * Return:
 * Integer evaluation -> 0 - (PIXEL_COLOR_VALUE-1)*3*width*height
 * If either value, or both are NULL -> -1
 * */
int PPMImage_compare(PPMImage* img1, PPMImage* img2);

/*
 * This function compares the difference in pixel values of the images
 * Larger differences in each pixel are weighted higher, as they are cubed
 * Relies on longs being 64bits long
 *
 * Parameters:
 * img1 - Non NULL pointer to a PPMImage
 * img2 - Non NULL pointer to a PPMImage
 *
 * Return:
 * Integer evaluation -> 0 - (PIXEL_COLOR_VALUE-1)*3*width*height
 * If either value, or both are NULL -> -1
 * */
long PPMImage_compare_weighted(PPMImage* img1, PPMImage* img2);

/*
 * Saves a given PPMImage to a specified filepath
 *
 * Parameters:
 * image - Non NULL pointer to a PPMImage
 * fp - Filepath of where to be saved
 *
 * Errors:
 * File could not be accessed/writing to file
 * Image was NULL
 * Filepath was NULL
 * Filepath did not end in .ppm
 * */
void PPMImage_save(PPMImage* image, char* fp);

/*
 *  Frees the memory allocated to each image and the individual pixels as well
 *
 *  Parameters:
 *  image - Non NULL pointer to a PPMImage
 *
 *  Errors:
 *  Image was NULL
 * */
void PPMImage_del(PPMImage* image);

/*
 * Prints the instance of an image
 *
 * Parameters:
 * image - Non NULL pointer to a PPMImage
 *
 * Errors:
 * Image was NULL
 * */
void PPMImage_print(PPMImage* image);


#endif
