#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "scene.h"
#include "renderer.h"
#include "edgebuffer.h"
#include "matrix.h"


void readObjFile(char *dir, EdgeBuffer *buffer) {
    char letters[80720]; // Should use stat to grab file size, but I'm lazy

    printf("reading and opening %s...\n", dir);

    int fd = open(dir, O_RDONLY);

    read(fd, letters, sizeof(letters));

    char *current_string;
    int index = 0;
    float coord[3];

    // Get rid of the "v" at the beginning and start the split chain
    strtok(letters, " ");
    //int counter = 0;
    while( (current_string = strtok(NULL, " ")) != NULL ) {
        //if (counter > 5) break;

        coord[index] = atof(current_string);
        index++;
        if (index == 3) {
            float x = coord[0];
            float y = coord[2];
            float z = coord[1];
            buffer->addPoint(x, y, z);
            index = 0;
        }
    } 
}

void Scene::init() {
    angle = 0;

    buffer = new EdgeBuffer();

    readObjFile("res/Banana.objfile", buffer);
}

void Scene::tick() {

}

void Scene::render(Renderer *g) {

    //angle += 0.1;

    struct Color p = {255, 255, 0};
    g->setColor(p);

    buffer->transformSetIdentity();
    buffer->translate(-120 + (int)angle,-280,0);
    buffer->rotate_z(180);

    buffer->apply();


    g->drawEdgeBufferLines(buffer);

}

Scene::~Scene() {
    delete buffer;
}
