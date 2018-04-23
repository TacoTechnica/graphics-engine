
#ifndef H_BUFFER
#define H_BUFFER

#include "matrix.h"
#include "stack.h"

class Buffer {
    protected:
        Matrix *points;
        Stack<Matrix> *transformations;
        //Matrix *transform;
        int pointCount;
        void add_transform(float* trans_mat);
        static const int PARAMETRIC_ACCURACY = 20;//30;// # of lines in a curve
    public:
        Buffer();
        Buffer(Matrix *m);
        ~Buffer();
        void addPoint(float x, float y, float z = 0);
        void addPoints(Matrix *m);
        void transformSetIdentity();
        void translate(float dx, float dy, float dz);
        void scale(float sx, float sy, float sz);
        void rotate_x(float theta);
        void rotate_y(float theta);
        void rotate_z(float theta);
        void transformPush();
        void transformPop();
        int getPointCount() {return pointCount;}
        Matrix *getPoints() {return points;}
};

#endif
