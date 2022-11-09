#ifndef UNTITLED_MULTIPLYINGTHREAD_H
#define UNTITLED_MULTIPLYINGTHREAD_H


#include "SynchronizedQueue.h"
#include "Runnable.h"

class MultiplyingThread : public Runnable {

private:
    int id;

    SynchronizedQueue<Runnable> &tasks;

    std::thread thread;

    void start();

public:
    MultiplyingThread(const MultiplyingThread &other) : id(other.id),
                                                        tasks(other.tasks) {}

    explicit MultiplyingThread(int id, SynchronizedQueue<Runnable> &tasks) : id(id),
                                                                             tasks(tasks),
                                                                             thread{} {}

    void run() override;

    void join();

};


#endif //UNTITLED_MULTIPLYINGTHREAD_H
