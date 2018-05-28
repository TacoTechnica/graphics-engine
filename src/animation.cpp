
#include "animation.h"
#include "image.h"


Animation::Animation() {
    image_count = 0;
    frames = NULL;
}

Animation::setImageCount(int image_count) {
    this->image_count = image_count;
    if (frames != NULL) {
        free(frames);
    }
    frames = malloc(image_count * sizeof(Image *));
}
