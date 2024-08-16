#include <stdlib.h>
#include <stdio.h>
#include "pixel.h"


PPMPixel* PPMPixel_new(unsigned char red, unsigned char green, unsigned char blue){
    return PPMPixel_set((PPMPixel*) malloc(sizeof(PPMPixel)), red, green, blue);
}


PPMPixel* PPMPixel_set(PPMPixel* pixel, unsigned char red, unsigned char green, unsigned char blue){
    if (pixel == NULL){
        return NULL;
    }

    if (red > PIXEL_COLOR_VALUE - 1) {red = (unsigned char)PIXEL_COLOR_VALUE-1;}
    if (green > PIXEL_COLOR_VALUE - 1) {green = (unsigned char)PIXEL_COLOR_VALUE-1;}
    if (blue > PIXEL_COLOR_VALUE - 1) {blue = (unsigned char)PIXEL_COLOR_VALUE-1;}

    if (red < 0) {red = 0;}
    if (green < 0 ) {green = 0;}
    if (blue < 0 ) {blue = 0;}

    pixel->red = red;
    pixel->green = green;
    pixel->blue = blue;

    return pixel;
}

bool PPMPixel_is_equal(PPMPixel* p1, PPMPixel* p2){
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

short PPMPixel_compare(PPMPixel* p1, PPMPixel* p2){
    if (p1 == NULL || p2 == NULL){
        return -1;
    }

    short sum = 0;
    sum += abs(p1->red - p2->red);
    sum += abs(p1->green - p2->green);
    sum += abs(p1->blue - p2->blue);
    return sum;
}

void PPMPixel_print(PPMPixel* pixel){
    if (pixel == NULL){
        printf("Pixel: NULL\n");
        return;
    }
    printf("Pixel: %03d - %03d - %03d\n", pixel->red, pixel->green, pixel->blue);
    
}

int main(void){
    PPMPixel* pixel1 = PPMPixel_new(10, 50, 20);
    PPMPixel* pixel2 = PPMPixel_new(15, 100, 40);
    PPMPixel* pixel3 = PPMPixel_new(20, 150, 60);
    PPMPixel* pixel4 = PPMPixel_new(25, 200, 80);

    PPMPixel_print(pixel1);
    PPMPixel_print(pixel3);
    PPMPixel_print(pixel2);
    PPMPixel_print(pixel4);

    printf("Pixel difference: %hd\n", PPMPixel_compare(pixel1, pixel2));
    printf("Pixel difference: %hd\n", PPMPixel_compare(pixel2, pixel3));
    printf("Pixel difference: %hd\n", PPMPixel_compare(pixel3, pixel4));
    printf("Pixel difference: %hd\n", PPMPixel_compare(pixel4, pixel1));
    printf("Pixel difference: %hd\n", PPMPixel_compare(pixel2, pixel1));

    PPMPixel pixel5 = {123, 123, 2234};
    PPMPixel_print(&pixel5);

    return 0;
}
