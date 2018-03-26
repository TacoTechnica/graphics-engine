
#include<stdio.h>
#include<math.h>

#include "trianglebuffer.h"

void TriangleBuffer::addTriangle(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2) {
    addPoint(x0, y0, z0);
    addPoint(x1, y1, z1);
    addPoint(x2, y2, z2);

}
Matrix *TriangleBuffer::genBox(float x, float y, float z, float xl, float yl, float zl) {
    TriangleBuffer *boxBuff = new TriangleBuffer();

    // 3 faces from origin
    boxBuff.addTriangle(0,0,0, x+xl,0,0, x+xl,y+yl,0);
    boxBuff.addTriangle(0,0,0, 0,y+yl,0, x+xl,y+yl,0);

    boxBuff.addTriangle(0,0,0, x+xl,0,0, x+xl,0,z+zl);
    boxBuff.addTriangle(0,0,0, 0,0,z+zl, x+xl,0,z+zl);

    boxBuff.addTriangle(0,0,0, 0,0,z+zl, 0,y+yl,z+zl);
    boxBuff.addTriangle(0,0,0, 0,y+yl,0, 0,y+yl,z+zl);

    // 3 other faces from not the origin
    boxBuff.addTriangle(x+xl,0,0, x+xl,y+yl,0, x+xl,y+yl,z+zl);
    boxBuff.addTriangle(x+xl,0,0, x+xl,0,z+zl, x+xl,y+yl,z+zl);

    boxBuff.addTriangle(0,0,z+zl, 0,y+yl,z+zl, x+xl,y+yl,z+zl);
    boxBuff.addTriangle(0,0,z+zl, x+xl,0,z+zl, x+xl,y+yl,z+zl);

    boxBuff.addTriangle(0,y+yl,0, x+xl,y+yl,0, x+xl,y+yl,z+zl);
    boxBuff.addTriangle(0,y+yl,0, 0,y+yl,z+zl, x+xl,y+yl,z+zl);



    Matrix *toCopy = new Matrix(0);
    boxBuff->getPoints()->copyTo(toCopy);
    delete boxBuff;

    return toCopy;
}

void TriangleBuffer::addBox(float x, float y, float z, float xlength, float ylength, float zlength) {
    Matrix *boxMat = genBox(x,y,z,xlength,ylength,zlength);
    addTriangles(boxMat);
    delete boxMat;
}

Matrix *TriangleBuffer::genSphere(float x, float y, float z, float r) {
    return NULL;

}

void TriangleBuffer::addSphere(float x, float y, float z, float r) {

}

Matrix *TriangleBuffer::genTorus(float x, float y, float z, float rCircle, float rTorus) {
    return NULL;
}

void TriangleBuffer::addTorus(float x, float y, float z, float rCircle, float rTorus) {

}

void TriangleBuffer::addCircle(float cx, float cy, float cz, float r) {

}

void TriangleBuffer::addHermite(float x0, float y0, float x1, float y1, float dx0, float dy0, float dx1, float dy1) {

}

void TriangleBuffer::addBezier(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3) {

}
