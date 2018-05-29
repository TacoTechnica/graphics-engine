
#include "animation.h"
#include "image.h"
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

#define ANIMATION_FOLDER "animations"

Animation::Animation() {
    image_count = 0;
    frames = NULL;
}

void Animation::setImageCount(int image_count) {
    this->image_count = image_count;
    if (frames != NULL) {
        free(frames);
    }
    frames = (Image *) malloc(image_count * sizeof(Image));
}

void Animation::addImage(int frame, Image *image) {
    printf("[animation.cpp] pre add image to frame\n");
    frames[frame] = *(new Image(*image));
    printf("[animation.cpp] post add image to frame %d\n", frame);
}

void Animation::saveToGIF(char *name) {
    char *frameName = (char *) malloc( strlen(name) + strlen(ANIMATION_FOLDER) + 4 );
    printf("[animation.cpp] Saving animation frames for %s\n", name);
    
    int frame;
    for(frame = 0; frame < image_count; frame++) {
        sprintf(frameName, "%s/%s%03d", ANIMATION_FOLDER, name, frame);
        Image img = frames[frame];

        printf("[animation.cpp] Saving Frame #%d to %s\n", frame, frameName);

        Image::writeToPPM(&img, frameName);
    }

    char *argName = (char *) malloc( strlen(name) + strlen(ANIMATION_FOLDER) + 2 );
    sprintf(argName, "%s/%s*", ANIMATION_FOLDER, name);
    strncat(name, ".gif", strlen(name) + strlen(".gif"));
    
    int f = fork();
    if (f == 0) {
        int e = execlp("convert", "convert", "-delay", "3", argName, name, NULL);
        printf("e: %d errno: %d: %s\n", e, errno, strerror(errno));
    }

    free(frameName);
    free(argName);

}
