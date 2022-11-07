#ifndef UNTITLED_OUTPUTWRITER_H
#define UNTITLED_OUTPUTWRITER_H


#include <future>
#include "Runnable.h"
#include "Worker.h"
#include "Matrix.h"
#include "SynchronizedQueue.h"

class OutputWriter : public Worker {

private:
    SynchronizedQueue<Matrix> &sums_queue;

    const int iterations_count;

    void print_result(std::ofstream &file, Matrix &matrix);

public:
    explicit OutputWriter(SynchronizedQueue<Matrix> &sums_queue,
                          const int iterations_count) : sums_queue(sums_queue),
                                                        iterations_count(iterations_count) {};

    void run() final;

};


#endif //UNTITLED_OUTPUTWRITER_H
