#include<stdio.h>
#include<math.h>

#include "edgebuffer.h"


//EdgeBuffer::EdgeBuffer() {
//    points = new Matrix(0);
//    transform = new Matrix(4,4);
//    transformSetIdentity();
//    pointCount = 0;
//}
//
//EdgeBuffer::~EdgeBuffer() {
//    delete points;
//    delete transform;
//}
//
//void EdgeBuffer::addPoint(float x, float y, float z) {
//    pointCount++;
//    points->growColumns(pointCount);
//    *points->get(pointCount - 1, 0) = x;
//    *points->get(pointCount - 1, 1) = y;
//    *points->get(pointCount - 1, 2) = z;
//    *points->get(pointCount - 1, 3) = 1;
//}

void EdgeBuffer::addEdge(float x0, float y0, float z0, float x1, float y1, float z1) {
    addPoint(x0, y0, z0);
    addPoint(x1, y1, z1);
}

void EdgeBuffer::addPointyPoint(float x, float y, float z) {
    float d = 1;
    addEdge(x-d/2, y-d/2, z-d/2, x+d/2, y+d/2, z+d/2);
}

// Adds all the edges from a matrix
// Basically appends this matrix to our own point matrix
void EdgeBuffer::addEdges(Matrix *m) {
    int col;
    for(col = 0; col < m->getNumColumns(); col++) {
        addPoint(*m->get(col, 0), *m->get(col, 1), *m->get(col, 2));
    }
}

Matrix *EdgeBuffer::genBox(float x, float y, float z, float xlength, float ylength, float zlength) {
	EdgeBuffer *boxBuff = new EdgeBuffer();

    // Front face
    boxBuff->addEdge(x,y,z, x+xlength,y,z);
    boxBuff->addEdge(x+xlength,y,z, x+xlength,y,z+zlength);
    boxBuff->addEdge(x+xlength,y,z+zlength, x,y,z+zlength);
    boxBuff->addEdge(x,y,z+zlength, x,y,z);

    // Back face
    boxBuff->addEdge(x,y+ylength,z, x+xlength,y+ylength,z);
    boxBuff->addEdge(x+xlength,y+ylength,z, x+xlength,y+ylength,z+zlength);
    boxBuff->addEdge(x+xlength,y+ylength,z+zlength, x,y+ylength,z+zlength);
    boxBuff->addEdge(x,y+ylength,z+zlength, x,y+ylength,z);

    // In betweeners
    boxBuff->addEdge(x,y,z, x,y+ylength,z);
    boxBuff->addEdge(x+xlength,y,z, x+xlength,y+ylength,z);
    boxBuff->addEdge(x,y,z+zlength, x,y+ylength,z+zlength);
    boxBuff->addEdge(x+xlength,y,z+zlength, x+zlength,y+ylength,z+zlength);

    printf("break 1\n");
    // Copy over the Box Edge Buffer's matrix and dispose of the buffer
    Matrix *toCopy = new Matrix(0);
    printf("break 2\n");
    boxBuff->getPoints()->copyTo(toCopy);
    printf("break 3\n");
    delete boxBuff;

    return toCopy;
}

void EdgeBuffer::addBox(float x, float y, float z, float xlength, float ylength, float zlength) {
    Matrix *boxMat = genBox(x,y,z,xlength,ylength,zlength);
    addEdges(boxMat);
    delete boxMat;
}

Matrix *EdgeBuffer::genSphere(float x, float y, float z, float r) {
    EdgeBuffer *sphereBuff = new EdgeBuffer();

    int theta_count, phi_count;
    for(phi_count = 0; phi_count < PARAMETRIC_ACCURACY; phi_count++) {
        double phi = M_PI*2.0*(double)(phi_count) / PARAMETRIC_ACCURACY;
        for(theta_count = 0; theta_count <= PARAMETRIC_ACCURACY; theta_count++) {
            double theta = M_PI*(double)(theta_count) / PARAMETRIC_ACCURACY;

            double px = r*cos(theta);
            double py = r*sin(theta)*cos(phi);
            double pz = r*sin(theta)*sin(phi);

            addPointyPoint(x + px,y + py,z + pz);
        }
    }

    // Copy over the Box Edge Buffer's matrix and dispose of the buffer
    Matrix *toCopy = new Matrix(0);
    sphereBuff->getPoints()->copyTo(toCopy);
    delete sphereBuff;

    return toCopy;
}

void EdgeBuffer::addSphere(float x, float y, float z, float r) {
    Matrix *sphereMat = genSphere(x,y,z,r);
    addEdges(sphereMat);
    delete sphereMat;
}

Matrix *EdgeBuffer::genTorus(float x, float y, float z, float rCircle, float rTorus) {
    EdgeBuffer *torusBuff = new EdgeBuffer();

    int theta_count, phi_count;
    for(phi_count = 0; phi_count < PARAMETRIC_ACCURACY; phi_count++) {
        double phi = M_PI*2.0*(double)(phi_count) / PARAMETRIC_ACCURACY;
        // Where our circle is to be drawn in the torus
        double cx = rTorus*cos(phi);
        double cy = rTorus*sin(phi);
        for(theta_count = 0; theta_count < PARAMETRIC_ACCURACY; theta_count++) {
            double theta = M_PI*2.0*(double)(theta_count) / PARAMETRIC_ACCURACY;

            double px = rCircle*cos(theta);
            double py = rCircle*sin(theta)*cos(phi);
            double pz = rCircle*sin(theta)*sin(phi);

            addPointyPoint(x + cx + px,y + cy + py,z + pz);
        }
    }

    // Copy over the Box Edge Buffer's matrix and dispose of the buffer
    Matrix *toCopy = new Matrix(0);
    torusBuff->getPoints()->copyTo(toCopy);
    delete torusBuff;

    return toCopy;
}

void EdgeBuffer::addTorus(float x, float y, float z, float rCircle, float rTorus) {
    Matrix *torusMat = genTorus(x,y,z,rCircle,rTorus);
    addEdges(torusMat);
    delete torusMat;
    
}

void EdgeBuffer::addCircle(float cx, float cy, float cz, float r) {
    int t;
    for(t = 0; t < PARAMETRIC_ACCURACY; t++) {
        double theta = M_PI*2.0*(double)(t) / PARAMETRIC_ACCURACY;
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
    Matrix *transformer = new Matrix(4,4,transform_mat);
    data->multiply(transformer);

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

    delete data;
    delete transformer;
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
    Matrix *transformer = new Matrix(4,4,transform_mat);
    data->multiply(transformer);

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

    delete data;
    delete transformer;
}


//void EdgeBuffer::transform(Matrix *transform) {
//    points->multiply(transform);
//}

