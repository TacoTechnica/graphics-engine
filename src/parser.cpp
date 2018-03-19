#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<unistd.h>

#include "parser.h"
#include "edgebuffer.h"
#include "renderer.h"
#include "image.h"

// private: Whether two commands are equal
bool eq(const char *s1, const char *s2) {
    return strcmp(s1, s2) == 0;
}

// private: Splits the arguments of a line 
void split_args(char *line, char **args) {
    int i = 0;
    char *arg;
    arg = strtok(line, " ");
    args[i++] = arg;
    while( (arg = strtok(NULL, " ")) != NULL ) {
        args[i++] = arg;
    }
}

void Parser::parseFile(char *filename, EdgeBuffer *buffer, Renderer *renderer) {
    FILE *f;
    char line[256];
    int line_num = 0;
    char *args[16]; // 16 max args

    if ( strcmp(filename, "stdin") == 0 ) 
        f = stdin;
    else
        f = fopen(filename, "r");

    while ( fgets(line, 255, f) != NULL ) {

        line_num++;
    
        line[strlen(line)-1]='\0';

        printf("line: %s\n", line);

		if (line[0] == '#' || line[0] == '\n') continue;

        if (eq(line, "line")) {        
            fgets(line, 255, f);
            line_num++;

            split_args(line, args);
            buffer->addEdge(
                atof(args[0]),
                atof(args[1]),
                atof(args[2]),
                atof(args[3]),
                atof(args[4]),
                atof(args[5])
            );
            continue;
        }
        if (eq(line, "ident")) {
            buffer->transformSetIdentity();
            continue;
        }
        if (eq(line, "scale")) {
            fgets(line, 255, f);
            line_num++;

            split_args(line, args);
            buffer->scale(
                atof(args[0]),
                atof(args[1]),
                atof(args[2])
            );
            continue;
        }
        if (eq(line, "move")) {
            fgets(line, 255, f);
            line_num++;

            split_args(line, args);
            buffer->translate(
                atof(args[0]),
                atof(args[1]),
                atof(args[2])
            );
            continue;
        }
        if (eq(line, "rotate")) {
            fgets(line, 255, f);
            line_num++;

            split_args(line, args);
            if (eq(args[0], "x")) {
                buffer->rotate_x( atof(args[1]) );
                continue;
            }
            if (eq(args[0], "y")) {
                buffer->rotate_y( atof(args[1]) );
                continue;
            }
            if (eq(args[0], "z")) {
                buffer->rotate_z( atof(args[1]) );
                continue;
            }
            printf("[parser.cpp rotate function on line %d] ERROR: Invalid rotation axis: %s\n", line_num, args[0]);
            exit(1);
            continue;
        }
        if (eq(line, "apply")) {
            buffer->apply();
            continue;
        }
        if (eq(line, "display")) {
            renderer->drawEdgeBufferLines(buffer);
            Image::writeToPPM( renderer->getImage(), "temp_image.ppm");
            char *args1[] = {"display", "temp_image.ppm", NULL};
            execvp("display", args1);
            remove("temp_image.ppm");
            //char *args2[] = {"read", "-p", "Press enter to continue", NULL};
            //execvp("read", args2);
            continue;
        }
        if (eq(line, "save")) {
            fgets(line, 255, f);
            line_num++;

            split_args(line, args);
            Image::writeToPPM( renderer->getImage(), "temp_image.ppm");
            char *args1[] = {"convert", "temp_image.ppm", args[0], NULL};
            execvp("convert", args1);
            remove("temp_image.ppm");
            continue;
        }
        if (eq(line, "box")) {
            fgets(line, 255, f);
            line_num++;

            split_args(line, args);
            buffer->addBox(
                atof(args[0]),
                atof(args[1]),
                atof(args[2]),
                atof(args[3]),
                atof(args[4]),
                atof(args[5])
            );
            continue;
        }

    }
}


