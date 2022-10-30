#include "../Headers/Buffer.h"


Buffer::Buffer(int max_size): max_buffer_size(max_size){
    this->cur_size = 0;
}

void Buffer::produce(Matrix& matrix){
    std::unique_lock<std::mutex> lock(this->mutex);
    is_possible_produce.wait(lock, [&]() -> bool {
        return this->cur_size < this->max_buffer_size;
    });
    queue.push(matrix);
    this->cur_size++;
    is_possible_consume.notify_one();
}

Matrix Buffer::consume(){
    std::unique_lock<std::mutex> lock(this->mutex);
    is_possible_consume.wait(lock, [&]() -> bool {
        return this->cur_size > 0;
    });
    Matrix matrix = queue.front();
    queue.pop();
    this->cur_size--;
    is_possible_produce.notify_one();
    return matrix;
}