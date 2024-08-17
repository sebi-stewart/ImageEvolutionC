#ifndef _PPMIMAGE_H
#define _PPMIMAGE_H

#include "PPMPixel.h"

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
 * This function simply creates a new instance of the PPMImage struct
 * The amount of memory allocated for pixels is dependent on the width and height inputs
 *
 * Parameters:
 * width - Pixel width or X value of the image as a positive integer
 * height - Pixel height or Y value of the image as a positive integer
 *
 * Return:
 * Pointer to a allocated PPMImage object
 * 
 * Errors:
 * Memory was not able to be allocated
 * Width or Height were not greater than 0
 * */
PPMImage* PPMImage_new(int width, int height);

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
 * Filepath was invalid
 * Invalid image format
 * Invalid image size
 * Invalid rgb component
 * File does not have 8-bit components
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
 * Saves a given PPMImage to a specified filepath
 *
 * Parameters:
 * image - Non NULL pointer to a PPMImage
 * fp - Filepath of where to be saved
 *
 * Errors:
 * File was not found
 * File could not be accessed/writing to file
 * Image was NULL
 * Filepath was NULL
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
