
#ifndef H_TRIANGLEBUFFER
#define H_TRIANGLEBUFFER

#include "matrix.h"
#include "buffer.h"

class TriangleBuffer : public Buffer {
    public:
        TriangleBuffer() : Buffer() {};
        TriangleBuffer(Matrix *m) : Buffer(m) {};
        void addTriangle(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2);
        void addTriangles(Matrix *m) { Buffer::addPoints(m); }
        static Matrix *genBox(float x, float y, float z, float xlength, float ylength, float zlength);
        void addBox(float x, float y, float z, float xlength, float ylength, float zlength);
        static Matrix *genSphere(float x, float y, float z, float r);
        void addSphere(float x, float y, float z, float r);
        static Matrix *genTorus(float x, float y, float z, float rCircle, float rTorus);
        void addTorus(float x, float y, float z, float rCircle, float rTorus);
};

#endif
