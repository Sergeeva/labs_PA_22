#ifndef UNTITLED_MATRICESHOLDER_H
#define UNTITLED_MATRICESHOLDER_H


#include "Matrix.h"

class MatricesHolder {

private:
    Matrix first;
    Matrix second;

public:
    MatricesHolder() : first(Matrix()), second(Matrix()) {}

    MatricesHolder(Matrix &first, Matrix &second) : first(first), second(second) {}

    MatricesHolder(std::vector<std::vector<int>> *first,
                   std::vector<std::vector<int>> *second,
                   int square_matrix_size)
            : first(Matrix(first, square_matrix_size)), second(Matrix(second, square_matrix_size)) {}

    Matrix &get_first();

    Matrix &get_second();

};


#endif //UNTITLED_MATRICESHOLDER_H
