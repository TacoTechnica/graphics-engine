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

#define ROOM_WIDTH 640
#define ROOM_HEIGHT 480

#define K_SPRING 0.03
#define SPHERE_RADIUS 100

void Scene::init() {

    //period = 60;
    //radius = 100;

    //angle = 0;
    Matrix *m = new Matrix(0);
    buffer = new TriangleBuffer(m);

    shape = buffer->genSphere(0, 0, 0, SPHERE_RADIUS);
    prop = buffer->genTorus(0, 0, 0, 30, SPHERE_RADIUS + 40);

    pos = new Vector3f(50,100,100);
    vel = new Vector3f(3,3,0);
    rotation = new Vector3f(0,0,0);
    scale = new Vector3f(1, 1, 1);
    
    gravity = new Vector3f(0, 0, 0);

    xPaused = false;
    yPaused = false;
}

void Scene::tick() {
    // Physics
    *vel += *gravity;

    // If x or y are paused, dont' move in that axis
    if (xPaused) {
        pos->setY( pos->getY() + vel->getY() );
    } else if (yPaused) {
        pos->setX( pos->getX() + vel->getX() );
    } else {
        *pos += *vel;
    }

    //Vector3f angularvel(0.6f, 0.7f, -0.2f);
    //*rotation += angularvel;

    // Point Buffer
    buffer->clearPoints();

    buffer->transformPush(); // Assume we're on the bottom of the stack
    buffer->translate(pos->getX(), pos->getY(), pos->getZ());
    buffer->rotate_x(rotation->getX());
    buffer->rotate_y(rotation->getY());
    buffer->rotate_z(rotation->getZ());
    buffer->scale(scale->getX(), scale->getY(), scale->getZ());
    buffer->addPoints(shape);
    //buffer->addPoints(prop);
    buffer->transformPop();

    //buffer->transformPush();
    //buffer->translate(100, 100, 0);
    //buffer->rotate_x(45);
    //buffer->rotate_z(45);
    //buffer->addPoints(prop);
    //buffer->transformPop();

    float scaleX = 1, scaleY = 1;

    yPaused = false;

    if (pos->getX() < SPHERE_RADIUS) {
        yPaused = true;
        float delSpring = (SPHERE_RADIUS - pos->getX());
        Vector3f force( delSpring * K_SPRING, 0, 0);
        *vel += force;

        scaleX = 1.0f - delSpring / SPHERE_RADIUS;
    } else if (pos->getX() > ROOM_WIDTH - SPHERE_RADIUS) {
        yPaused = true;
        float delSpring = SPHERE_RADIUS - (ROOM_WIDTH - pos->getX());
        Vector3f force( delSpring * K_SPRING, 0, 0);
        *vel -= force;

        scaleX = 1.0f - delSpring / SPHERE_RADIUS;
    }

    xPaused = false;
    if (pos->getY() < SPHERE_RADIUS) {
        xPaused = true;
        float delSpring = (SPHERE_RADIUS - pos->getY());
        Vector3f force( 0, delSpring * K_SPRING, 0);
        *vel += force;

        scaleY = 1.0f - delSpring / SPHERE_RADIUS;
    } else if (pos->getY() > ROOM_HEIGHT - SPHERE_RADIUS) {
        xPaused = true;
        float delSpring = SPHERE_RADIUS - (ROOM_HEIGHT - pos->getY());
        Vector3f force( 0, delSpring * K_SPRING, 0);
        *vel -= force;

        scaleY = 1.0f - delSpring / SPHERE_RADIUS;
    }
    
    if (scaleX != 0 && scaleX < 1) {
        scaleY = 1.0f / pow(scaleX, .3);
    } else if (scaleY != 0 && scaleY < 1) {
        scaleX = 1.0f / pow(scaleY, .3);
    }

    scale->setX(scaleX);
    scale->setY(scaleY);

    //if (pos->getX() < 100) {
    //    pos->setX(100);
    //    vel->setX( vel->getX() * -1 );
    //} else if (pos->getX() > ROOM_WIDTH - 100) {
    //    pos->setX(ROOM_WIDTH - 100);
    //    vel->setX( vel->getX() * -1 );
    //}

    //if (pos->getY() < 100) {
    //    pos->setY(100);
    //    vel->setY( vel->getY() * -1 );
    //} else if (pos->getY() > ROOM_HEIGHT - 100) {
    //    pos->setY(ROOM_HEIGHT - 100);
    //    vel->setY( vel->getY() * -1 );
    //}
}

void Scene::render(Renderer *g) {

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
