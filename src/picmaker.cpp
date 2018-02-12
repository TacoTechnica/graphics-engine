#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>

#include<math.h>

// This is what a pixel is made of.
struct Pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

// This image holds pixels
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

void plot(Image *img, int x, int y, struct Pixel *color) {
    memcpy(img->getPixel(x,y), color, sizeof(struct Pixel));
}


void draw_line(Image *img, int x0, int y0, int x1, int y1, struct Pixel *color) {
    // swapping
    int swap;
    if (x0 > x1) {
        swap = x0;
        x0 = x1;
        x1 = swap;
        swap = y0;
        y0 = y1;
        y1 = swap;
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int a = dy; // TODO: You can remove a and b...
    int b = -dx;
    int d;

    if (dy >= 0) { // Quadrant 1 and 3
        // Octant 1 and 5
        if (dy <= dx) {
            d = 2*a + b;
            while(x0 <= x1) {
                plot(img, x0, y0, color);
                if (d > 0) {
                    y0++;
                    d += 2*b;
                }
                x0++;
                d += 2*a;
            }
        // Octant 2 and 6
        } else {
            d = a + 2*b;
            while(y0 <= y1) {
                plot(img, x0, y0, color);
                if (d < 0) {
                    x0++;
                    d += 2*a;
                }
                y0++;
                d += 2*b;
            }
            
        }
        return;
    } else { // Quadrant 2 and 4 
        // Octant 4 and 8
        if (-dy <= dx) {
            d = -2*a + b;
            while(x0 <= x1) {
                plot(img, x0, y0, color);
                if (d > 0) {
                    y0--;
                    d += 2*b;
                }
                x0++;
                d -= 2*a;
            }
        // Octant 2 and 6
        } else {
            d = -a + 2*b;
            while(y0 >= y1) {
                plot(img, x0, y0, color);
                if (d < 0) {
                    x0++;
                    d -= 2*a;
                }
                y0--;
                d += 2*b;
            }
        }
        return;
    }
}

void draw_object_ish(Image *img, float *coordinates, int size, int px, int py, struct Pixel *color) {
    float SCALE_FACTOR = 7;
    int coordinate_size = size / 3;
    int i;

    while(i < coordinate_size) {
        float x = coordinates[3*i];
        float y = -1 * coordinates[3*i + 1];
        float z = coordinates[3*i + 2];

        int real_x = (int) (x * SCALE_FACTOR + px);
        int real_y = (int) (y * SCALE_FACTOR + py);

        i++;

        float xN = coordinates[3*i];
        float yN = -1 * coordinates[3*i + 1];
        float zN = coordinates[3*i + 2];

        int real_xN = (int) (xN * SCALE_FACTOR + px);
        int real_yN = (int) (yN * SCALE_FACTOR + py);

        i++;

        draw_line(img, real_x, real_y, real_xN, real_yN, color);
    }
}


int main() {
    Image img(420,320);

    struct Pixel p = {255, 255, 255};

    // Das a lot of space, kinda arbitrary
    float coords[64895 / 20];
    
    char letters[64895]; // Should use stat to grab file size, but I'm lazy

    printf("reading and opening file...\n");
    int fd = open("res/utah-teapot.obj", O_RDONLY);
    read(fd, letters, sizeof(letters));

    close(fd);

    printf("Done reading file, now storing data...\n");

    char *current_string;
    int index = 0;
    // Get rid of the V at the beginning and start the split chain
    strtok(letters, " ");
    while( (current_string = strtok(NULL, " ")) != NULL ) {

        coords[index] = atof(current_string);
        printf("current float: %lf\n", coords[index]);

        index++;
    }

    draw_object_ish(&img, coords, index, img.getWidth() / 2, img.getHeight() / 2, &p);

    /*
    double theta;
    double r = 100.0;
    for(theta = 0; theta <= 2.0 * 3.1415; theta += 3.1415 / 10) {
        draw_line(&img, 150, 150, 150 + (int)(r * cos(theta)), 150 + (int)(r * sin(theta)), &p);
    }
    */

    Image::writeToPPM(&img, "sp00k.ppm");

    return 0;
}
