
#include <stdio.h>

#include "vector3f.h"


void Vector3f::normalize() {
    float mag = getMagnitude();
    if (mag != 0)
        multiply( 1.0f / mag );
}

Vector3f *Vector3f::makeCopy() {
    return new Vector3f(x,y,z);
}

float Vector3f::getDotProduct(Vector3f *v1, Vector3f *v2) {
    return v1->x*v2->x + v1->y*v2->y + v1->z*v2->z;
}

// | i  j  k  |
// | x1 y1 z1 |
// | x2 y2 z2 |
Vector3f *Vector3f::getCrossProduct(Vector3f *v1, Vector3f *v2) {
    return new Vector3f(v1->y * v2->z - v1->z * v2->y,
                        v1->z * v2->x - v1->x * v2->z,
                        v1->x * v2->y - v1->y * v2->x
               );
}

Vector3f *Vector3f::getDelta(Vector3f *v1, Vector3f *v2) {
    return new Vector3f(v2->x - v1->x,
                        v2->y - v1->y,
                        v2->z - v1->z
               );
}

void Vector3f::print() {
    printf("V3f: (%.02f, %.02f, %0.2f)\n", x, y, z);
}

 Vector3f Vector3f::operator+( Vector3f &vec) {
    Vector3f *result = this->makeCopy();
    result->x += vec.x;
    result->y += vec.y;
    result->z += vec.z;
    return *result;
}

 Vector3f Vector3f::operator-( Vector3f &vec) {
    Vector3f *result = this->makeCopy();
    result->x -= vec.x;
    result->y -= vec.y;
    result->z -= vec.z;
    return *result;
}

 Vector3f Vector3f::operator*( float val) {
    Vector3f *result = this->makeCopy();
    result->x *= val;
    result->y *= val;
    result->z *= val;
    return *result;
}

 Vector3f Vector3f::operator/( float val) {
    Vector3f *result = this->makeCopy();
    result->x /= val;
    result->y /= val;
    result->z /= val;
    return *result;
}
// Vector3f operator*( float val,  Vector3f &vec) {
//    vec.x *= val;
//    vec.y *= val;
//    return vec;
//}
// Vector3f operator/( float val,  Vector3f &vec) {
//    vec.x /= val;
//    vec.y /= val;
//    return vec;
//}

Vector3f &Vector3f::operator+=( Vector3f &vec) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
    return *this;
}

Vector3f &Vector3f::operator-=( Vector3f &vec) {
    x -= vec.x;
    y -= vec.y;
    z -= vec.z;
    return *this;
}

Vector3f &Vector3f::operator*=( float val) {
    x *= val;
    y *= val;
    z *= val;
    return *this;
}

Vector3f &Vector3f::operator/=( float val) {
    x /= val;
    y /= val;
    z /= val;
    return *this;
}
