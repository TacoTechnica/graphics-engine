#ifndef H_ANIMATION
#define H_ANIMATION

#include "image.h"

class Animation {
    private:
        Image *frames;
        int image_count;
    public:
        Animation();
        addImage(int frame, Image image);
        saveToPPM(char *path);

        setImageCount(int image_count);
};

#endif
