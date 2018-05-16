#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "image.h"
#include "renderer.h"
#include "edgebuffer.h"
#include "matrix.h"
#include "parser.h"

#include "stack.h"

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

int mainTEST() {
    Image img(500, 500);
    Renderer renderer(&img);
    TriangleBuffer *buffer = new TriangleBuffer();

    struct Color p = {255, 255, 255};
    renderer.setColor(p);
    renderer.refill();

    p = {0, 0, 0};
    renderer.setColor(p);


    //buffer->addTorus(200, 200, 0, 40, 200 - 20);
    //buffer->addBox(-100, -100, -100, 200, 200, 200);
    buffer->rotate_z(45);
    buffer->addSphere(100,100,100,200);
    //buffer->addSphere(0, 0, 0, 100);


    //buffer->scale(1,1,2);
    //buffer->apply();
    //buffer->transformSetIdentity();

    // box works
    renderer.drawTriangleBufferMesh(buffer);

    Image::writeToPPM(&img, "image.ppm");

    exit(0);
    return 0;
}

int mainOldParser() {
    Image img(500, 500);
    Renderer renderer(&img);
    Matrix matrix(4);
    //TriangleBuffer *buffer = new TriangleBuffer();

    struct Color p = {255, 255, 255};
    renderer.setColor(p);
    renderer.refill();

    p = {0, 0, 0};
    renderer.setColor(p);

	// Deleted: Parser::parseFile("src/script", &matrix , &renderer);

    //////////////////////////////////////////////////
    // End
	exit(0);

    /*


    buffer->addTorus(0, 0, 0, 40, 200 - 20);
    //buffer->addBox(-100, -100, -100, 200, 200, 200);
    //buffer->addSphere(100,100,100,10);
    //buffer->addSphere(0, 0, 0, 100);

    //buffer->scale(1,1,2);
    //buffer->apply();
    //buffer->transformSetIdentity();

    // box works
    buffer->rotate_y(40);
    buffer->apply();
    buffer->transformSetIdentity();

    buffer->rotate_x(-45);
    buffer->apply();
    buffer->transformSetIdentity();

    buffer->translate(200, 200, 0);
    buffer->apply();

    renderer.drawTriangleBufferMesh(buffer);

    Image::writeToPPM(&img, "image.ppm");

    exit(0);
    //////////////////////////////////////////////////
    / *

    readObjFile("res/Banana.objfile", buffer);

    buffer->translate(-120,170,0);
    buffer->rotate_z(180);

    buffer->apply();


    //struct Color p = {255, 255, 0};
    renderer.setColor(p);
    renderer.drawEdgeBufferLines(buffer);


    Image::writeToPPM(&img, "image.ppm");

    delete buffer;    
    */
    return 0;
}

