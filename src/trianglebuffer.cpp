
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
    boxBuff->addTriangle(0,0,0, x+xl,0,0, x+xl,y+yl,0);
    boxBuff->addTriangle(0,0,0, 0,y+yl,0, x+xl,y+yl,0);

    boxBuff->addTriangle(0,0,0, x+xl,0,0, x+xl,0,z+zl);
    boxBuff->addTriangle(0,0,0, 0,0,z+zl, x+xl,0,z+zl);

    boxBuff->addTriangle(0,0,0, 0,0,z+zl, 0,y+yl,z+zl);
    boxBuff->addTriangle(0,0,0, 0,y+yl,0, 0,y+yl,z+zl);

    // 3 other faces from not the origin
    boxBuff->addTriangle(x+xl,0,0, x+xl,y+yl,0, x+xl,y+yl,z+zl);
    boxBuff->addTriangle(x+xl,0,0, x+xl,0,z+zl, x+xl,y+yl,z+zl);

    boxBuff->addTriangle(0,0,z+zl, 0,y+yl,z+zl, x+xl,y+yl,z+zl);
    boxBuff->addTriangle(0,0,z+zl, x+xl,0,z+zl, x+xl,y+yl,z+zl);

    boxBuff->addTriangle(0,y+yl,0, x+xl,y+yl,0, x+xl,y+yl,z+zl);
    boxBuff->addTriangle(0,y+yl,0, 0,y+yl,z+zl, x+xl,y+yl,z+zl);



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
    TriangleBuffer *sphereBuff = new TriangleBuffer();

    int theta_count, phi_count;
    double phi_factor   = M_PI*2.0/(double)PARAMETRIC_ACCURACY;
    double theta_factor = M_PI/(double)PARAMETRIC_ACCURACY;
    for(phi_count = 0; phi_count < PARAMETRIC_ACCURACY; phi_count++) {
        double phi = (double)phi_count*phi_factor;
        for(theta_count = 0; theta_count <= PARAMETRIC_ACCURACY; theta_count++) {
            double theta = (double)theta_count*theta_factor;

            // Make a square, but in polar coordinates
            double p1x = r*cos(theta);
            double p1y = r*sin(theta)*cos(phi);
            double p1z = r*sin(theta)*sin(phi);

            double p2x = r*cos(theta + theta_factor);
            double p2y = r*sin(theta + theta_factor)*cos(phi);
            double p2z = r*sin(theta + theta_factor)*sin(phi);

            double p3x = r*cos(theta + theta_factor);
            double p3y = r*sin(theta + theta_factor)*cos(phi + phi_factor);
            double p3z = r*sin(theta + theta_factor)*sin(phi + phi_factor);

            double p4x = r*cos(theta);
            double p4y = r*sin(theta)*cos(phi + phi_factor);
            double p4z = r*sin(theta)*sin(phi + phi_factor);

            sphereBuff->addTriangle(p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z);
            sphereBuff->addTriangle(p3x, p3y, p3z, p4x, p4y, p4z, p1x, p1y, p1z);

            //sphereBuff->addPointyPoint(x + px,y + py,z + pz);
        }
    }

    Matrix *toCopy = new Matrix(0);
    sphereBuff->getPoints()->copyTo(toCopy);
    delete sphereBuff;
    return toCopy;

}

void TriangleBuffer::addSphere(float x, float y, float z, float r) {
    Matrix *sphereMat = genSphere(x,y,z,r);
    addTriangles(sphereMat);
    delete sphereMat;
}

Matrix *TriangleBuffer::genTorus(float x, float y, float z, float rCircle, float rTorus) {
    TriangleBuffer *torusBuff = new TriangleBuffer();

    int theta_count, phi_count;
    double angle_factor = M_PI*2.0/(double)PARAMETRIC_ACCURACY;
    for(phi_count = 0; phi_count < PARAMETRIC_ACCURACY; phi_count++) {
        double phi = (double)phi_count * angle_factor;
        // Where our circle is to be drawn in the torus
        double c1x = rTorus*cos(phi);
        double c1y = rTorus*sin(phi);
        double c2x = rTorus*cos(phi + angle_factor);
        double c2y = rTorus*sin(phi + angle_factor);
        for(theta_count = 0; theta_count < PARAMETRIC_ACCURACY; theta_count++) {
            double theta = (double)theta_count * angle_factor;

            double p1x = x + c1x + rCircle*sin(theta)*cos(phi);
            double p1y = y + c1y + rCircle*sin(theta)*sin(phi);
            double p1z = z + rCircle*cos(theta);

            double p2x = x + c1x + rCircle*sin(theta + angle_factor)*cos(phi);
            double p2y = y + c1y + rCircle*sin(theta + angle_factor)*sin(phi);
            double p2z = z + rCircle*cos(theta + angle_factor);
 
            double p3x = x + c2x + rCircle*sin(theta + angle_factor)*cos(phi + angle_factor);
            double p3y = y + c2y + rCircle*sin(theta + angle_factor)*sin(phi + angle_factor);
            double p3z = z + rCircle*cos(theta + angle_factor);

            double p4x = x + c1x + rCircle*sin(theta)*cos(phi + angle_factor);
            double p4y = y + c1y + rCircle*sin(theta)*sin(phi + angle_factor);
            double p4z = z + rCircle*cos(theta);

            torusBuff->addTriangle(p1x, p1y, p1z, p2x, p2y, p2z, p3x, p3y, p3z);
            torusBuff->addTriangle(p3x, p3y, p3z, p4x, p4y, p4z, p1x, p1y, p1z);
            //torusBuff->addPointyPoint(x + cx + px,y + cy + py,z + pz);
        }
    }

    Matrix *toCopy = new Matrix(0);
    torusBuff->getPoints()->copyTo(toCopy);
    delete torusBuff;
    return toCopy;
}

void TriangleBuffer::addTorus(float x, float y, float z, float rCircle, float rTorus) {
    Matrix *torusMat = genTorus(x,y,z,rCircle, rTorus);
    addTriangles(torusMat);
    delete torusMat;
}

