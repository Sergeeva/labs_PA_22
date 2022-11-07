#ifndef LAB2_BUFFER_H
#define LAB2_BUFFER_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include "./Matrix.h"


class Buffer{
private:
    int max_buffer_size;
    int cur_size;
    std::mutex mutex;
    std::queue<Matrix> queue;
    std::condition_variable is_possible_consume, is_possible_produce;
public:
    Buffer(int max_size);
    Matrix consume();
    void produce(Matrix& matrix);
};



#endif
