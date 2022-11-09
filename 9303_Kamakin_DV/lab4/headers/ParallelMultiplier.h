#ifndef UNTITLED_PARALLELMULTIPLIER_H
#define UNTITLED_PARALLELMULTIPLIER_H


#include <future>
#include "Worker.h"
#include "Matrix.h"
#include "MatricesHolder.h"
#include "SynchronizedQueue.h"
#include "MultiplyingThread.h"
#include "Configuration.h"
#include "MatricesMultiplier.h"

class ParallelMultiplier : public Worker {

private:

    SynchronizedQueue<MatricesHolder> &matrices_queue;

    SynchronizedQueue<Matrix> &sums_queue;

    MatricesMultiplier &multiplier;

    const int thread_count;

    const int iterations_count;

    std::vector<MultiplyingThread> initialize_threads(SynchronizedQueue<Runnable> &tasks) const;

public:

    ParallelMultiplier(SynchronizedQueue<MatricesHolder> &matrices_queue,
                       SynchronizedQueue<Matrix> &sums_queue,
                       MatricesMultiplier &multiplier,
                       int workers_count,
                       int iterations_count) : matrices_queue(matrices_queue),
                                               sums_queue(sums_queue),
                                               multiplier(multiplier),
                                               thread_count(workers_count),
                                               iterations_count(iterations_count) {}

    void run() final;

};


#endif //UNTITLED_PARALLELMULTIPLIER_H
