#ifndef _PPM_PIXEL_H
#define _PPM_PIXEL_H

#include <stdbool.h>

/*
 * This defines how many colour values each RGB component could have
 * The maximum value for each RGB component is PIXEL_COLOR_VALUE
 * Therefore we can have PIXEL_COLOR_VALUE + 1 different colour values
 * */
#define PIXEL_COLOR_VALUE 255

/*
 * Definers for the to string conversion
 * The buffer is of size 4 * 3 + 1
 * This means that we have 4 (3 maximum digits per RGB value + 1 white space) * 3 times + 1 null terminator
 * */
#define PPM_STRING_BUFFER 13
#define PPM_STRING_FORMAT "%u %u %u "



/*
 * This is a singular pixel that holds colour values
 * Each pixel points to the next pixel in the image
 * The pixels go from top left to top right and then 
 * a line down each time
 * */
typedef struct {
    unsigned char R;
    unsigned char G;
    unsigned char B;
} PPMPixel;


/*
 * This function creates a new pixel based on the input values
 *
 * Parameters:
 * R - int RGB red value 0-255
 * G - int RGB green value 0-255
 * B - int RGB blue value 0-255
 *
 * Return:
 * Pointer to the newly created pixel struct
 * */
PPMPixel* ppm_pixel_new(unsigned char R, unsigned char G, unsigned char B);

/*
 * This function sets the values of the pixel pointed to
 * If any of the RGB inputs are not given, or is null, it is marked as 0
 * if any of the RGB inputs are larger than the max value they are marked as the max value
 * If any of the RGB inputs are smaller than 0 they are marked as 0
 *
 * Parameters:
 * pixel - Pointer to a newly created pixel
 * R - int RGB red value 0-255
 * G - int RGB green value 0-255
 * B - int RGB blue value 0-255
 * 
 * Return:
 * Pointer to assigned pixel
 * */
PPMPixel* ppm_pixel_set(PPMPixel* pixel, unsigned char R, unsigned char G, unsigned char B);

/*
 * Copies the values of one pixel to the other
 * If either is null, memory is assigned for it and the existing one is used as the template for the values
 * If both exist then p2 is edited
 *
 * Parameters:
 * p1 - Pointer to a Pixel
 * p2 - Pointer to a Pixel
 *
 * Return:
 * Pointer to the pixel that was edited
 *
 * Errors:
 * Both pointers were NULL
 * */
PPMPixel* ppm_pixel_copy(PPMPixel* p1, PPMPixel* p2);

/*
 * This function checks if two pixels are equal to each other
 * If both inputs are null it returns true
 * If they are not equal or if only one is null, it returns false
 * If they are both equal it returns true
 *
 * Parameters:
 * p1 - A Non-NULL pointer to the first pixel
 * p2 - A Non-NULL pointer to the second pixel
 *
 * Return: 
 * Bool evaluation -> true | false
 * 
 * */
bool ppm_pixel_is_equal(const PPMPixel* p1, const PPMPixel* p2);

/*
 * This function compares how similar two pixels are in RGB values
 * We expect the same result if the inputs aka p1 and p2 were swapped
 * 
 * Parameters:
 * p1 - A Non-NULL pointer to the first pixel
 * p2 - A Non-NULL pointer to the second pixel 
 *
 * Return:
 * Short evaluation -> 0 - (PIXEL_COLOR_VALUE)*3
 * If either value, or both are NULL -1 is returned
 * */
unsigned int ppm_pixel_compare(const PPMPixel* p1, const PPMPixel* p2);

/*
 * This function converts a given pixel into a string of format 
 * Uses the P3_STRING_FORMAT
 *
 * Parameters:
 * pixel - A Non-NULL pointer to the pixel
 *
 * Return:
 * Pointer to a char array of the pixel
 *
 * Errors:
 * If the pixel is null, the function is terminated
 * Unable to allocate memory for the string
 * */
char* ppm_pixel_to_string(PPMPixel* pixel);

/*
 * Prints the pixel to the console
 *
 * Parameters:
 * pixel - A Non-NULL pointer to a pixel
 *
 * Errors:
 * If pixel is null, the function is terminated
 * */
void ppm_pixel_print(const PPMPixel* pixel);
#endif
