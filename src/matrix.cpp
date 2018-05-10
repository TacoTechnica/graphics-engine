
#include "matrix.h"

#include<stdio.h>
#include<stdlib.h>

Matrix::Matrix(int columns, int rows) {
    this->rows = rows;
    this->columns = columns;
    values = (float**) calloc( columns, sizeof(float *));
    int col;
    for(col = 0; col < columns; col++) {
        values[col] = (float*) calloc(rows, sizeof(float));
    }
}

/* Extra constructor that lets us create a matrix from a 1d array
 * for example, passing this array will make a 3x3 identity matrix
 * { 
 *   1, 0, 0
 *   0, 1, 0
 *   0, 0, 1
 * }
 */
Matrix::Matrix(int columns, int rows, float *values1D) : Matrix(columns,rows) {
    int col, row;
    for(col = 0; col < columns; col++)
        for(row = 0; row < rows; row++) {
            values[col][row] = values1D[col + row*columns];
        }
}

Matrix::~Matrix() {
    int col;
    for(col = 0; col < columns; col++) {
        free(values[col]);
    }
    free(values);
}

/*
Matrix *Matrix::createIdentityMatrix(int size) {
    Matrix *mat = new Matrix(size, size);
    int i;
    for(i = 0; i < size; i++) {
        *mat->get(i,i) = 1;
    }
    return mat;
}
*/

void Matrix::fillWithIdentity() {
    int col, row;
    for(col = 0; col < columns; col++)
        for(row = 0; row < rows; row++) {
            if (col == row)
                values[col][row] = 1;
            else
                values[col][row] = 0;
        } 
}

void Matrix::multiply(Matrix *mat) {
    int col, row, m;

    Matrix *dup = new Matrix(columns, rows);
    copyTo(dup); // Need to copy since we're resetting the matrix as we go along
    for(col = 0; col < columns; col++) {
        for(row = 0; row < rows; row++) {
            *get(col,row) = 0;
            for(m = 0; m < rows; m++) {

                float val1 = *dup->get(col, m);
                float val2 = (*(mat->get(m, row)));
                *get(col,row) += val1 * val2;
            }
        }
    }

    delete dup;
}

void Matrix::growColumns(int newColumns) {
    values = (float**) realloc(values, newColumns * sizeof(float *));
    int col;
    for(col = columns; col < newColumns; col++) {
        values[col] = (float*) calloc(rows, sizeof(float));
    }
    columns = newColumns;
}

void Matrix::copyTo(Matrix *mat) {
    // Resize if we're not there
    mat->growColumns(columns);
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

float *Matrix::get(int col, int row) {
    return &values[col][row];
}

// Gets a column, in vector form
Vector3f *Matrix::getColumnVector(int col) {
    return new Vector3f( *get(col, 0), *get(col, 1), *get(col, 2) );
}

void Matrix::clear() {
    printf("don't use me (Matrix::clear())\n");
    //int col;
    //for(col = 0; col < columns; col++) {
    //    free(values[col]);// = (float*) realloc(values[col], sizeof(float));
    //}
    //columns = 0;
    //values = (float**) realloc(values, sizeof(float*));
}
