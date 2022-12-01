#ifndef PA_LAB2_BQUEUE_H
#define PA_LAB2_BQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

#include "../matrix/Matrix.h"

template <typename T>
class BQueue {
    int max_size = Config::max_size;
    int size = 0;

    std::queue<T> data;

    std::mutex block;
    std::condition_variable supply;
    std::condition_variable demand;

public:
    BQueue() = default;
    explicit BQueue(int size_limit): max_size(size_limit) {
        this->size = 0;
    };

    void add(T item) {
        std::unique_lock<std::mutex> lock (this->block);

        this->supply.wait(lock, [&]() -> bool {
            return this->size < this->max_size;
        });

        this->data.push(item);
        this->size++;

        this->demand.notify_one();
        lock.unlock();

    }

    T get() {
        std::unique_lock<std::mutex> lock(this->block);

        this->demand.wait(lock, [&]() -> bool
        {
            return this->size > 0;
        });

        T value = this->data.front();
        this->data.pop();
        this->size--;

        this->supply.notify_one();
        lock.unlock();

        return value;
    };
};


#endif //PA_LAB2_BQUEUE_H
