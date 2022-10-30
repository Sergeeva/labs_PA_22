#ifndef UNTITLED_PARALLELSUMMATOR_H
#define UNTITLED_PARALLELSUMMATOR_H


#include <future>
#include "Worker.h"
#include "Matrix.h"
#include "MatricesHolder.h"
#include "SynchronizedQueue.h"
#include "SummatorThread.h"

class ParallelSummator : public Worker {

private:

    SynchronizedQueue<MatricesHolder> &matrices_queue;

    SynchronizedQueue<Matrix> &sums_queue;

    const int thread_count;

    const int iterations_count;

    std::vector<SummatorThread> initialize_summators(SynchronizedQueue<SumTask> &tasks,
                                                     std::atomic<int> &processed_batches);

    int calculate_batch_size(int total_size) const;

    int calculate_batches_count(int total_size, int batch_size) const;

    Matrix sum(MatricesHolder &holder,
               SynchronizedQueue<SumTask> &tasks,
               std::vector<SummatorThread> &threads,
               std::atomic<int> &processed_batches);

public:

    ParallelSummator(SynchronizedQueue<MatricesHolder> &matrices_queue,
                     SynchronizedQueue<Matrix> &sums_queue,
                     const int thread_count,
                     const int iterations_count) : matrices_queue(matrices_queue),
                                                   sums_queue(sums_queue),
                                                   thread_count(thread_count),
                                                   iterations_count(iterations_count) {};

    void run() final;

};


#endif //UNTITLED_PARALLELSUMMATOR_H
