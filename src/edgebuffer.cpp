//#include<stdio.h>
#include<math.h>

#include "edgebuffer.h"

EdgeBuffer::EdgeBuffer() {
    points = new Matrix(0);
    transform = new Matrix(4,4);
    transformSetIdentity();
    pointCount = 0;
}

EdgeBuffer::~EdgeBuffer() {
    delete points;
    delete transform;
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


//void EdgeBuffer::transform(Matrix *transform) {
//    points->multiply(transform);
//}

// Helper function for ease of use
void EdgeBuffer::add_transform(float* trans_mat) {
    Matrix *trans = new Matrix(4,4, trans_mat);
    transform->multiply(trans);
    delete trans;
}

void EdgeBuffer::apply() {
    points->multiply(transform);
}

void EdgeBuffer::transformSetIdentity() {
    transform->fillWithIdentity();
}

void EdgeBuffer::translate(float dx, float dy, float dz) {

    float trans_mat[] = {
            1, 0, 0, dx,
            0, 1, 0, dy,
            0, 0, 1, dz,
            0, 0, 0, 1
        };
    add_transform(trans_mat);
}

void EdgeBuffer::scale(float sx, float sy, float sz) {

    float trans_mat[] = {
            sx, 0, 0, 0,
            0, sy, 0, 0,
            0, 0, sz, 0,
            0, 0, 0, 1
        };
    add_transform(trans_mat);
}

void EdgeBuffer::rotate_x(float theta) {
    theta *= M_PI / 180.0f;
    float c = cos(theta);
    float s = sin(theta);
    float trans_mat[] = {
            //s, c,  0, 0,
            //0, 0,  1, 0,
            //c, -s, 0, 0,
            //0, 0,  0, 1
            1, 0,  1, 0,
            0, c,  s, 0,
            0, -s, c, 0,
            0, 0,  0, 1
        };
    add_transform(trans_mat);

}

void EdgeBuffer::rotate_y(float theta) {
    theta *= M_PI / 180.0f;
    float c = cos(theta);
    float s = sin(theta);
    float trans_mat[] = {
            //0, 0,  1, 0,
            //c, -s, 0, 0,
            //s, c,  0, 0,
            //0, 0,  0, 1
            c, 0, s, 0,
            0, 1, 0,  0,
            -s, 0, c,  0,
            0, 0, 0,  1
        };
    add_transform(trans_mat);
}


void EdgeBuffer::rotate_z(float theta) {
    theta *= M_PI / 180.0f;
    float c = cos(theta);
    float s = sin(theta);
    float trans_mat[] = {
            c, s, 0, 0,
            -s, c,  0, 0,
            0, 0,  1, 0,
            0, 0,  0, 1
        };
    add_transform(trans_mat);

}
