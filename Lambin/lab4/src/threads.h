#ifndef THREADS_H
#define THREADS_H

#include "matrix.h"

class Threads {
public:
    Threads(int, char *, char *);
    ~Threads() = default;
    void run();

private:
    Matrix<int> strassenAlgorithm();
    Matrix<int> recursionStrassen(Matrix<int>, Matrix<int>, int);
    void write(char *, Matrix<int> &);

    int matrixSize;
    char *multiplicationResultFileName;
    char *strassenResultFileName;
    Matrix<int> matrix1, matrix2;

};

#endif //THREADS_H
