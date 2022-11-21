#ifndef LABS_PA_22_LAB2_BLOCKING_QUEUE_H
#define LABS_PA_22_LAB2_BLOCKING_QUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <sstream>
#include <utility>
#include <iostream>

template<typename T>
class BlockingQueue {

private:
    std::string name;

    std::queue<T> queue;
    size_t max_size;

    std::mutex mutex;
    std::condition_variable is_full_condition;
    std::condition_variable is_empty_condition;

    void print_text_with_thread_id(const std::string& text) const;

public:
    explicit BlockingQueue(std::string name, size_t max_size);

    void push(const T& value);

    T pop();
};

#include "blocking-queue.tpp"

#endif //LABS_PA_22_LAB2_BLOCKING_QUEUE_H
