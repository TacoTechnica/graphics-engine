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


// Utility function: Returns random number from 0 to n
double randi(double n) {
    return n * (double)rand() / (double)RAND_MAX;
}

// Nice static filter
void static_filter(struct Pixel *pixel, double mix) {
    pixel->r = pixel->r * (1.0 - mix) + randi(255)*mix;
    pixel->g = pixel->g * (1.0 - mix) + randi(255)*mix;
    pixel->b = pixel->b * (1.0 - mix) + randi(255)*mix;
}

// Tries to draw a circle-ish gradient at that point
void try_circle_ish(struct Pixel *pixel, int x, int y, int circle_x, int circle_y) {
    double distSqr = 0.5*(x-circle_x)*(x-circle_x) + (y-circle_y)*(y-circle_y);
    distSqr = pow(distSqr,0.5);
    double r = 2;
    double intensity = (r - distSqr)/r;
    if (intensity < 0) return;
    pixel->r = 255 * intensity;
    pixel->g = 125 * intensity;
    pixel->b = 125 * intensity;
}


// Temporary. This is the plane that's on the ground (checkerboard pattern)
void get_plane_col(struct Pixel *pixel, int x, int y, double distance_squared) {
    // Distance squared adds a fog effect
    double dist_fact = pow(distance_squared / 100000.0, 0.2);
    dist_fact = fmax(fmin(1.0 - dist_fact, 1), 0);

    if (((int)(x/10) + (int)(y/10)) % 2 == 0) {
        pixel->r = 175;
        pixel->g = 90;
        pixel->b = 90;
    } else {
        pixel->r = 90;
        pixel->g = 90;
        pixel->b = 175;
    }

    /*
    pixel->r *= dist_fact;
    pixel->g *= dist_fact;
    pixel->b *= dist_fact;
    */
}

// Bad math. Supposed to mimic perspective, but it failed. I forget how perspective works...
void ray_cast_ish(struct Pixel *pixel, double height, double thetaX, double thetaY) {
    // If we're not looking at the plane (above the horizon)
    if (thetaY <= 0) {
        pixel->r = 0;
        pixel->g = 0;
        pixel->b = 0;
        return;
    }

    // Makes it wobbly in the X axis
    //thetaX += randi(M_PI/24);

    double r = height * tan(M_PI/2 - thetaY);
    double y = r * cos(thetaX);
    double x = r * sin(thetaX);

    //double y = height * tan(M_PI/2 - thetaY);
    //double x = y * tan(thetaX);
    //y / tan(M_PI/2 - thetaX);
    get_plane_col( pixel, (int)x, (int)y, y*y+x*x+height*height );
}

int main() {
    Image img(320,320);

    double cam_fov_w = 90.0 * M_PI/180.0;
    double cam_fov_h = 90.0 * M_PI/180.0;

    double projection_plane_distance = img.getWidth() / (2.0 * tan(cam_fov_w / 2.0));

    // Grab every pixel from the image and change it
    int xx, yy;
    for(xx = 0; xx < img.getWidth(); xx++) {
        for(yy = 0; yy < img.getHeight(); yy++) {
            struct Pixel *currentPixel = img.getPixel(xx, yy);
            double w = img.getWidth();
            double h = img.getHeight();

            double thetaX = M_PI/4.0 + atan((double)xx / projection_plane_distance);//cam_fov_w * ((double)xx - w/2)/w;
            double thetaY = atan((double)yy / projection_plane_distance);//cam_fov_h * ((double)yy - h/2)/h;
            //printf("(%f, %f)\n", thetaX, thetaY);
            ray_cast_ish(currentPixel, 40, thetaX, thetaY);
            //try_circle_ish(currentPixel, xx, yy, (int)w/2 - 10, (int)h / 2);
            //try_circle_ish(currentPixel, xx, yy, (int)w/2, (int)h / 2);
            static_filter(currentPixel, 0.1);
        }
    }

    Image::writeToPPM(&img, "sp00k.ppm");

    return 0;
}
