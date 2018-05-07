
#ifndef H_LIGHTING
#define H_LIGHTING

#include "vector3f.h"

class Light {
    private:
        Vector3f *direction;
        Vector3f *areflect;
        Vector3f *dreflect;
        Vector3f *sreflect;
        struct Color ambient;
        struct Color color;
        Vector3f *lightColor;
        Vector3f *calculate_ambient();
        Vector3f *calculate_diffuse(Vector3f *normal);
        Vector3f *calculate_specular(Vector3f *normal, Vector3f *view);
    public:
        Light(Vector3f *direction, struct Color ambient, struct Color color, Vector3f *areflect, Vector3f *dreflect, Vector3f *sreflect);
        struct Color getSurfaceLighting(Vector3f *normal, Vector3f *view);

};

#endif
