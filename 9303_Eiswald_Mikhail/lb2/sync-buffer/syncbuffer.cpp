#include "syncbuffer.h"

SyncBuffer::SyncBuffer(size_t size): size(size) {
    data = (Matrix*) new char[sizeof(Matrix)*size];
    read_index = write_index = 0;
    empty = true;
}

void SyncBuffer::push(const Matrix &matrix) {
    unique_lock<mutex> unique_lock(buffer_mutex);
    if(!empty && read_index == write_index) {
        able_to_write.wait(unique_lock, [this](){
            return empty || read_index != write_index;
        });
    }
    data[write_index] = matrix;
    write_index = (write_index + 1) % size;
    empty = false;
    unique_lock.unlock();
    able_to_read.notify_one();
}

Matrix SyncBuffer::pop() {
    unique_lock<mutex> unique_lock(buffer_mutex);
    if(empty) {
        able_to_read.wait(unique_lock, [this](){
            return !empty;
        });
    }
    Matrix& result = data[read_index];
    read_index = (read_index + 1) % size;
    if(read_index == write_index) empty = true;
    able_to_write.notify_one();
    return result;
}
