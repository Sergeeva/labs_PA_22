#ifndef BUFFER_H
#define BUFFER_H

#include "definition.h"

template<typename T>
class Buffer {
public:
    Buffer(int);
    ~Buffer() = default;
    void produce(T);
    T consume();

private:
    std::queue<T> buffer;
    int bufferSize;
    int bufferCapacity;
    std::mutex mutex;
    std::condition_variable producerCondition;
    std::condition_variable consumerCondition;

};

template<typename T>
Buffer<T>::Buffer(int capacity) {
    bufferCapacity = capacity;
    bufferSize = 0;
}

template<typename T>
void Buffer<T>::produce(T element) {
    std::unique_lock<std::mutex> lock(mutex);
    producerCondition.wait(lock, [&] () -> bool {
        return bufferSize < bufferCapacity;
    });
    buffer.push(element);
    bufferSize++;
    consumerCondition.notify_one();
}

template<typename T>
T Buffer<T>::consume() {
    std::unique_lock<std::mutex> lock(mutex);
    consumerCondition.wait(lock, [&] () -> bool {
        return bufferSize > 0;
    });
    auto result = buffer.front();
    buffer.pop();
    bufferSize--;
    producerCondition.notify_one();
    return result;
}

#endif //BUFFER_H
