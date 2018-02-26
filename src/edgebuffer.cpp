
//#include<stdio.h>
#include "edgebuffer.h"

EdgeBuffer::EdgeBuffer() {
    points = new Matrix(0);
    pointCount = 0;
}

EdgeBuffer::~EdgeBuffer() {
    delete points;
}

void EdgeBuffer::addPoint(float x, float y, float z) {
    pointCount++;
    points->growColumns(pointCount);
    *points->get(pointCount - 1, 0) = x;
    *points->get(pointCount - 1, 1) = y;
    *points->get(pointCount - 1, 2) = z;
    *points->get(pointCount - 1, 3) = 1;
}

void EdgeBuffer::addEdge(float x0, float y0, float z0, float x1, float y1, float z1) {
    addPoint(x0, y0, z0);
    addPoint(x1, y1, z1);
}

void EdgeBuffer::transform(Matrix *transform) {
    points->multiply(transform);
}

// Helper function for ease of use
void EdgeBuffer::transform(float* trans_mat) {
    Matrix *trans = new Matrix(4,4, trans_mat);
    transform(trans);
    delete trans;
}

void EdgeBuffer::translate(float dx, float dy, float dz) {

    float trans_mat[] = {
            1, 0, 0, dx,
            0, 1, 0, dy,
            0, 0, 1, dz,
            0, 0, 0, 1
        };
    transform(trans_mat);
}

void EdgeBuffer::scale(float sx, float sy, float sz) {

    float trans_mat[] = {
            sx, 0, 0, 0,
            0, sy, 0, 0,
            0, 0, sz, 0,
            0, 0, 0, 1
        };
    transform(trans_mat);
}
