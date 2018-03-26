#include<stdio.h>
#include<math.h>

#include "buffer.h"
#include "matrix.h"

Buffer::Buffer() {
    points = new Matrix(0);
    transform = new Matrix(4,4);
    transformSetIdentity();
    pointCount = 0;
}
Buffer::~Buffer() {
    delete points;
    delete transform;
}

void Buffer::addPoint(float x, float y, float z) {
    pointCount++;
    points->growColumns(pointCount);
    *points->get(pointCount - 1, 0) = x;
    *points->get(pointCount - 1, 1) = y;
    *points->get(pointCount - 1, 2) = z;
    *points->get(pointCount - 1, 3) = 1;
}

// Adds all the edges from a matrix
// Basically appends this matrix to our own point matrix
void Buffer::addPoints(Matrix *m) {
    int col;
    for(col = 0; col < m->getNumColumns(); col++) {
        addPoint(*m->get(col, 0), *m->get(col, 1), *m->get(col, 2));
    }
}

// Helper function for ease of use
void Buffer::add_transform(float* trans_mat) {
    Matrix *trans = new Matrix(4,4, trans_mat);
    transform->multiply(trans);
    delete trans;
}

void Buffer::apply() {
    points->multiply(transform);
}

void Buffer::transformSetIdentity() {
    transform->fillWithIdentity();
}

void Buffer::translate(float dx, float dy, float dz) {

    float trans_mat[] = {
            1, 0, 0, dx,
            0, 1, 0, dy,
            0, 0, 1, dz,
            0, 0, 0, 1
        };
    add_transform(trans_mat);
}

void Buffer::scale(float sx, float sy, float sz) {

    float trans_mat[] = {
            sx, 0, 0, 0,
            0, sy, 0, 0,
            0, 0, sz, 0,
            0, 0, 0, 1
        };
    add_transform(trans_mat);
}

void Buffer::rotate_x(float theta) {
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

void Buffer::rotate_y(float theta) {
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


void Buffer::rotate_z(float theta) {
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
