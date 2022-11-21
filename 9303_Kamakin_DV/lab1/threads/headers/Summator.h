#ifndef UNTITLED_SUMMATOR_H
#define UNTITLED_SUMMATOR_H


#include <future>
#include "Worker.h"
#include "Matrix.h"
#include "MatricesHolder.h"

class Summator : public Worker {

private:
    MatricesHolder &holder;

    std::promise<Matrix> result;

public:

    Summator(MatricesHolder &holder, std::promise<Matrix> result_future) : holder(holder),
                                                                           result(std::move(result_future)) {};

    void run() final;

    Matrix sum();

};


#endif //UNTITLED_SUMMATOR_H
