
#ifndef H_RENDERER
#define H_RENDERER

#include "image.h"
#include "edgebuffer.h"
#include "trianglebuffer.h"

class Renderer {
    private:
        Color color;
        Image *img;
        Matrix *zbuffer;
    public:
        Renderer(Image *img);
        ~Renderer() {
            delete zbuffer;
        }
        void plot(int x, int y, float z);
        void drawLine(int x0, int y0, float z0, int x1, int y1, float z1);
        void setColor(Color color) {this->color = color;}
        void refill();
        Color getColor() {return color;}
        Image *getImage() {return img;}
        
        void drawEdgeBufferLines(EdgeBuffer *buffer);
        void drawTriangleBufferMesh(TriangleBuffer *buffer);

        void clearZBuffer();
};

#endif
