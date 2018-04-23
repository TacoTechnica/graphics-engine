#include<stdio.h>
#include<math.h>

#include "buffer.h"
#include "matrix.h"
#include "stack.h"

Buffer::Buffer(Matrix *m) {
    points = m;
    transformations = new Stack<Matrix>();
    
    Matrix *global = new Matrix(4,4);
    global->fillWithIdentity();
    transformations->push(global);

    //transform = new Matrix(4,4);
    transformSetIdentity();
    pointCount = 0;
}

Buffer::Buffer() : Buffer(new Matrix(0)) {
    //points = new Matrix(0);
    //transform = new Matrix(4,4);
    //transformSetIdentity();
    //pointCount = 0;
}


Buffer::~Buffer() {
    delete points;
    delete transformations;
}

void Buffer::addPoint(float x, float y, float z) {

    // Put the point in this matrix (like a vector)
    // and apply the transformation to the point
    Matrix *temp = new Matrix(1);
    *temp->get(0, 0) = x;
    *temp->get(0, 1) = y;
    *temp->get(0, 2) = z;
    *temp->get(0, 3) = 1;
    
    temp->multiply(transformations->peek());

    // Then copy the point on to the points matrix
    pointCount++;
    points->growColumns(pointCount);

    int i;
    for(i = 0; i < temp->getNumRows(); i++) {
        *points->get(pointCount - 1, i) = *temp->get(0, i);
    }

//    *points->get(pointCount - 1, 0) = x;
//    *points->get(pointCount - 1, 1) = y;
//    *points->get(pointCount - 1, 2) = z;
//    *points->get(pointCount - 1, 3) = 1;

    delete temp;
}

// Adds all the points from a matrix
// and applies the current transformation when adding this shape
void Buffer::addPoints(Matrix *m) {

    int col;
    for(col = 0; col < m->getNumColumns(); col++) {
        addPoint(*m->get(col, 0), *m->get(col, 1), *m->get(col, 2));
    }
}

// Helper function for ease of use
void Buffer::add_transform(float* trans_mat) {
    Matrix *trans = new Matrix(4,4, trans_mat);

    // Multiply trans mat by our current transformation, and replace
    // our current transformation mat with that new mat
    trans->multiply(transformations->peek());
    delete transformations->pop();
    transformations->push(trans);

    //transformations->peek()->multiply(trans);
    //delete trans;
}

void Buffer::transformSetIdentity() {
    transformations->peek()->fillWithIdentity();
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
            1, 0,  0, 0,
            0, c,  s, 0, // positive
            0, -s, c, 0, // negative
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

void Buffer::transformPush() {
    Matrix *copy = new Matrix(4);
    transformations->peek()->copyTo(copy);
    transformations->push(copy);
}

void Buffer::transformPop() {
    if (transformations->getSize() > 1) {
        Matrix *mat = transformations->pop();
        delete mat;
    }
}
