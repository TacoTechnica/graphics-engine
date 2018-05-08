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


/*int main() {
    Image img(500, 500);
    Renderer renderer(&img);
    Matrix matrix(4);
    //TriangleBuffer *buffer = new TriangleBuffer();

    struct Color p = {255, 255, 255};
    renderer.setColor(p);
    renderer.refill();

    p = {0, 0, 0};
    renderer.setColor(p);

	Parser::parseFile("src/script", &matrix , &renderer);

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
    * /
    return 0;
}
*/
