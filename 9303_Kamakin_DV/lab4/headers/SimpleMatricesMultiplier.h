#ifndef UNTITLED_SIMPLEMATRICESMULTIPLIER_H
#define UNTITLED_SIMPLEMATRICESMULTIPLIER_H

#include "Matrix.h"
#include "MatricesHolder.h"
#include "MatricesMultiplier.h"

class SimpleMatricesMultiplier : public MatricesMultiplier {

private:
    int thread_count;

public:

    class SimpleMultiplyTask : public Runnable {
    private:
        int start_index;
        int end_index;
        Matrix *target;
        MatricesHolder *holder;
        std::atomic<int> *processed_tasks;
        std::promise<Matrix *> *promise;
        int total_tasks;

    public:
        SimpleMultiplyTask(int start_index,
                           int end_index,
                           Matrix *target,
                           MatricesHolder *holder,
                           std::atomic<int> *processed_tasks,
                           std::promise<Matrix *> *promise,
                           int total_tasks) : start_index(start_index),
                                              end_index(end_index),
                                              target(target),
                                              holder(holder),
                                              processed_tasks(processed_tasks),
                                              promise(promise),
                                              total_tasks(total_tasks) {}

        void run() override;

    };

    explicit SimpleMatricesMultiplier(int thread_count) : thread_count(thread_count) {}

    std::promise<Matrix *> *multiply(MatricesHolder *matrices,
                                     SynchronizedQueue<Runnable> *task_consumer) override;

};


#endif //UNTITLED_SIMPLEMATRICESMULTIPLIER_H
