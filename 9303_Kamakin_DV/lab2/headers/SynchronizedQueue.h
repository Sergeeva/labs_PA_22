#ifndef UNTITLED_SYNCHRONIZEDQUEUE_H
#define UNTITLED_SYNCHRONIZEDQUEUE_H

#include <queue>
#include <mutex>
#include <spdlog/spdlog.h>

template<typename T>
class SynchronizedQueue {

private:

    std::queue<T> queue;
    std::mutex mutex;
    std::atomic<bool> is_closed;
    std::condition_variable has_elements;

public:

    T pop() {
        spdlog::debug("Attempt to acquire lock");

        std::unique_lock<std::mutex> lock(mutex);

        spdlog::debug("Start waiting for objects in queue");

        has_elements.wait(lock, [this]() {
            if (is_closed) {
                throw std::runtime_error("Exhausted while waiting for an object");
            }

            return queue.size() != 0;
        });

        spdlog::debug("Done waiting for objects in queue");

        auto elem = queue.front();
        queue.pop();

        has_elements.notify_one();

        spdlog::debug("Element was removed from the queue");

        return elem;
    }

    void push(T &elem) {
        std::scoped_lock<std::mutex> lock(mutex);

        spdlog::debug("Element was successfully pushed to the queue");

        queue.push(elem);

        has_elements.notify_one();
    }

    void close() {
        is_closed = true;
        has_elements.notify_all();
    }

};

#endif
