#ifndef UNTITLED_SYNCHRONIZEDQUEUE_H
#define UNTITLED_SYNCHRONIZEDQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "Log.h"

template<typename T>
class SynchronizedQueue {

private:

    std::queue<T> queue;
    std::mutex mutex;
    std::atomic<bool> is_closed;
    std::condition_variable has_elements;

public:

    T pop() {
        if (is_closed) {
            throw std::runtime_error("Queue is not producing elements");
        }

        Log::info("Attempt to acquire lock");

        std::unique_lock<std::mutex> lock(mutex);

        Log::info("Start waiting for objects in queue");

        has_elements.wait(lock, [this]() {
            if (is_closed) {
                throw std::runtime_error("Exhausted while waiting for an object");
            }

            return queue.size() != 0;
        });

        Log::info("Done waiting for objects in queue");

        auto elem = queue.front();
        queue.pop();

        has_elements.notify_one();

        Log::info("Element was removed from the queue");

        return elem;
    }

    void push(T &elem) {
        if (is_closed) {
            throw std::runtime_error("Queue is not consuming elements");
        }

        std::scoped_lock<std::mutex> lock(mutex);

        Log::info("Element was successfully pushed to the queue");

        queue.push(elem);

        has_elements.notify_one();
    }

    void close() {
        is_closed = true;
        has_elements.notify_all();
    }

};

#endif
