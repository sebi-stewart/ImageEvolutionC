#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PPMImage.h"
#include <limits.h>


PPMImage* ppm_image_init(int width, int height, int R, int G, int B){
    if (width < 1 || height < 1){
        fprintf(stderr, "ppm_image_init: Height and/or Width of image were less than or equal to 0\n");
        exit(1);
    }

    PPMImage* img;
    img = (PPMImage *)malloc(sizeof(PPMImage));
    if (!img){
        fprintf(stderr, "ppm_image_init: Unable to allocate memory\n");
        exit(1);
    }
    
    img->x = width;
    img->y = height;
    img->data = (PPMPixel*)malloc(img->x * img->y * sizeof(PPMPixel));
    if (!img){
        fprintf(stderr, "ppm_image_init: Unable to allocate memory\n");
        exit(1);
    }
    
    int i, lim;
    lim = img->x * img->y;
    for(i = 0; i < lim; i++){
        img->data[i] = *ppm_pixel_set(&img->data[i], R, G, B);
    }
    return img;

}

PPMImage* ppm_image_new(int width, int height, int R, int G, int B){
    if (
            R < 0 || R > PIXEL_COLOR_VALUE ||
            G < 0 || G > PIXEL_COLOR_VALUE ||
            B < 0 || B > PIXEL_COLOR_VALUE){
        fprintf(stderr, "ppm_image_new: RGB values were out of bounds\n");
        exit(1);
    }
    
    PPMImage* img = ppm_image_init(width, height, R, G, B);
    return img;
}

PPMImage* ppm_image_new_blank(int width, int height){
    PPMImage* img = ppm_image_init(width, height, 0, 0, 0);
    return img;
}

PPMImage* ppm_image_set_pixel(PPMImage* image, int x, int y, unsigned char R, unsigned char G, unsigned char B){
    if (!image){
        fprintf(stderr, "ppm_image_set_pixel: Image was NULL\n");
        exit(1);
    }
    if (!image->data){
        fprintf(stderr, "ppm_image_set_pixel: Image data was NULL\n");
        exit(1);
    }

    if (x < 0 || x >= image->x || y < 0 || y >= image->y){
        fprintf(stderr, "ppm_image_set_pixel: X and/or Y value were out of bounds\n");
        exit(1);
    }

    if (
            R < 0 || R > PIXEL_COLOR_VALUE ||
            G < 0 || G > PIXEL_COLOR_VALUE ||
            B < 0 || B > PIXEL_COLOR_VALUE){
        fprintf(stderr, "ppm_image_set_pixel: RGB values were out of bounds\n");
        exit(1);
    }

    image->data[y * image->x + x] = *ppm_pixel_set(&image->data[y * image->x + x], R, G, B);
    return image;
}


PPMImage* ppm_image_set_background(PPMImage* image, unsigned char R, unsigned char G, unsigned char B){
    if (!image){
        fprintf(stderr, "ppm_image_set_pixel: Image was NULL\n");
        exit(1);
    }
    if (!image->data){
        fprintf(stderr, "ppm_image_set_pixel: Image data was NULL\n");
        exit(1);
    }

    if (
            R < 0 || R > PIXEL_COLOR_VALUE ||
            G < 0 || G > PIXEL_COLOR_VALUE ||
            B < 0 || B > PIXEL_COLOR_VALUE){
        fprintf(stderr, "ppm_image_set_pixel: RGB values were out of bounds\n");
        exit(1);
    }
    int i, lim;
    lim = image->x * image->y;
    for (i=0; i < lim; i++){
        image->data[i] = *ppm_pixel_set(&image->data[i], R, G, B);
    }
    return image;
}

