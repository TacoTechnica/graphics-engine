
// Note: For the purpose of graphics, the renderer is private.
// All rendering must be done on the image itself!

#ifndef H_WINDOW
#define H_WINDOW

#include <SDL2/SDL.h>
#include "image.h"
#include "renderer.h"

class Window {
    private:
        SDL_Window *window;
        SDL_Renderer *renderer;
        Image *frame;
        Renderer *graphics;
        const char *title;
    public:
        Window(int width, int height, const char *title);
        ~Window();
        Image *getFrame() {return frame;}
        Renderer *getGraphics() {return graphics;}
        //SDL_Renderer *getRenderer() {return renderer;}
        int getWidth();
        int getHeight();

        void showFrame();
};

#endif
