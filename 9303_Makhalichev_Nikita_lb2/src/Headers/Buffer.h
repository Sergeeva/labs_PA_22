#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "./Matrix.h"


class Buffer{
private:
    int max_size_;
    int current_size_;
    std::mutex mutex;
    std::queue<Matrix> queue;
    std::condition_variable can_be_consumed, can_be_produced;
public:
    Buffer(int max_size);
    Matrix consume();
    void produce(Matrix& matrix);
};

#endif
