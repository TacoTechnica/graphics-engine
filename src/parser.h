#ifndef H_PARSER
#define H_PARSER

#include "edgebuffer.h"
#include "renderer.h"

class Parser {
    public:
        static void parseFile(char *dir, Matrix *m, Renderer *renderer);
};

#endif
