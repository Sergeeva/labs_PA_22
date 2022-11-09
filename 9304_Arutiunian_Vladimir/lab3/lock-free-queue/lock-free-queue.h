#ifndef LABS_PA_22_LAB3_LOCK_FREE_QUEUE_H
#define LABS_PA_22_LAB3_LOCK_FREE_QUEUE_H

#include <string>
#include "node/node.h"

template<class T>
class LockFreeQueue {
private:
    std::atomic<Node<T>*> head;
    std::atomic<Node<T>*> tail;
    std::string name;
public:
    explicit LockFreeQueue(std::string_view name);

    ~LockFreeQueue();

    void push(const T& value);

    T pop();
};

#include "lock-free-queue.tpp"

#endif //LABS_PA_22_LAB3_LOCK_FREE_QUEUE_H
