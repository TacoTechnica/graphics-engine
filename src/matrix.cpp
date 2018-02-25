
#include "matrix.h"

#include<stdio.h>
#include<stdlib.h>

Matrix::Matrix(int columns, int rows) {
    this->rows = rows;
    this->columns = columns;
    values = (double**) calloc( columns, sizeof(double *));
    int col;
    for(col = 0; col < columns; col++) {
        values[col] = (double*) calloc(rows, sizeof(double));
    }
}

Matrix::~Matrix() {
    int col;
    for(col = 0; col < columns; col++) {
        free(values[col]);
    }
    free(values);
}

void Matrix::identity() {

}


void Matrix::multiply(Matrix *mat) {
    int col, row, m;
    for(col = 0; col < columns; col++) {
        for(row = 0; row < rows; row++) {
            printf("%d, %d\n", col, row);
            *get(col,row) = 0;
            for(m = 0; m < rows; m++) {
                *get(col,row) += *get(col, m) * (*(mat->get(m, row)));
                    //+ *get(col, row+1) * *mat->get(1, row);
                    //+ *get(col, row+2) * *mat->get(2, row);
            }
        }
    }
}

void Matrix::growColumns(int newColumns) {
    values = (double**) realloc(values, newColumns * sizeof(double *));
    int col;
    for(col = columns; col < newColumns; col++) {
        values[col] = (double*) calloc(rows, sizeof(double));
    }
    columns = newColumns;
}

void Matrix::copyTo(Matrix *mat) {
    int col, row;
    for(col = 0; col < columns; col++) {
        for(row = 0; row < rows; row++) {
            *mat->get(col, row) = *get(col,row);
        }
    }
}

void Matrix::print() {
    int col, row;
    for(row = 0; row < rows; row++) {
        printf("|");
        for(col = 0; col < columns; col++) {
            printf("%.2f  ", *get(col, row));
        }
        printf("|\n");
    }
}

double *Matrix::get(int col, int row) {
    return &values[col][row];
}

