#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>

#include<math.h>

struct Pixel {
    char r;
    char g;
    char b;
};


class Image {
    private:
        int width;
        int height;
        int pixel_resolution; // 255 by default
        struct Pixel *pixels;
    public:
        Image(int width, int height, int pixel_resolution = 255) {
            this->width = width;
            this->height = height;
            this->pixel_resolution = pixel_resolution;
            pixels = (struct Pixel*)calloc(width*height, sizeof(struct Pixel)); 
            // ^ calloc because we want our image to start empty
        }
        ~Image();

        int getWidth() {return width;}
        int getHeight() {return height;}
        int getPixelResolution() {return pixel_resolution;}
        struct Pixel *getPixels() { return pixels; }
        struct Pixel *getPixel(int x, int y) { return &pixels[x + y*width]; }

        static void writeToPPM(Image *img, const char* name);
};

Image::~Image() {
    free(pixels);
}

void Image::writeToPPM(Image *img, const char* name) {
    int fd = open(name, O_CREAT | O_WRONLY, 0644);

    // Place header at top of image
    char header[16];
    sprintf(header, "P3\n%d %d\n%d\n", img->getWidth(), img->getHeight(), img->getPixelResolution());
    write(fd, header, strlen(header));
    

    // Fill the rest of the image with pixel data
    char pixel_buffer[12]; // 12 is the maximum size (ex. "255 255 255 ")
    int xx, yy;
    for(xx = 0; xx < img->getWidth(); xx++) {
        for(yy = 0; yy < img->getHeight(); yy++) {
            struct Pixel *currentPixel = img->getPixel(xx, yy);
            sprintf(pixel_buffer, "%d %d %d ", 
                    currentPixel->r, 
                    currentPixel->g,
                    currentPixel->b
                    );
            write(fd, pixel_buffer, strlen(pixel_buffer));
        }
    }
}


int main() {
    Image img(32,32);

    int xx, yy;
    for(xx = 0; xx < img.getWidth(); xx++) {
        for(yy = 0; yy < img.getHeight(); yy++) {
            struct Pixel *currentPixel = img.getPixel(xx, yy);

            int dx = xx - img.getWidth()/2;
            int dy = yy - img.getHeight()/2;
            double distanceSqr = dx*dx + dy*dy;
            int val = fmax(fmin( (int) (distanceSqr), 255 ), 0);

            currentPixel->r = 255 - val;
            currentPixel->g = 125;
            currentPixel->b = 125;
        }
    }

    printf("Image: %dx%d\n", img.getWidth(), img.getHeight());
    Image::writeToPPM(&img, "hi.ppm");

    return 0;
}
