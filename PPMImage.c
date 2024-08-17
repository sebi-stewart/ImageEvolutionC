#include <stdio.h>
#include <stdlib.h>
#include "PPMImage.h"



PPMImage* PPMImage_new(int width, int height){
    if (width < 1 || height < 1){
        fprintf(stderr, "Height and/or Width of image were less than or equal to 0");
        exit(1);
    }

    PPMImage* img;
    img = (PPMImage *)malloc(sizeof(PPMImage));
    if (!img){
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    
    img->x = width;
    img->y = height;
    img->data = (PPMPixel*)malloc(img->x * img->y * sizeof(PPMPixel));
    if (!img){
        fprintf(stderr, "Unable to allocate memory\n");
        exit(1);
    }
    
    int x, y, i;
    for(y = 0; y < img->y; y++){
        for(x = 0; x < img->x; x++){
            i = y * img->x + x;
            img->data[i] = *PPMPixel_set(&img->data[i], 0, 0, 0);
        }
    }

    return img;
}


void PPMImage_print(PPMImage* image){
    if (!image){
        fprintf(stderr, "Image did not exist\n");
        exit(1);
    }
    int x, y, i;
    for(y = 0; y < image->y; y++){
        for(x = 0; x < image->x; x++){
            i = y * image->x + x;
            PPMPixel_print(&image->data[i]);
        }
        printf("\n");
    }
}

int main(void){
    
    PPMImage* img = PPMImage_new(10, 10);
    PPMImage_print(img);
    
}
