#include <stdlib.h>
#include <stdio.h>
#include "PPMPixel.h"


PPMPixel* ppm_pixel_new(unsigned char R, unsigned char G, unsigned char B){
    return ppm_pixel_set((PPMPixel*) malloc(sizeof(PPMPixel)), R, G, B);
}


PPMPixel* ppm_pixel_set(PPMPixel* pixel, unsigned char R, unsigned char G, unsigned char B){
    if (pixel == NULL){
        return NULL;
    }

    if (R > PIXEL_COLOR_VALUE) {R = (unsigned char)PIXEL_COLOR_VALUE;}
    if (G > PIXEL_COLOR_VALUE) {G = (unsigned char)PIXEL_COLOR_VALUE;}
    if (B > PIXEL_COLOR_VALUE) {B = (unsigned char)PIXEL_COLOR_VALUE;}

    if (R < 0) {R = 0;}
    if (G < 0 ) {G = 0;}
    if (B < 0 ) {B = 0;}

    pixel->R = R;
    pixel->G = G;
    pixel->B = B;

    return pixel;
}

PPMPixel* ppm_pixel_copy(PPMPixel* p1, PPMPixel* p2){
    if (p1 == NULL && p2 == NULL){
        fprintf(stderr, "ppm_pixel_copy: Both pixels were NULL");
        exit(1);
    }

    if (p1 == p2){
        return p1;
    }

    if (p1 == NULL){
        p1 = p2;
        p2 = NULL;
    }

    if (p2 == NULL){
        p2 = ppm_pixel_new(p1->R, p1->G, p1->B);
    } else {
        p2 = ppm_pixel_set(p2, p1->R, p1->G, p1->B);
    }
    return p2;
}

bool ppm_pixel_is_equal(const PPMPixel* p1, const PPMPixel* p2){
    if (p1 == p2){
        return true;
    }
    if (p1 == NULL || p2 == NULL){
        return false;
    }
    if (p1->R == p2->R &&
            p1->G == p2->G &&
            p1->B == p2->B){
        return true;
    }
    return false;
}

unsigned int ppm_pixel_compare(const PPMPixel* p1, const PPMPixel* p2){
    if (p1 == NULL || p2 == NULL){
        return -1;
    }

    int sum = 0;
    sum += abs(p1->R - p2->R);
    sum += abs(p1->G - p2->G);
    sum += abs(p1->B - p2->B);
    return sum;
}

char* ppm_pixel_to_string(PPMPixel* pixel){
    if (!pixel){
        fprintf(stderr, "ppm_pixel_to_string: Pixel was NULL\n");
            exit(1);
    }
    char* pstring = (char*)malloc(PPM_STRING_BUFFER);
    if (!pstring){
        fprintf(stderr, "ppm_pixel_to_string: Unable to allocate memory for string\n");
        exit(1);
    }
    sprintf(pstring, PPM_STRING_FORMAT, pixel->R, pixel->G, pixel->B);

    return pstring;
}

void ppm_pixel_print(const PPMPixel* pixel){
    if (pixel == NULL){
        printf("NULL");
        return;
    }
    printf("%03d - %03d - %03d ", pixel->R, pixel->G, pixel->B);
}

//int main(void){
//    PPMPixel* pixel1 = ppm_pixel_new(10, 50, 20);
//    PPMPixel* pixel2 = ppm_pixel_new(15, 100, 40);
//    PPMPixel* pixel3 = ppm_pixel_new(20, 150, 60);
//    PPMPixel* pixel4 = ppm_pixel_new(25, 200, 80);
//
//    ppm_pixel_print(pixel1);
//    ppm_pixel_print(pixel3);
//    ppm_pixel_print(pixel2);
//    ppm_pixel_print(pixel4);
//
//    printf("Pixel difference: %hd\n", ppm_pixel_compare(pixel1, pixel2));
//    printf("Pixel difference: %hd\n", ppm_pixel_compare(pixel2, pixel3));
//    printf("Pixel difference: %hd\n", ppm_pixel_compare(pixel3, pixel4));
//    printf("Pixel difference: %hd\n", ppm_pixel_compare(pixel4, pixel1));
//    printf("Pixel difference: %hd\n", ppm_pixel_compare(pixel2, pixel1));
//
//    PPMPixel pixel5 = {123, 123, 2234};
//    ppm_pixel_print(&pixel5);
//
//    return 0;
//}
