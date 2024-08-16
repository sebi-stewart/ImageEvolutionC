#include "pixel.h"

/* 
 * This is a component that holds all the data of an 
 * image, and holds the top left pixel of the image in front,
 * as well as the width and height of the image
 * */
typedef struct {
    int width;
    int height;
    pixel_t *front;
} PPMImage;