PPMImage* ppm_image_copy(PPMImage* img1, PPMImage* img2){
    if (img1 == NULL && img2 == NULL){
        fprintf(stderr, "ppm_image_copy: Both images were NULL\n");
        exit(1);
    }

    if (img1 == img2){
        return img1;
    }
    
    if (img1 == NULL){
        img1 = img2;
        img2 = NULL;
    }

    if (img1->data == NULL){
        fprintf(stderr, "ppm_image_copy: Image data was NULL\n");
        exit(1);
    }

    if (img2 == NULL){
        img2 = ppm_image_new_blank(img1->x, img1->y);
    }

    int i, lim;
    lim = img1->x * img1->y;
    for (i=0; i < lim; i++){
        img2->data[i] = *ppm_pixel_copy(&img1->data[i], &img2->data[i]);
    }
    return img2;
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

PPMImage* ppm_image_load(char* fp){
    char buff[16];
    PPMImage* img;
    FILE *file;

    if (!has_ppm_extension(fp)){
        fprintf(stderr, "ppm_image_load: File didn't end in PPM or was not long enough\n");
        exit(1);
    }

    file = fopen(fp, "rb");
    if (!file){
        fprintf(stderr, "ppm_image_load: Unable to open file\n");
        exit(1);
    }

    // Read and Check image format
    if (!fgets(buff, sizeof(buff), file)){
        fprintf(stderr, "ppm_image_load: Unable to read file\n");
        exit(1);
    }

    if (buff[0] != 'P' || buff[1] != '6'){
        fprintf(stderr, "ppm_image_load: Invalid Image format (must be P6)\n");
        exit(1);
    }

    int c, x, y, rgb_comp_colour;
    // Check for comments
    c = getc(file);
    while (c == '#') {
        while (getc(file) != '\n') ;
        c = getc(file);
    }
    ungetc(c, file);

    // Read image size information
    if (fscanf(file, PPM_SIZE_FORMAT, &x, &y) != 2 || x <= 0 || y <= 0){
        fprintf(stderr, "ppm_image_load: Invalid image size\n");
        exit(1);
    }

    // Read RGB Component
    if (fscanf(file, PPM_RGB_FORMAT, &rgb_comp_colour) != 1){
        fprintf(stderr, "ppm_image_load: Invalid RGB Component\n");
        exit(1);
    }

    // Check RGB Component depth
    if (rgb_comp_colour != PIXEL_COLOR_VALUE){
        fprintf(stderr, "ppm_image_load: File does not contain 8-bit components\n");
        exit(1);
    }

    while (fgetc(file) != '\n') ;

    // Memory allocation for the image
    img = ppm_image_new_blank(x, y);
    if (!img || !img->data){
        fprintf(stderr, "ppm_image_load: Unable to allocate memory for image\n");
        exit(1);
    }

    // Load image data
    if (fread(img->data, 3 * img->x, img->y, file) != img->y){
        fprintf(stderr, "ppm_image_load: Unable to load image data\n");
        exit(1);
    } 

    fclose(file);
    return img;

}

bool ppm_image_is_equal(PPMImage* img1, PPMImage* img2){
    if (img1 == img2){
        return true;
    }
    bool dimensions = ppm_image_equal_dimensions(img1, img2);
    if (dimensions == false){
        return false;
    }
    
    int i, lim;
    lim = img1->x * img1->y;
    for(i = 0; i < lim; i++){
        if (!ppm_pixel_is_equal(&img1->data[i], &img2->data[i])){
            return false;
        }
    }
    return true;
}


bool ppm_image_equal_dimensions(PPMImage* img1, PPMImage* img2){
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

int ppm_image_compare(PPMImage* img1, PPMImage* img2){
    if (img1 == NULL || img2 == NULL){
        return -1;
    }
    if (img1 == img2){
        return 0;
    }

    if (!ppm_image_equal_dimensions(img1, img2)){
        return -1;
    }

    short temp;
    int i, lim, total;
    lim = img1->x * img1->y;
    total = 0;
    for(i = 0; i < lim; i++){
        temp = ppm_pixel_compare(&img1->data[i], &img2->data[i]);
        if (temp == -1){
            return -1;
        }
        total += temp;
    }
    return total;
}

long ppm_image_compare_weighted(PPMImage* img1, PPMImage* img2){
    if (img1 == NULL || img2 == NULL){
        return -1;
    }
    if (img1 == img2){
        return 0;
    }

    if (!ppm_image_equal_dimensions(img1, img2)){
        return -1;
    }
    
    int i, lim, temp;
    long total = 0;
    lim = img1->x * img1->y;
    for(i = 0; i < lim; i++){
        temp = ppm_pixel_compare(&img1->data[i], &img2->data[i]);
        if (temp == -1){
            return -1; 
        }
        total += (temp * temp);
    }
    return total;
}


void ppm_image_save(PPMImage* image, char* fp){
    if (!image){
        fprintf(stderr, "ppm_image_save: Image was NULL\n");
        exit(1);
    }

    if (!fp){
        fprintf(stderr, "ppm_image_save: Filepath was NULL\n");
        exit(1);
    }

    if (!has_ppm_extension(fp)){
        fprintf(stderr, "ppm_image_save: Filepath didn't end in PPM or was not long enough");
        exit(1);
    }

    FILE *file;

    file = fopen(fp, "wb");
    if (!file){
        fprintf(stderr, "ppm_image_save: File was unable to be created");
        exit(1);
    }
    
    // Write the header to the file
    fprintf(file, PPM_HEADER_FORMAT, image->x, image->y, PIXEL_COLOR_VALUE);
    
    // Write the image contents
    fwrite(image->data, 3 * image->x, image->y, file);

    fclose(file);
}

void ppm_image_del(PPMImage* image){
    if (!image){
        fprintf(stderr, "ppm_image_del: Image was NULL\n");
        exit(1);
    }
    
    if (image->data){ 
        free(image->data);
    }
    free(image);
}

void ppm_image_print(PPMImage* image){
    if (!image){
        fprintf(stderr, "ppm_image_print: Image was NULL\n");
        exit(1);
    }
    int x, y, i;
    for(y = 0; y < image->y; y++){
        for(x = 0; x < image->x; x++){
            i = y * image->x + x;
            ppm_pixel_print(&image->data[i]);
        }
        printf("\n");
    }
}

int main(void){
    char* file_path = "7a.ppm";
    
    PPMImage* img1 = ppm_image_load(file_path);
    int itter = 10000;

    int i, dif;
    for(i=0; i < itter; i++){
        PPMImage* img2 = ppm_image_new(200, 200, 122, 122, 122);
        dif = ppm_image_compare(img1, img2);

        ppm_image_del(img2);
    }
    
    return 0;
}
