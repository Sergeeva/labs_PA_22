#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <iostream>
#include <cassert>

class Matrix
{
public:
    std::vector<std::vector<int>> matrix;
    Matrix(int N, int M);
    Matrix();
    void add(Matrix& term);
    int*  serialization();
    void deserialization(int* buf,int  N,int M);
    void fileOutput(char* fileName);
    void set_zero();
    Matrix operator=(Matrix copy);
    void shape(int new_size);
};
Matrix operator+(Matrix first,Matrix second);
Matrix operator*(Matrix first,Matrix second);
Matrix operator-(Matrix first,Matrix second);


#endif // MATRIX_H
