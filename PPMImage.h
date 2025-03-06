#ifndef IMAGE_EVOLUTION_PPM_IMAGE_H
#define IMAGE_EVOLUTION_PPM_IMAGE_H

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
 * The function is not supposed to be called directly, but rather called via ppm_image_new or ppm_image_new_blank
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
 * Pointer to an allocated PPMImage object
 * 
 * Errors:
 * Memory was not successfully allocated
 * Width or Height were not greater than 0
 * */
PPMImage* ppm_image_init(unsigned int width, unsigned int height, int R, int G, int B);


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
 * Pointer to an allocated PPMImage object
 * 
 * Errors:
 * Memory was not successfully allocated (via ppm_image_init)
 * Width or Height were not greater than 0 (via ppm_image_init)
 * */
PPMImage* ppm_image_new_blank(int width, int height);

/*
 * This function sets the value of the specified pixel to the values inputted
 *
 * Parameters:
 * image - Non-NULL pointer to the Image of which the pixel should be edited
 * x - Position of the pixel on the X-Axis
 * y - Position of the pixel on the Y-Axis
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
PPMImage* ppm_image_set_pixel(PPMImage* image, int x, int y, unsigned char R, unsigned char G, unsigned char B);

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
PPMImage* ppm_image_load(char* fp);


int ppm_image_compare_unsafe(const PPMImage* img1, const PPMImage* img2);


/*
 * Saves a given PPMImage to a specified filepath
 *
 * Parameters:
 * image - Non-NULL pointer to a PPMImage
 * fp - Filepath of where to be saved
 *
 * Errors:
 * File could not be accessed/writing to file
 * Image was NULL
 * Filepath was NULL
 * Filepath did not end in .ppm
 * */
void ppm_image_save(PPMImage* image, char* fp);

/*
 *  Frees the memory allocated to each image and the individual pixels as well
 *
 *  Parameters:
 *  image - Non-NULL pointer to a PPMImage
 *
 *  Errors:
 *  Image was NULL
 * */
void ppm_image_del(PPMImage* image);

#endif
