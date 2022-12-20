#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <iostream>


class Matrix
{
public:
    std::vector<std::vector<int>> matrix;
    Matrix(int N, int M);
    Matrix();
    void add(Matrix& term);
    int*  serialization();
    void deserialization(int* buf,int  N,int M);
    void fileOutput(std::string& fileName);
    Matrix operator=(Matrix copy);
};

#endif // MATRIX_H