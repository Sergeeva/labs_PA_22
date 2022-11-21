#ifndef UNTITLED_STARTER_H
#define UNTITLED_STARTER_H

#include <future>
#include "Runnable.h"
#include "MatricesHolder.h"
#include "SynchronizedQueue.h"

class Starter : public Runnable {

private:
    static void sum(SynchronizedQueue<MatricesHolder> &matrices_queue,
                    SynchronizedQueue<Matrix> &sums_queue,
                    int iterations_count,
                    int thread_count);

    static void run(const std::string &file_name, int iterations_count, int thread_count);

public:

    void run() final;

};


#endif //UNTITLED_STARTER_H
