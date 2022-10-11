#ifndef PA1_MATRIX_H
#define PA1_MATRIX_H

#include <cstdlib>
#include <vector>
#include <iostream>
#include <random>

//class Matrix{

typedef std::vector<std::vector<int>> Matrix;

int get_width();
int get_height();
void printMatrix(Matrix);
Matrix generateMatrix(int, int);

//};

#endif //PA1_MATRIX_H
