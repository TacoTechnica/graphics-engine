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

void torusTorus(EdgeBuffer *buffer, float x, float y, float z, float rBigTorus, float rMiniTorus, float rMiniTorusCircle) {
    EdgeBuffer *torusBuff = new EdgeBuffer();
    Matrix *torusMat = EdgeBuffer::genTorus(0,0,0,rMiniTorusCircle, rMiniTorus);
    torusBuff->addEdges(torusMat);

    torusBuff->rotate_x(90);
    torusBuff->apply();
    torusBuff->transformSetIdentity();
    int theta_count;
    int PARAMETRIC_ACCURACY = 30;
    for(theta_count = 0; theta_count < PARAMETRIC_ACCURACY; theta_count++) {
        double theta = M_PI*2.0*(double)(theta_count) / PARAMETRIC_ACCURACY;

        double px = rBigTorus*cos(theta);
        double py = rBigTorus*sin(theta);

        torusBuff->rotate_z(-360.0 / (double)PARAMETRIC_ACCURACY);
        torusBuff->apply();
        torusBuff->transformSetIdentity();

        torusBuff->translate(x + px, y + py, z);
        torusBuff->apply();
        torusBuff->transformSetIdentity();

        buffer->addEdges(torusBuff->getPoints());

    
        torusBuff->translate(-(x + px), -(y + py), -z);
        torusBuff->apply();
        torusBuff->transformSetIdentity();

        

        //buffer->addSphere(0,0,0, 10);
        //buffer->addTorus(0,0,0, rMiniTorusCircle, rMiniTorus);
        //buffer->rotate_y(90);
        //buffer->apply();

        //buffer->transformSetIdentity();
        //buffer->translate(x + px, y + py, z);
        //buffer->apply();
        //buffer->transformSetIdentity();
    }

    delete torusBuff;
    delete torusMat;
}


int main() {
    

    Image img(400, 400);
    Renderer renderer(&img);
    EdgeBuffer *buffer = new EdgeBuffer();

    struct Color p = {255, 255, 255};
    renderer.setColor(p);
    renderer.refill();

    p = {0, 0, 0};
    renderer.setColor(p);

    //buffer->addTorus(0, 0, 0, 10, 100);
    //buffer->addSphere(0, 0, 0, 100);

    // box works
    //buffer->addBox(10, 10, 10, 100, 200, 100);
    //buffer->transformSetIdentity();
    ////buffer->rotate_x(10);
    ////buffer->apply();
    
    torusTorus(buffer, 0, 0, 0, 150, 50, 5);
    buffer->rotate_y(-45);
    buffer->apply();
    buffer->transformSetIdentity();

    buffer->translate(200, 200, 0);
    buffer->apply();

    renderer.drawEdgeBufferLines(buffer);

    Image::writeToPPM(&img, "image.ppm");

    //////////////////////////////////////////////////
    // End
    exit(0);
    //////////////////////////////////////////////////

    Parser::parseFile("src/script", buffer, &renderer);

    exit(0);

    readObjFile("res/Banana.objfile", buffer);

    buffer->translate(-120,170,0);
    buffer->rotate_z(180);

    buffer->apply();


    //struct Color p = {255, 255, 0};
    renderer.setColor(p);
    renderer.drawEdgeBufferLines(buffer);


    Image::writeToPPM(&img, "image.ppm");

    delete buffer;    

    return 0;
}
