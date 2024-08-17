#ifndef _PPMPIXEL_H
#define _PPMPIXEL_H

#include <stdbool.h>

/*
 * This defines how many colour values each RGB component could have
 * The maximum value for each RGB component is PIXEL_COLOR_VALUE - 1
 * */
#define PIXEL_COLOR_VALUE 256


/*
 * This is a singular pixel that holds colour values
 * Each pixel points to the next pixel in the image
 * The pixels go from top left to top right and then 
 * a line down each time
 * */
typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} PPMPixel;


/*
 * This function creates a new pixel based on the input values
 *
 * Parameters:
 * red - int RGB red value 0-255
 * green - int RGB green value 0-255
 * blue - int RGB blue value 0-255
 *
 * Return:
 * Pointer to the newly created pixel struct
 * */
PPMPixel* PPMPixel_new(unsigned char red, unsigned char green, unsigned char blue);

/*
 * This function sets the values of the pixel pointed to
 * If any of the RGB inputs are not given, or is null, it is marked as 0
 * if any of the RGB inputs are larger than the max value they are marked as the max value
 * If any of the RGB inputs are smaller than 0 they are marked as 0
 *
 * Parameters:
 * pixel - Pointer to a newly created pixel
 * red - int RGB red value 0-255
 * green - int RGB green value 0-255
 * blue - int RGB blue value 0-255
 * 
 * Return:
 * Pointer to assigned pixel
 * */
PPMPixel* PPMPixel_set(PPMPixel* pixel, unsigned char red, unsigned char green, unsigned char blue);

/*
 * This function checks if two pixels are equal to eachother
 * If both inputs are null it returns true
 * If they are not equal or if only one is null, it returns false
 * If they are both equal it returns true
 *
 * Parameters:
 * p1 - A non null pointer to the first pixel
 * p2 - A non null pointer to the second pixel
 *
 * Return: 
 * Bool evaluation -> true | false
 * 
 * */
bool PPMPixel_is_equal(PPMPixel* p1, PPMPixel* p2);

/*
 * This function compares the how similar two pixels are in RGB values
 * We expect the same result if the inputs aka p1 and p2 were swapped
 * 
 * Parameters:
 * p1 - A non null pointer to the first pixel
 * p2 - A non null pointer to the second pixel 
 *
 * Return:
 * Short evaluation -> 0 - (PIXEL_COLOR_VALUE-1)*3
 * If either value, or both are NULL -1 is returned
 * */
short PPMPixel_compare(PPMPixel* p1, PPMPixel* p2);

/*
 * Prints the pixel to the console
 *
 * Parameters:
 * pixel - A non null pointer to a pixel
 *
 * Errors:
 * If pixel is null, the function is terminated
 * */
void PPMPixel_print(PPMPixel* pixel);
#endif
