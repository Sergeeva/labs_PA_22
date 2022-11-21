#ifndef Buffer_hpp
#define Buffer_hpp

#include <condition_variable>
#include <stdio.h>
#include "Matrix.hpp"

#define BUFFER_CAPACITY 2

class Buffer {
    Matrix buffer[BUFFER_CAPACITY];
    int buffer_size;
    int producer_index;
    int consumer_index;

    std::mutex mtx;
    std::condition_variable not_empty;
    std::condition_variable not_full;

public:
    void produce(Matrix matrix, bool needToPrint);

    Matrix consume();

    Buffer();
};

#endif