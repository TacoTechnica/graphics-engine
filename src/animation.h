#ifndef H_ANIMATION
#define H_ANIMATION

#include "image.h"

#define ANIMATION_FOLDER "animations"

class Animation {
    private:
        Image *frames;
        int image_count;
    public:
        Animation();
        void addImage(int frame, Image *image);
        void saveToGIF(char *name);

        void setImageCount(int image_count);
};

#endif
