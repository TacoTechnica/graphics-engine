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

/*int main() {
    

    

    Image img(320, 320);
    Renderer renderer(&img);
    EdgeBuffer *buffer = new EdgeBuffer();

    struct Color p = {255, 255, 0};
    renderer.setColor(p);

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
*/
