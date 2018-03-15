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

void EdgeBuffer::addCircle(float cx, float cy, float cz, float r) {
    int t;
    for(t = 0; t < PARAMETRIC_ACCURACY; t++) {
        double theta = M_PI*2.0*(double)(t)/ PARAMETRIC_ACCURACY;
        double xx = cx + r * cos(theta);
        double yy = cy + r * sin(theta);
        addPoint(xx, yy, cz);
        if (t != 0) {
            addPoint(xx, yy, cz); // Second point in the same place
        }
    }
    // Complete the circle
    addPoint(cx + r, cy, cz);
}

// TODO: Hermite and Bezier are basically the same, with minor changes. Reduce copying
void EdgeBuffer::addHermite(float x0, float y0, float x1, float y1, float dx0, float dy0, float dx1, float dy1) {
    float data_mat[] = {
        x0,  y0,
        x1,  y1,
        dx0, dy0,
        dx1, dy1
    };
    Matrix *data = new Matrix(2, 4, data_mat);
    float transform_mat[] = {
         2, -2,  1,  1,
        -3,  3, -2, -1,
         0,  0,  1,  0,
         1,  0,  0,  0
    };
    Matrix *transform = new Matrix(4,4,transform_mat);
    data->multiply(transform);

    float ax, bx, cx, dx,
          ay, by, cy, dy;
    ax = *(data->get(0,0));
    bx = *(data->get(0,1));
    cx = *(data->get(0,2));
    dx = *(data->get(0,3));
    ay = *(data->get(1,0));
    by = *(data->get(1,1));
    cy = *(data->get(1,2));
    dy = *(data->get(1,3));

    float zz = 0;
    int i;
    for(i = 0; i < PARAMETRIC_ACCURACY; i++) {
        float t = (float)i/PARAMETRIC_ACCURACY;
        float t3 = t*t*t;
        float t2 = t*t;
        float xx = ax*t3 + bx*t2 + cx*t + dx;
        float yy = ay*t3 + by*t2 + cy*t + dy;
        addPoint(xx, yy, zz);
        if (t != 0) {
            addPoint(xx, yy, zz); // Second point in the same place
        }
    }
}

void EdgeBuffer::addBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) {
    float data_mat[] = {
        x0, y0,
        x1, y1,
        x2, y2,
        x3, y3
    };
    Matrix *data = new Matrix(2, 4, data_mat);
    float transform_mat[] = {
        -1,  3, -3,  1,
         3, -6,  3,  0,
        -3,  3,  0,  0,
         1,  0,  0,  0
    };
    Matrix *transform = new Matrix(4,4,transform_mat);
    data->multiply(transform);

    float ax, bx, cx, dx,
          ay, by, cy, dy;
    ax = *(data->get(0,0));
    bx = *(data->get(0,1));
    cx = *(data->get(0,2));
    dx = *(data->get(0,3));
    ay = *(data->get(1,0));
    by = *(data->get(1,1));
    cy = *(data->get(1,2));
    dy = *(data->get(1,3));

    float zz = 0;

    int i;
    for(i = 0; i < PARAMETRIC_ACCURACY; i++) {
        float t = (float)i/PARAMETRIC_ACCURACY;

        float t3 = t*t*t;
        float t2 = t*t;
        float xx = ax*t3 + bx*t2 + cx*t + dx;
        float yy = ay*t3 + by*t2 + cy*t + dy;
        addPoint(xx, yy, zz);
        if (t != 0) {
            addPoint(xx, yy, zz); // Second point in the same place
        }
    }
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
