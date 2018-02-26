
#ifndef H_IMAGE
#define H_IMAGE

// This is what a pixel is made of.
struct Color {
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
        struct Color *pixels;
    public:
        Image(int width, int height, int pixel_resolution = 255);
        ~Image();
        int getWidth() {return width;}
        int getHeight() {return height;}
        int getPixelResolution() {return pixel_resolution;}
        struct Color *getPixels() { return pixels; }
        struct Color *getPixel(int x, int y) { return &pixels[x + y*width]; }

        static void writeToPPM(Image *img, const char* name);
};

#endif
