#ifndef H_PARSER_OLD
#define H_PARSER_OLD

#include "edgebuffer.h"
#include "renderer.h"

class ParserOld {
    public:
        static void parseFile(char *dir, Matrix *m, Renderer *renderer);
};

#endif
