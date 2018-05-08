#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "scene.h"
#include "renderer.h"
#include "trianglebuffer.h"
#include "matrix.h"


/*void readObjFile(char *dir, EdgeBuffer *buffer) {
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
}*/

void Scene::init() {
    angle = 0;
    Matrix *m = new Matrix(0);
    buffer = new TriangleBuffer(m);

    shape = buffer->genSphere(0, 0, 0, 100);

    pos = new Vector3f(100,100,100);
    vel = new Vector3f(0,0,0);
    rotation = new Vector3f(0,0,0);
}

void Scene::tick() {
    // Physics
    *pos += *vel;

    // Point Buffer
    buffer->clearPoints();
    buffer->transformPush(); // Assume we're on the bottom of the stack
    buffer->translate(pos->getX(), pos->getY(), pos->getZ());
    buffer->rotate_x(rotation->getX());
    buffer->rotate_y(rotation->getY());
    buffer->rotate_z(rotation->getZ());
    buffer->addPoints(shape);
    buffer->transformPop();
}

void Scene::render(Renderer *g) {
    *rotation += Vector3f(0.1, 0, 0);
    //angle += 0.1;

    struct Color p = {255, 255, 0};
    g->setColor(p);

    g->drawTriangleBufferMesh(buffer);

}

Scene::~Scene() {
    delete shape;
    delete pos;
    delete vel;
    delete rotation;
}
