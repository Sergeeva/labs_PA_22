#include <iostream>
#include "Buffer.hpp"

Buffer::Buffer() {
  buffer_size = 0;
  producer_index = 0;
  consumer_index = 0;
}


void Buffer::produce(Matrix matrix, bool needToPrint) {
  std::unique_lock<std::mutex> unique_lock(mtx);

  not_full.wait(unique_lock, [this]() {
      return buffer_size != BUFFER_CAPACITY;
  });

  if(needToPrint) {
   matrix.print();
  }
  buffer[consumer_index] = matrix;

  consumer_index = (consumer_index + 1) % BUFFER_CAPACITY;
  buffer_size++;

  unique_lock.unlock();

  not_empty.notify_one();
}

Matrix Buffer::consume() {
  std::unique_lock<std::mutex> unique_lock(mtx);

  not_empty.wait(unique_lock, [this]() {
      return buffer_size != 0;
  });

  Matrix result = buffer[producer_index];

  producer_index = (producer_index + 1) % BUFFER_CAPACITY;
  buffer_size--;

  unique_lock.unlock();

  not_full.notify_one();

  return result;
}