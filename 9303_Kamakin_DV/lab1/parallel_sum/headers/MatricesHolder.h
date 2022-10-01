#ifndef UNTITLED_MATRICESHOLDER_H
#define UNTITLED_MATRICESHOLDER_H


#include "Matrix.h"

class MatricesHolder {

private:
    Matrix &first;
    Matrix &second;

public:
    MatricesHolder(Matrix &first, Matrix &second) : first(first), second(second) {};

    Matrix &get_first();

    Matrix &get_second();

};


#endif //UNTITLED_MATRICESHOLDER_H
