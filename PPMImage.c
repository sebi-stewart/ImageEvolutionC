#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PPMImage.h"

PPMImage* ppm_image_init(const unsigned int width, const unsigned int height, const int R, const int G, const int B){
    if (width < 1 || height < 1){
        fprintf(stderr, "ppm_image_init: Height and/or Width of image were less than or equal to 0\n");
        BREAK_OUT(1);
    }

    PPMImage *img = (PPMImage *) malloc(sizeof(PPMImage));
    if (!img){
        fprintf(stderr, "ppm_image_init: Unable to allocate memory\n");
        BREAK_OUT(1);
    }
    
    img->x = (int)width;
    img->y = (int)height;
    img->data = (PPMPixel*)malloc(width * height * sizeof(PPMPixel));

    const int lim = (int) (width * height);
    for(int i = 0; i < lim; i++){
        ppm_pixel_set(&img->data[i], R, G, B);
    }
    return img;

}

PPMImage* ppm_image_new_blank(const int width, const int height){
    return ppm_image_init(width, height, 0, 0, 0);
}

PPMImage* ppm_image_set_pixel(PPMImage* image, int x, int y, int R, int G, int B){
    if (!image){
        fprintf(stderr, "ppm_image_set_pixel: Image was NULL\n");
        BREAK_OUT(1);
    }
    if (!image->data){
        fprintf(stderr, "ppm_image_set_pixel: Image data was NULL\n");
        BREAK_OUT(1);
    }

    if (x < 0 || x >= image->x || y < 0 || y >= image->y){
        fprintf(stderr, "ppm_image_set_pixel: X and/or Y value were out of bounds\n");
        BREAK_OUT(1);
    }

    if (
            R < 0 || R > PIXEL_COLOR_VALUE ||
            G < 0 || G > PIXEL_COLOR_VALUE ||
            B < 0 || B > PIXEL_COLOR_VALUE){
        fprintf(stderr, "ppm_image_set_pixel: RGB values were out of bounds\n");
        BREAK_OUT(1);
    }

    ppm_pixel_set_unsafe(&image->data[y * image->x + x], R, G, B);
    return image;
}


bool has_ppm_extension(char* fp){
    if (!fp){
        fprintf(stderr, "has_ppm_extension: Filepath was NULL\n");
        BREAK_OUT(1);
    }

    size_t len = strlen(fp);
    if (len > 4){
        const char* last4 = fp + len - 4;
        if (strcasecmp(last4, ".ppm") == 0) {
            return true;
        }
    }

    return false;
}

PPMImage* ppm_image_load(char* fp){
    char buff[16];

    if (!has_ppm_extension(fp)){
        fprintf(stderr, "ppm_image_load: File didn't end in PPM or was not long enough\n");
        BREAK_OUT(1);
    }

    FILE *file = fopen(fp, "rb");
    if (!file){
        fprintf(stderr, "ppm_image_load: Unable to open file at: %s\n", fp);
        BREAK_OUT(1);
    }

    // Read and Check image format
    if (!fgets(buff, sizeof(buff), file)){
        fprintf(stderr, "ppm_image_load: Unable to read file\n");
        BREAK_OUT(1);
    }

    if (buff[0] != 'P' || buff[1] != '6'){
        fprintf(stderr, "ppm_image_load: Invalid Image format (must be P6)\n");
        BREAK_OUT(1);
    }

    int c, x, y, rgb_comp_colour;
    // Check for comments
    c = getc(file);
    while (c == '#') {
        while (getc(file) != '\n') {}
        c = getc(file);
    }
    ungetc(c, file);

    // Read image size information
    if (fscanf(file, PPM_SIZE_FORMAT, &x, &y) != 2 || x <= 0 || y <= 0){
        fprintf(stderr, "ppm_image_load: Invalid image size\n");
        BREAK_OUT(1);
    }

    // Read RGB Component
    if (fscanf(file, PPM_RGB_FORMAT, &rgb_comp_colour) != 1){
        fprintf(stderr, "ppm_image_load: Invalid RGB Component\n");
        BREAK_OUT(1);
    }

    // Check RGB Component depth
    if (rgb_comp_colour != PIXEL_COLOR_VALUE){
        fprintf(stderr, "ppm_image_load: File does not contain 8-bit components\n");
        BREAK_OUT(1);
    }

    while (fgetc(file) != '\n') {}

    // Memory allocation for the image
    PPMImage *img = ppm_image_new_blank(x, y);
    if (!img || !img->data){
        fprintf(stderr, "ppm_image_load: Unable to allocate memory for image\n");
        BREAK_OUT(1);
    }

    // Load image data
    if (fread(img->data, 3 * img->x, img->y, file) != img->y){
        fprintf(stderr, "ppm_image_load: Unable to load image data\n");
        BREAK_OUT(1);
    } 

    fclose(file);
    return img;

}


int ppm_image_compare(const PPMImage* img1, const PPMImage* img2){
    int total, lim;
    lim = img1->x * img1->y;
    total = 0;

    for(int i = 0; i < lim; i++){
        const PPMPixel* p1 = &img1->data[i];
        const PPMPixel* p2 = &img2->data[i];
        total += ppm_pixel_compare(p1, p2);
    }
    return total;
}


void ppm_image_save(PPMImage* image, char* fp){
    if (!image){
        fprintf(stderr, "ppm_image_save: Image was NULL\n");
        BREAK_OUT_VOID(1);
    }

    if (!fp){
        fprintf(stderr, "ppm_image_save: Filepath was NULL\n");
        BREAK_OUT_VOID(1);
    }

    if (!has_ppm_extension(fp)){
        fprintf(stderr, "ppm_image_save: Filepath didn't end in PPM or was not long enough");
        BREAK_OUT_VOID(1);
    }

    FILE *file;

    file = fopen(fp, "wb");
    if (!file){
        fprintf(stderr, "ppm_image_save: File was unable to be created");
        BREAK_OUT_VOID(1);
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
        BREAK_OUT_VOID(1);
    }
    
    if (image->data){ 
        free(image->data);
    }
    free(image);
}
