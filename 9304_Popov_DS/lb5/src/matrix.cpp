#include <random>
#include <iostream>

#include "matrix.hpp"



namespace MatrixSpace
{

Matrix::Matrix(const unsigned size)
    : _size(size)
{
    _data.resize(_size * _size);
}

void generateMatrix(Matrix& matrix)
{
    for(auto& e : matrix._data)
        e = rand() % 50 + 1;
}

void readMatrix(const Matrix& matrix)
{
    const unsigned size = matrix._size * matrix._size;

    for (unsigned j = 0; j < matrix._size; ++j)
    {
        for (unsigned i = j; i < size; i += matrix._size)
            std::cout << matrix._data[i] << ' ';

        std::cout << '\n';
    }
}

}