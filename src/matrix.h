
#ifndef H_MATRIX
#define H_MATRIX

class Matrix {
    private:
        float **values;
        int rows, columns;
        int lastcol;
    public:
        Matrix(int columns, int rows = 4);
        Matrix(int columns, int rows, float *values1D);
        ~Matrix();
        void multiply(Matrix *mat);
        void growColumns(int newColumns);
        void copyTo(Matrix *mat);
        void print();
        float *get(int col, int row);
        int getNumColumns() { return columns; }
        int getNumRows() { return rows; }
        void fillWithIdentity();
//        static Matrix *createIdentityMatrix(int size);
};

#endif
