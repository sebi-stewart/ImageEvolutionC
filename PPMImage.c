#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PPMImage.h"



PPMImage* PPMImage_new(int width, int height){
    if (width < 1 || height < 1){
        fprintf(stderr, "PPMImage_new: Height and/or Width of image were less than or equal to 0\n");
        exit(1);
    }

    PPMImage* img;
    img = (PPMImage *)malloc(sizeof(PPMImage));
    if (!img){
        fprintf(stderr, "PPMImage_new: Unable to allocate memory\n");
        exit(1);
    }
    
    img->x = width;
    img->y = height;
    img->data = (PPMPixel*)malloc(img->x * img->y * sizeof(PPMPixel));
    if (!img){
        fprintf(stderr, "PPMImage_new: Unable to allocate memory\n");
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

bool has_ppm_extension(char* fp){
    if (!fp){
        fprintf(stderr, "has_ppm_extension: Filepath was NULL\n");
        exit(1);
    }

    size_t len = strlen(fp);
    if (len > 4){
        const char* last4 = fp + len - 4;
        if (strcmp(last4, ".ppm") == 0) {
            return true; 
        }
    }

    return false;
}

bool PPMImage_is_equal(PPMImage* img1, PPMImage* img2){
    if (img1 == img2){
        return true;
    }
    bool dimensions = PPMImage_equal_dimensions(img1, img2);
    if (dimensions == false){
        return false;
    }

    int x, y, i;
    for(y = 0; y < img1->y; y++){
        for(x = 0; x < img1->x; x++){
            i = y * img1->x + x;
            if (!PPMPixel_is_equal(&img1->data[i], &img2->data[i])){
                return false;
            }
        }
    }
    return true;
}


bool PPMImage_equal_dimensions(PPMImage* img1, PPMImage* img2){
    if (img1 == img2){
        return true;
    }
    if (img1 == NULL || img2 == NULL){
        return false;
    }

    if(
            img1->x != img2->x ||
            img1->y != img2->y){
        return false;
    }

    return true;

}

int PPMImage_compare(PPMImage* img1, PPMImage* img2){
    if (img1 == NULL || img2 == NULL){
        return -1;
    }
    if (img1 == img2){
        return 0;
    }

    if (!PPMImage_equal_dimensions(img1, img2)){
        return -1;
    }

    short temp;
    int x, y, i, total;
    total = 0;
    for(y = 0; y < img1->y; y++){
        for(x = 0; x < img1->x; x++){
            i = y * img1->x + x;
            temp = PPMPixel_compare(&img1->data[i], &img2->data[i]);
            if (temp == -1) {
                return -1;
            }
            total += temp;
        }
    }
    return total;
}


void PPMImage_save(PPMImage* image, char* fp){
    if (!image){
        fprintf(stderr, "PPMImage_save: Image was NULL\n");
        exit(1);
    }

    if (!fp){
        fprintf(stderr, "PPMImage_save: Filepath was NULL\n");
        exit(1);
    }

    if (!has_ppm_extension(fp)){
        fprintf(stderr, "PPMImage_save: Filepath didn't end in PPM or was not long enough");
        exit(1);
    }

    FILE *file;

    file = fopen(fp, "w");
    if (!file){
        fprintf(stderr, "PPMImage_save: File was unable to be created");
        exit(1);
    }

    fprintf(file, P3_HEADER_FORMAT, image->x, image->y, PIXEL_COLOR_VALUE);
    fclose(file);

    
}

void PPMImage_del(PPMImage* image){
    if (!image){
        fprintf(stderr, "PPMImage_del: Image was NULL\n");
        exit(1);
    }
    
    if (image->data){ 
        free(image->data);
    }
    free(image);
}

void PPMImage_print(PPMImage* image){
    if (!image){
        fprintf(stderr, "PPMImage_print: Image was NULL\n");
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
    
    PPMImage* img1 = PPMImage_new(10, 10);
    PPMImage* img2 = PPMImage_new(10, 10);
    PPMImage* img3 = PPMImage_new(11, 10);
    PPMImage* img4 = PPMImage_new(10, 10);
    PPMImage_print(img1);
    printf("\n%d\n", PPMImage_is_equal(img1, img2));
    printf("\n%d\n", PPMImage_is_equal(img2, img1));
    printf("\n%d\n", PPMImage_is_equal(NULL, NULL));
    printf("%d\n", PPMImage_compare(NULL, NULL));

    printf("\n%d\n", PPMImage_is_equal(img2, img3));
    printf("%d\n", PPMImage_compare(img2, img3));

    img4->data[1] = *PPMPixel_set(&img4->data[1], 255, 255, 255);
    
    printf("\n%d\n", PPMImage_is_equal(img2, img4));
    printf("%d\n", PPMImage_compare(img2, img4));

    img2->data[1] = *PPMPixel_set(&img2->data[1], 255, 255, 255);

    printf("\n%d\n", PPMImage_is_equal(img2, img4));
    printf("%d\n", PPMImage_compare(img2, img4));


    PPMImage_del(img4);
    PPMImage_del(img3);

    PPMImage_save(img2, "a.ppm");

}
