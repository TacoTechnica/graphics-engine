#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>

#include "image.h"

Image::Image(int width, int height, int pixel_resolution) {
    this->width = width;
    this->height = height;
    this->pixel_resolution = pixel_resolution;
    pixels = (struct Color*)calloc(width*height, sizeof(struct Color)); 
    // ^ calloc because we want our image to start empty
}

Image::~Image() {
    free(pixels);
}

// Takes an image and writes it to a PPM File
void Image::writeToPPM(Image *img, const char* name) {
    int fd = open(name, O_CREAT | O_WRONLY, 0644);

    // Place header at top of image
    char header[16];
    sprintf(header, "P3\n%d %d\n%d\n", img->getWidth(), img->getHeight(), img->getPixelResolution());
    write(fd, header, strlen(header));
    

    // Fill the rest of the image with pixel data
    char pixel_buffer[12]; // 12 is the maximum size (ex. "255 255 255 ")
    int xx, yy;
    for(yy = 0; yy < img->getHeight(); yy++) {
        for(xx = 0; xx < img->getWidth(); xx++) {
            struct Color *currentPixel = img->getPixel(xx, yy);
            sprintf(pixel_buffer, "%d %d %d ", 
                    currentPixel->r, 
                    currentPixel->g,
                    currentPixel->b
                    );
            write(fd, pixel_buffer, strlen(pixel_buffer));
        }
    }
}
