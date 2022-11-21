#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class BlockingQueue {
private:
    int maxSize;
    std::queue<T> queue;

    std::mutex mutex;
    std::condition_variable notFull;
    std::condition_variable notEmpty;

public:
    explicit BlockingQueue(int maxSize) : maxSize(maxSize) {};

    T pop() {
        std::unique_lock<std::mutex> uniqueLock(mutex);

        notEmpty.wait(uniqueLock, [this]() {
            return queue.size() != 0;
        });

        T result = queue.front();
        queue.pop();

        notFull.notify_one();
        return result;
    };

    void push(const T &item) {
        std::unique_lock<std::mutex> uniqueLock(mutex);

        notFull.wait(uniqueLock, [this]() {
            return queue.size() != maxSize;
        });

        queue.push(item);

        notEmpty.notify_one();
    };
};
