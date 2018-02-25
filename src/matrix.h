
#ifndef H_MATRIX
#define H_MATRIX

class Matrix {
    private:
        double **values;
        int rows, columns;
        int lastcol;

    public:
        Matrix(int columns, int rows = 4);
        ~Matrix();
        void identity();
        void multiply(Matrix *mat);
        void growColumns(int newColumns);
        void copyTo(Matrix *mat);
        void print();
        double *get(int col, int row);
        double getColumns() { return columns; }
        double getRows() { return rows; }
};

#endif
