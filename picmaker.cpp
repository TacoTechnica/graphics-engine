#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>


struct Pixel {
    char r;
    char g;
    char b;
};


class Image {
    private:
        int width;
        int height;
        struct Pixel *pixels;
    public:
        Image(int width, int height) {
            this->width = width;
            this->height = height;
            pixels = (struct Pixel*)malloc(width*height*sizeof(struct Pixel));
        }
        ~Image();

        int getWidth() {return width;}
        int getHeight() {return height;}
        struct Pixel *getPixels() { return pixels; }
        struct Pixel *getPixel(int x, int y) { return &pixels[x + y*width]; }

        static void writeToPPM(Image *img, const char* name);
};

Image::~Image() {
    free(pixels);
}

void Image::writeToPPM(Image *img, const char* name) {
    int fd = open(name, O_CREAT | O_WRONLY, 0644);
    char header[16];
    sprintf(header, "P3\n%d %d\n", img->getWidth(), img->getHeight());
    write(fd, header, strlen(header));
    
    //close(fd);
}


int main() {
    Image img(3,4);

    printf("Image: %dx%d\n", img.getWidth(), img.getHeight());
    Image::writeToPPM(&img, "hi.ppm");

    return 0;
}
