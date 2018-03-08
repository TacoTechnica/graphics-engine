
#ifndef H_RENDERER
#define H_RENDERER

#include "image.h"
#include "edgebuffer.h"

class Renderer {
    private:
        Color color;
        Image *img;
    public:
        Renderer(Image *img) {this->img = img;}
        void plot(int x, int y);
        void drawLine(int x0, int y0, int x1, int y1);
        void fillRect(int x, int y, int w, int h);
        void setColor(Color color) {this->color = color;}
        Color getColor() {return color;}
        Image *getImage() {return img;}

        void drawEdgeBufferLines(EdgeBuffer *buffer);
};

#endif
