#ifndef PA1_MATRIX_H
#define PA1_MATRIX_H

#include <cstdlib>
#include <vector>
#include <iostream>
#include <random>

class Matrix{

    std::vector<std::vector<int>> matrix;
    int width;
    int height;
    bool genenrate;

public:
    Matrix(int width, int height, bool generate);
    ~Matrix();

    int get_width();
    int get_height();
    std::vector<std::vector<int>> get_matrix();
    void set_matrix(std::vector<std::vector<int>> matrix);

    void printMatrix();
    std::vector<std::vector<int>> generateMatrix(int, int);

};

#endif //PA1_MATRIX_H
