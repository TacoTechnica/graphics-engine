
#include "lighting.h"


Light::Light(Vector3f direction, struct Color ambient, Vector3f areflect, Vector3f dreflect, Vector3f sreflect) {
    this->direction = direction;
    this->ambient = ambient;
    this->areflect = areflect;
    this->dreflect = dreflect;
    this->sreflect = sreflect;
}

struct Color Light::calculate_ambient() {
    Vector3f *result = new Vector3f(ambient.r, ambient.g, ambient.g);
    result.multiplyComponents(&areflect);
    struct Color col = result->toColor();

    delete result;
    return col;
}

struct Color Light::calculate_diffuse(Vector3f *normal) {
    Vector3f *result = dreflect.makeCopy();
    float brightness = Vector3f::getDotProduct(normal, &direction);
    
    //result->multiplyComponents(&dreflect);
    result->multiply(brightness);

    struct Color col = result->toColor();

    delete result;
    return col;
}

struct Color Light::calculate_specular(Vector3f *normal, Vector3f *view) {
    Vector3f *result = sreflect.makeCopy();
    sreflect
}

struct Color Light::getSurfaceLighting(Vector3f *normal, Vector3f *view);
