
#ifndef H_LIGHTING
#define H_LIGHTING

class Lighting {
    private:
        Light light;
};

class Light {
    private:
        Vector3f direction;
        Vector3f areflect;
        Vector3f dreflect;
        Vector3f sreflect;
        struct Color ambient;
        struct Color calculate_ambient();
        struct Color calculate_diffuse(Vector3f *normal);
        struct Color calculate_specular(Vector3f *normal, Vector3f *view);
    public:
        Light(Vector3f direction, Vector3f areflect, Vector3f dreflect, Vector3f sreflect);
        struct Color getSurfaceLighting(Vector3f *normal, Vector3f *view);

}

#endif
