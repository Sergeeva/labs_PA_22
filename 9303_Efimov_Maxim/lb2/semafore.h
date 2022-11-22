#ifndef SEMAFORE_H
#define SEMAFORE_H


#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <string>
#include <list>
#include <chrono>


/*
 * This semaphore is implemented as weak (waiting processes is a set).
 */
class Semafore {
private:
    int                     count;
    std::mutex              mutex;
    std::condition_variable cond_var;

public:
                            Semafore(int count = 0);
    void                    up();
    void                    down();
};

#endif // SEMAFORE_H
