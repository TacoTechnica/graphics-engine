/** Edge Buffer
 *
 *      Holds a bunch of lines in a matrix with transformations
 *
 */

#ifndef H_EDGEBUFFER
#define H_EDGEBUFFER

#include "matrix.h"

class EdgeBuffer {
    private:
        Matrix *points;
        Matrix *transform;
        int pointCount;
        void add_transform(float* trans_mat);
        const int PARAMETRIC_ACCURACY = 30;// # of lines in a curve
    public:
        EdgeBuffer();
        ~EdgeBuffer();
        void addPoint(float x, float y, float z = 0);
        void addEdge(float x0, float y0, float z0, float x1, float y1, float z1);
        void addCircle(float cx, float cy, float cz, float r);
        void addHermite(float x0, float y0, float x1, float y1, float dx0, float dy0, float dx1, float dy1);
        void addBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3);
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
