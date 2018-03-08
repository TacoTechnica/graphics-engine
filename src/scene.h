
#ifndef H_SCENE
#define H_SCENE

#include "renderer.h"
#include "edgebuffer.h"

class Scene {
    private:
        EdgeBuffer *buffer;
        double angle;
    public:
        ~Scene();
        void init();
        void tick();
        void render(Renderer *g);

};
    

#endif
