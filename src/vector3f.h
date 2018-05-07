
#ifndef VECTOR3F
#define VECTOR3F

#include<math.h>
#include "image.h"

class Vector3f {
    private:
        float x,y,z;
    public:
        Vector3f(float x, float y, float z) { this->x = x; this->y = y; this->z = z;}
        float getMagnitudeSquared() { return x*x + y*y + z*z; }
        float getMagnitude() { return sqrt( getMagnitudeSquared() ); }
        void normalize();
        float getX() { return x; }
        float getY() { return y; }
        float getZ() { return z; }
        float setX(float x) { this->x = x; }
        float setY(float y) { this->y = y; }
        float setZ(float z) { this->z = z; }

        void add(Vector3f *v)  { this->x += v->x; this->y += v->y; this->z += v->z; }
        void multiply(float m) { this->x *= m; this->y *= m; this->z *= m; }

        void multiplyComponents(Vector3f *v) { this->x *= v->x; this->y *= v->y; this->z *= v->z; }

        Vector3f *makeCopy();

        void print();

        struct Color toColor() {
            struct Color col = { (unsigned char)fmin(x,255) , (unsigned char)fmin(y,255), (unsigned char)fmin(z,255) };
            return col;
        }

        // Operators
        //Vector3f *operator+( Vector3f &vec);
        //Vector3f *operator-( Vector3f &vec);
        //Vector3f *operator*( float val);
        //Vector3f *operator/( float val);
         Vector3f operator+( Vector3f &vec);
         Vector3f operator-( Vector3f &vec);
         Vector3f operator*( float val);
         Vector3f operator/( float val);
        // Vector3f operator*( float val,  Vector3f &vec);
        // Vector3f operator/( float val,  Vector3f &vec);

        Vector3f &operator+=( Vector3f &vec);
        Vector3f &operator-=( Vector3f &vec);
        Vector3f &operator*=( float val);
        Vector3f &operator/=( float val);

        static float getDotProduct(Vector3f *v1, Vector3f *v2);
        static Vector3f *getCrossProduct(Vector3f *v1, Vector3f *v2);
        static Vector3f *getDelta(Vector3f *v1, Vector3f *v2);
};

#endif
