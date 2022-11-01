#include "Queue.hpp"

Queue::Queue(): buffer(new Bucket [SIZE]) {
  for (size_t i = 0; i < SIZE; i ++)
    buffer[i].index_near_free.store(i, std::memory_order_relaxed);
  head.store(0, std::memory_order_relaxed);
  tail.store(0, std::memory_order_relaxed);
}

  Queue::~Queue() {
    delete [] buffer;
  }

bool Queue::pop(Matrix& data) {
  Bucket* bucket;
  int pos = head.load(std::memory_order_relaxed);
  while(true) {
    bucket = &buffer[pos % SIZE];
    int seq = bucket -> index_near_free.load(std::memory_order_acquire);
    int diff = seq - (pos + 1);
    if (diff == 0) {
      if (head.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
        break;
    }
    else if (diff < 0)
      continue;
    else
      pos = tail.load(std::memory_order_relaxed);
  }
  data = bucket -> data;
  bucket -> index_near_free.store(pos + SIZE, std::memory_order_release);
  return true;
}

bool Queue::push(Matrix const& data) {
  Bucket* bucket;
  int pos = tail.load(std::memory_order_relaxed);
  while(true) {
    bucket = &buffer[pos % SIZE];
    int seq = bucket -> index_near_free.load(std::memory_order_acquire);
    int diff = seq - pos;
    if (diff == 0) {
      if (tail.compare_exchange_weak(pos, pos + 1, std::memory_order_relaxed))
        break;
    }
    else if (diff < 0)
      continue;
    else
       pos = tail.load(std::memory_order_relaxed);
  }
  bucket -> data = data;
  bucket -> index_near_free.store(pos + 1, std::memory_order_release);
  return true;
}