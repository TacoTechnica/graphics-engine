
#ifndef H_SCENE
#define H_SCENE

#include "renderer.h"
#include "trianglebuffer.h"

class Scene {
    private:
        TriangleBuffer *buffer;

        Matrix *shape, *prop;

        Vector3f *pos, *vel;
        Vector3f *gravity;
        Vector3f *rotation;

        Vector3f *scale;

        bool yPaused, xPaused;
        //float period;
        //float radius;
    public:
        ~Scene();
        void init();
        void tick();
        void render(Renderer *g);

};
    

#endif
