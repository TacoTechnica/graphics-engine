#include <SDL2/SDL.h>
//#include <SDL2/SDL_render.h>

#include "image.h"
#include "window.h"
#include "renderer.h"


Window::Window(int width, int height, const char *title) {
    this->title = title;
    window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_OPENGL
    );
    renderer = SDL_CreateRenderer(
            window, 
            -1, 
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    frame = new Image(width, height);
    graphics = new Renderer(frame);
}

Window::~Window() {
    delete frame;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int Window::getWidth() {
    return frame->getWidth();
}

int Window::getHeight() {
    return frame->getHeight();
}

void Window::showFrame() {
    int xx, yy;
    for(yy = 0; yy < getHeight(); yy++) {
        for(xx = 0; xx < getWidth(); xx++) {
            struct Color *pix = frame->getPixel(xx,yy);
            SDL_SetRenderDrawColor(renderer, pix->r, pix->g, pix->b, 255);
            SDL_RenderDrawPoint(renderer, xx, yy);
        }
    }
    SDL_RenderPresent(renderer);
}
