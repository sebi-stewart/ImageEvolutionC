#include <stdlib.h>
#include <stdio.h>
#include "pixel.h"


pixel_t* pixel_new(int red, int green, int blue){
    return pixel_set((pixel_t*) malloc(sizeof(pixel_t)), red, green, blue);
}

pixel_t* pixel_set(pixel_t* pixel, int red, int green, int blue){
    if (pixel == NULL){
        return NULL;
    }

    if (red > PIXEL_COLOR_VALUE - 1) {red = PIXEL_COLOR_VALUE;}
    if (green > PIXEL_COLOR_VALUE - 1) {green = PIXEL_COLOR_VALUE;}
    if (blue > PIXEL_COLOR_VALUE - 1) {blue = PIXEL_COLOR_VALUE;}

    if (red < 0) {red = 0;}
    if (green < 0 ) {green = 0;}
    if (blue < 0 ) {blue = 0;}

    pixel->red = red;
    pixel->green = green;
    pixel->blue = blue;
    pixel->next = NULL;

    return pixel;
}

bool pixel_is_equal(pixel_t* p1, pixel_t* p2){
    if (p1 == p2){
        return true;
    }
    if (p1 == NULL || p2 == NULL){
        return false;
    }
    if (p1->red == p2->red &&
            p1->green == p2->green &&
            p1->blue == p2->blue){
        return true;
    }
    return false;
}

int pixel_compare(pixel_t* p1, pixel_t* p2){
    if (p1 == NULL || p2 == NULL){
        return -1;
    }

    int sum = 0;
    sum += abs(p1->red - p2->red);
    sum += abs(p1->green - p2->green);
    sum += abs(p1->blue - p2->blue);
    return sum;
}

void pixel_print(pixel_t* pixel){
    if (pixel == NULL){
        printf("Pixel: NULL\n\n");
        return;
    }
    printf("Pixel: %03d - %03d - %03d\n-> ", pixel->red, pixel->green, pixel->blue);
    
}

void pixel_clear(pixel_t* pixel){
    pixel_set(pixel, 0, 0, 0);
}

int main(void){
    pixel_t* pixel1 = pixel_new(10, 50, 20);
    pixel_t* pixel2 = pixel_new(15, 100, 40);
    pixel_t* pixel3 = pixel_new(20, 150, 60);
    pixel_t* pixel4 = pixel_new(25, 200, 80);

    pixel_link(pixel1, pixel2);
    pixel_link(pixel2, pixel3);
    pixel_link(pixel3, pixel4);


    pixel_print(pixel1);
    pixel_print(pixel3);

    pixel_delete(&pixel1, pixel3);

    pixel_print(pixel1);
    pixel_print(pixel4);

    return 0;
}
