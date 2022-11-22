#include "Buffer.h"


Buffer::Buffer(int max_size): size_max(max_size)
{
    this->size_curr = 0;
};

void Buffer::produce(Matrix& matrix)
{
    std::unique_lock<std::mutex> lock(this->mutex);

    produced.wait(lock, [&]() -> bool 
    {
        return this->size_curr < this->size_max;
    });

    queue.push(matrix);

    this->size_curr++;

    lock.unlock();

    consumed.notify_one();
}

Matrix Buffer::consume(){

    std::unique_lock<std::mutex> lock(this->mutex);

    consumed.wait(lock, [&]() -> bool 
    {
        return this->size_curr > 0;
    });

    Matrix matrix = queue.front();

    queue.pop();

    this->size_curr--;

    lock.unlock();

    produced.notify_one();
    
    return matrix;
}