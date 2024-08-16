#ifndef _PIXEL_H
#define _PIXEL_H

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
typedef struct pixel {
    int red;
    int green;
    int blue;
} pixel_t;


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
pixel_t* pixel_new(int red, int green, int blue);

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
pixel_t* pixel_set(pixel_t* pixel, int red, int green, int blue);

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
bool pixel_is_equal(pixel_t* p1, pixel_t* p2);

/*
 * This function compares the how similar two pixels are in RGB values
 * 
 * Parameters:
 * p1 - A non null pointer to the first pixel
 * p2 - A non null pointer to the second pixel 
 *
 * Return:
 * Integer evaluation -> 0 - (PIXEL_COLOR_VALUE-1)*3
 * If either value, or both are NULL -1 is returned
 * */
int pixel_compare(pixel_t* p1, pixel_t* p2);

/*
 * Prints the pixel to the console
 *
 * Parameters:
 * pixel - A non null pointer to a pixel
 *
 * Errors:
 * If pixel is null, the function is terminated
 * */
void pixel_print(pixel_t* pixel);

/*
 * Free the allocated memory of the pixel struct element
 *
 * If the pixel to be deleted is null, this function has no effect
 * If the head and to be deleted pixel are the same, the head gets freed
 * and the next pixel is returned
 * Else the pixel to be deleted is found and deleted and cut out
 *
 *
 * Parameters:
 * head - Pointer to the pointer to the head of the pixel list
 * del_pixel - Pointer to pixel struct to be freed
 *
 * Return:
 * If head is the same as del_pixel then head->next is returned
 * Else head is returned
 * */
void pixel_clear(pixel_t* pixel);

#endif
