#ifndef Queue_hpp
#define Queue_hpp

#include <stdio.h>
#include <atomic>
#include <iostream>
#include "Matrix.hpp"

#define SIZE 6

class Queue {
public:
  Queue();
  ~Queue();
  bool push(Matrix const& data);
  bool pop(Matrix& data);

private:
  struct Bucket {
    std::atomic<size_t> index_near_free;
    Matrix data;
  };
  Bucket* const buffer;
  std::atomic<int> tail;
  std::atomic<int> head;
};

#endif