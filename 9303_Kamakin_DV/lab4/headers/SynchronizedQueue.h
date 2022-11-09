#ifndef UNTITLED_SYNCHRONIZEDQUEUE_H
#define UNTITLED_SYNCHRONIZEDQUEUE_H

#include <queue>
#include <mutex>
#include <atomic>
#include <memory>
#include <optional>
#include <condition_variable>
#include "Log.h"


template<typename T>
class SynchronizedQueue {

private:

    std::queue<T *> queue;
    std::mutex mutex;
    std::atomic<bool> is_closed;
    std::condition_variable has_elements;

public:

    std::shared_ptr<T> pop() {
        std::unique_lock<std::mutex> lock(mutex);

        has_elements.wait(lock, [this]() {
            return is_closed || queue.size() != 0;
        });

        if (queue.size() == 0) {
            return std::shared_ptr<T>{};
        } else {
            auto elem = queue.front();
            queue.pop();

            has_elements.notify_one();

            return std::shared_ptr<T>(elem);
        }
    }

    void push(T *elem) {
        std::scoped_lock<std::mutex> lock(mutex);

        queue.push(elem);

        has_elements.notify_one();
    }

    void close() {
        is_closed = true;
        has_elements.notify_all();
    }

};

#endif
