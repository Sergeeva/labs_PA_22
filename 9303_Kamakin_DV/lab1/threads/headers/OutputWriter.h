#ifndef UNTITLED_OUTPUTWRITER_H
#define UNTITLED_OUTPUTWRITER_H


#include <future>
#include "Runnable.h"
#include "Worker.h"
#include "Matrix.h"

class OutputWriter : public Worker {

private:
    Matrix &target_matrix;

    void print_result();

public:
    explicit OutputWriter(Matrix &target_matrix) : target_matrix(target_matrix) {};

    void run() final;

};


#endif //UNTITLED_OUTPUTWRITER_H
