#pragma once

#include <mutex>
#include <condition_variable>

class semaphore {
    std::mutex mutex;
    std::condition_variable condition;
    unsigned long count = 0;

public:
    void release() {
        std::lock_guard lock(mutex);
        ++count;
        condition.notify_one();
    }

    void acquire() {
        std::unique_lock lock(mutex);
        while (!count) condition.wait(lock);
        --count;
    }

    bool try_acquire() {
        std::lock_guard lock(mutex);
        if (count) {
            --count;
            return true;
        }
        return false;
    }
};
