#include<SDL2/SDL.h>

#include "window.h"
#include "image.h"
#include "scene.h"

#include "matrix.h"

#define GAME_FPS 60

Window *window;

bool running;

SDL_Event sdl_event;

void start();
void loop();
void tick();
void render();
void quit();

//int temp = 0;


void start() {
    running = true;
    window = new Window(640, 480, "Test!");


    loop();
}


void loop() {
    unsigned int now_time;
    unsigned int prev_time = SDL_GetTicks();
    unsigned int delta_time;

    bool should_render = 0;

    unsigned int ms_per_frame = 1000 / GAME_FPS;

    int tick_count = 0;

    while(running) {
        should_render = 0;

        now_time = SDL_GetTicks();

        delta_time = now_time - prev_time;
        while(delta_time > ms_per_frame) {
            delta_time -= ms_per_frame;
            should_render = 1;

            tick();
            tick_count++;

            prev_time = now_time;
        }

        if (should_render) {
            render();
        }
    }

}


void tick() {
    while (SDL_PollEvent(&sdl_event)) { 
        switch(sdl_event.type) {
            // Grab window events
            case SDL_WINDOWEVENT:
                switch (sdl_event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE:
                        quit();
                        exit(0);
                    break;
                }
                break;
        }
    }


}
void render() {
    /*Image *frame = window->getFrame();
    int xx, yy;
    for(yy = 0; yy < window->getHeight(); yy++) {
        for(xx = 0; xx < window->getWidth(); xx++) {
            struct Pixel *pix = frame->getPixel(xx,yy);
            pix->r = 0;
            pix->g = 0;
            pix->b = 0;
        }
    }
    */
    Graphics *g = window->getGraphics();

    g->setColor( (struct Pixel){0,0,0} );
    g->fillRect(0,0,window->getWidth(), window->getHeight());


    window->showFrame();
}


void quit() {
    delete window;

    delete tempScene;
}

int main() {

    Window win(640, 480, "Window 1");

    win.getFrame()->getPixel(15,15)->r = 255;
    win.showFrame();
    SDL_Delay(2000);
    return 0;
}
