
#include <stdio.h>
#include "lighting.h"


Light::Light(Vector3f *direction, struct Color ambient, struct Color color, Vector3f *areflect, Vector3f *dreflect, Vector3f *sreflect) {
    this->direction = direction;
    this->ambient = ambient;
    this->color = color;
    this->areflect = areflect;
    this->dreflect = dreflect;
    this->sreflect = sreflect;

    direction->normalize();
    lightColor = new Vector3f(color.r, color.g, color.b);
}

Vector3f *Light::calculate_ambient() {
    Vector3f *result = new Vector3f(ambient.r, ambient.g, ambient.b);
    result->multiplyComponents(areflect);
    struct Color col = result->toColor();

    return result;
}

Vector3f *Light::calculate_diffuse(Vector3f *normal) {
    Vector3f *result = dreflect->makeCopy();
    result->multiplyComponents(lightColor);

    float brightness = Vector3f::getDotProduct(normal, direction);

    if (brightness < 0) brightness = 0;

    *result *= brightness;

    return result;
}    

Vector3f *Light::calculate_specular(Vector3f *normal, Vector3f *view) {
    Vector3f *result = sreflect->makeCopy();
    result->multiplyComponents(lightColor);

    float normDirDot = Vector3f::getDotProduct(normal, direction);
    if (normDirDot < 0) normDirDot = 0;

    Vector3f innerTerm = *normal * 2.0f * normDirDot - *direction;

    float secondBeforeExponentDot = Vector3f::getDotProduct( &innerTerm, view);
    if (secondBeforeExponentDot < 0) secondBeforeExponentDot = 0;

    *result *= (float)pow(secondBeforeExponentDot, 12.0);

    return result;
}

struct Color Light::getSurfaceLighting(Vector3f *normal, Vector3f *view) {
    Vector3f *ambient  = calculate_ambient();
    Vector3f *diffuse  = calculate_diffuse(normal);
    Vector3f *specular = calculate_specular(normal, view);
    Vector3f result = *ambient + *diffuse + *specular;
    

    struct Color col = result.toColor();

    delete ambient;
    delete diffuse;
    delete specular;
    //delete &result;
    return col;
}
