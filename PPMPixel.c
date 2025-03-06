#include <stdlib.h>
#include <stdio.h>
#include "PPMPixel.h"


PPMPixel* ppm_pixel_new(unsigned char R, unsigned char G, unsigned char B){
    PPMPixel* new_pixel = (PPMPixel*) malloc(sizeof(PPMPixel));
    if (new_pixel == NULL){
        fprintf(stderr, "ppm_pixel_new: Memory allocation for PPMPixel failed\n");
        exit(1);
    }
    ppm_pixel_set(new_pixel, R, G, B);
    return new_pixel;
}


void ppm_pixel_set(PPMPixel* pixel, int R, int G, int B){
    if (pixel == NULL){
        return;
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
}

void ppm_pixel_set_unsafe(PPMPixel* pixel, int R, int G, int B){
    pixel->R = R;
    pixel->G = G;
    pixel->B = B;
}

PPMPixel* ppm_pixel_copy(PPMPixel* p1, PPMPixel* p2){
    if (p1 == NULL && p2 == NULL){
        fprintf(stdout, "ppm_pixel_copy: Both pixels were NULL");
        return p1;
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
        ppm_pixel_set(p2, p1->R, p1->G, p1->B);
    }
    return p2;
}

int ppm_pixel_compare(const PPMPixel* p1, const PPMPixel* p2){
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
            return "";
    }
    char* pstring = (char*)malloc(PPM_STRING_BUFFER);
    if (!pstring){
        fprintf(stderr, "ppm_pixel_to_string: Unable to allocate memory for string\n");
        exit(1);
    }
    sprintf(pstring, PPM_STRING_FORMAT, pixel->R, pixel->G, pixel->B);

    return pstring;
}

void ppm_pixel_print(PPMPixel* pixel){
    if (pixel == NULL){
        printf("NULL");
        return;
    }

    char* pixel_string = ppm_pixel_to_string(pixel);
    printf("%s", pixel_string);
}
