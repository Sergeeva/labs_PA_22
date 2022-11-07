#ifndef UNTITLED_SUMMATORTHREAD_H
#define UNTITLED_SUMMATORTHREAD_H


#include "SumTask.h"
#include "SynchronizedQueue.h"
#include "Runnable.h"

class SummatorThread : public Runnable {

private:
    int id;

    std::atomic<int> &processed_batches;

    SynchronizedQueue<SumTask> &tasks;

    std::thread thread;

    void start();

public:
    SummatorThread(const SummatorThread &other) : id(other.id),
                                                  processed_batches(other.processed_batches),
                                                  tasks(other.tasks) {}

    explicit SummatorThread(int id,
                            std::atomic<int> &processed_batches,
                            SynchronizedQueue<SumTask> &tasks) : id(id),
                                                                 processed_batches(processed_batches),
                                                                 tasks(tasks),
                                                                 thread{} {}

    void run();

    void join();

};


#endif //UNTITLED_SUMMATORTHREAD_H
