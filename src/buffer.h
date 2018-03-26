
#ifndef H_BUFFER
#define H_BUFFER

#include "matrix.h"

class Buffer {
    protected:
        Matrix *points;
        Matrix *transform;
        int pointCount;
        void add_transform(float* trans_mat);
    public:
        Buffer();
        ~Buffer();
        void addPoint(float x, float y, float z = 0);
        void transformSetIdentity();
        void translate(float dx, float dy, float dz);
        void scale(float sx, float sy, float sz);
        void rotate_x(float theta);
        void rotate_y(float theta);
        void rotate_z(float theta);
        void apply();
        int getPointCount() {return pointCount;}
        Matrix *getPoints() {return points;}
};

#endif
