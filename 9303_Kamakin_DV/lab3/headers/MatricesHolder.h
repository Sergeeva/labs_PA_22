#ifndef UNTITLED_MATRICESHOLDER_H
#define UNTITLED_MATRICESHOLDER_H


#include "Matrix.h"

class MatricesHolder {

private:
    Matrix first;
    Matrix second;

public:
    MatricesHolder() : first(Matrix()), second(Matrix()) {}

    Matrix &get_first();

    Matrix &get_second();

};


#endif //UNTITLED_MATRICESHOLDER_H
