#ifndef UNTITLED_PARALLELSUMMATOR_H
#define UNTITLED_PARALLELSUMMATOR_H


#include <future>
#include "Worker.h"
#include "Matrix.h"
#include "MatricesHolder.h"

class ParallelSummator : public Worker {

private:
    MatricesHolder &holder;

    std::promise<Matrix> result;

    int thread_count = 1;

    int calculate_batch_size(int total_size) const;

    static void sum_thread(int offset,
                           int batch_size,
                           int total_size,
                           Matrix &target,
                           MatricesHolder &holder);


public:

    ParallelSummator(MatricesHolder &holder,
                     std::promise<Matrix> result_future,
                     int thread_count) : holder(holder),
                                         result(std::move(result_future)),
                                         thread_count(thread_count) {};

    void run() final;

    Matrix sum();

};


#endif //UNTITLED_PARALLELSUMMATOR_H
