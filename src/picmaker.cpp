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

int main() {
    
    
    /*float f_mat[] =
        {1, 2, 3,
         4, 5, 6,
         7, 8, 9,
         1, 1, 1};

    float f_transform[] =
        {1, 0, 0, 1,
         0, 1, 0, 0,
         0, 0, 1, 0,
         0, 0, 0, 1};

    Matrix mat1(3, 4, f_mat);
    Matrix transform(4, 4, f_transform);


    printf("TRANSFORM: \n");
    transform.print();
    printf("MAT: \n");
    mat1.print();

    mat1.multiply(&transform);

    printf("MAT AFTER: \n");
    mat1.print();
    */

    Image img(440, 320);
    Renderer renderer(&img);
    EdgeBuffer *buffer = new EdgeBuffer();;

    char letters[80720]; // Should use stat to grab file size, but I'm lazy

    printf("reading and opening file...\n");

    int fd = open("res/Banana.objfile", O_RDONLY);

    read(fd, letters, sizeof(letters));

    printf("Done reading file, now storing data...\n");

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

            //counter++;
        }
    }
    
    //buffer->getPoints()->print();

    // Center the banana
    //buffer->translate(-150,180,0);
    
    // Scale up
    //buffer->scale(2,2,0);

    // Move to proper coordinate
    //buffer->translate(200, 200, 0);

    //buffer->getPoints()->print();

    /* buffer->addEdge(100,100,0,
                   200,200,0);
    */
 

    buffer->translate(200,200,0);
    buffer->rotate_z(-20 * 3.1415 / 100);
    buffer->translate(-150,180,0);
    //buffer->rotate_x(3.1415 / 2);

    // Now scale to our final size (2x)
    //buffer->scale(1.5,1.5,0);
    
    // Move banana to starting position
    //buffer->translate(210, -160, 0);
    // Center the banana to (0,0)

    buffer->apply();

    /*
    int i;
    for(i = 0; i < 20; i++) {

        buffer->scale(1.01f,1.01f,0);
    
        unsigned char col = 255 * ((float)i / 20.0f);
        struct Color p = {col, col, 0};
        renderer.setColor(p);

        renderer.drawEdgeBufferLines(buffer);
    }*/
    struct Color p = {255, 255, 0};
    renderer.setColor(p);
    renderer.drawEdgeBufferLines(buffer);


    Image::writeToPPM(&img, "image.ppm");

    delete buffer;    

    return 0;
}
