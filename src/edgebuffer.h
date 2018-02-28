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
    public:
        EdgeBuffer();
        ~EdgeBuffer();
        void addPoint(float x, float y, float z = 0);
        void addEdge(float x0, float y0, float z0, float x1, float y1, float z1);
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
