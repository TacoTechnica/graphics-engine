
#ifndef H_SCENE
#define H_SCENE

#include "renderer.h"
#include "trianglebuffer.h"

class Scene {
    private:
        TriangleBuffer *buffer;
        double angle;

        Matrix *shape;

        Vector3f *pos, *vel;
        Vector3f *rotation;
    public:
        ~Scene();
        void init();
        void tick();
        void render(Renderer *g);

};
    

#endif
