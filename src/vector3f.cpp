
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
const Vector3f Vector3f::operator+(const Vector3f &vec) {
    Vector3f *result = this->makeCopy();
    result->x += vec.x;
    result->y += vec.y;
    return *result;
}

const Vector3f Vector3f::operator-(const Vector3f &vec) {
    Vector3f *result = this->makeCopy();
    result->x -= vec.x;
    result->y -= vec.y;
    return *result;
}
const Vector3f Vector3f::operator*(const float val) {
    Vector3f *result = this->makeCopy();
    result->x *= val;
    result->y *= val;
    return *result;
}

const Vector3f Vector3f::operator/(const float val) {
    Vector3f *result = this->makeCopy();
    result->x /= val;
    result->y /= val;
    return *result;
}

Vector3f &Vector3f::operator+=(const Vector3f &vec) {
    x += vec.x;
    y += vec.y;
    return *this;
}

Vector3f &Vector3f::operator-=(const Vector3f &vec) {
    x -= vec.x;
    y -= vec.y;
    return *this;
}
