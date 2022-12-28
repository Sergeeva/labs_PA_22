#pragma once

#include <vector>



namespace MatrixSpace
{

class Matrix {
public:
    explicit Matrix(const unsigned size);
    const unsigned _size; 
    std::vector<int> _data;
};

void generateMatrix(Matrix&);
void readMatrix(const Matrix&);

}