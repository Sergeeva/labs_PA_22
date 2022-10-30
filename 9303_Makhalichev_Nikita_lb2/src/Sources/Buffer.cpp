#include "../Headers/Buffer.h"


Buffer::Buffer(int max_size): max_size_(max_size){
    this->current_size_ = 0;
};

void Buffer::produce(Matrix& matrix){
    std::unique_lock<std::mutex> locker(this->mutex);
    can_be_produced.wait(locker, [&]() -> bool {
        return this->current_size_ < this->max_size_;
    });
    queue.push(matrix);
    this->current_size_++;
    locker.unlock();
    can_be_consumed.notify_one();
}

Matrix Buffer::consume(){
    std::unique_lock<std::mutex> locker(this->mutex);
    can_be_consumed.wait(locker, [&]() -> bool {
        return this->current_size_ > 0;
    });
    Matrix matrix = queue.front();
    queue.pop();
    this->current_size_--;
    locker.unlock();
    can_be_produced.notify_one();
    return matrix;
}
