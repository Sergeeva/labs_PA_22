#include <thread>
#include <iostream>
#include <cstdlib>
#include "Queue.hpp"
#include "Matrix.hpp"
#include "strassen.hpp"

void produceMatrix(Queue& buffer, int iterationsCount, std::mutex& print_mtx) {
  for (int thread_num = 0; thread_num < iterationsCount; thread_num++) {
    Matrix matrix;
    matrix.print(print_mtx);
    buffer.push(matrix);
  }
}

void consumeSumProduceMatrix(Queue& buffer, Queue& writeBuffer, int iterationsCount) {
  for (int thread_num = 0; thread_num < iterationsCount; thread_num++) {
    Matrix matrix;
    buffer.pop(matrix);
    clock_t start_time = clock();
    matrix.multiply(matrix.first_matrix, matrix.second_matrix);
//    matrix.result = multiplyShtrassen(matrix.first_matrix, matrix.second_matrix);
    printf("Time taken: %.6f\n", (double)(clock() - start_time)/CLOCKS_PER_SEC);
    writeBuffer.push(matrix);
  }
}

void consumeAndWrite(Queue& buffer, std::ofstream& file, std::mutex& file_mtx, int iterationsCount) {
  for (int thread_num = 0; thread_num < iterationsCount; thread_num++) {
    Matrix matrix;
    buffer.pop(matrix);
    matrix.printToFile(file, file_mtx);
  }
}

void makeSumConsumers(Queue &buffer, Queue &writeBuffer, std::thread* consumeThread, int iterationsCount, int thread_num) {
    for (int i = 0; i < thread_num; i++) {
        consumeThread[i] = std::thread{consumeSumProduceMatrix, std::ref(buffer), std::ref(writeBuffer), iterationsCount};
    }
}
void makeInitProducers(Queue &buffer, std::thread* produceThread, int iterationsCount, std::mutex& print_mtx, int thread_num) {
  for (int i = 0; i < thread_num; i++) {
    produceThread[i] = std::thread{produceMatrix, std::ref(buffer), iterationsCount, std::ref(print_mtx)};
  }
}

void makeResultWriters(Queue &buffer, std::thread* writeThread, std::ofstream& file, std::mutex& file_mtx, int iterationsCount, int thread_num) {
  for (int i = 0; i < thread_num; i++) {
    writeThread[i] = std::thread{consumeAndWrite, std::ref(buffer), std::ref(file), std::ref(file_mtx), iterationsCount};
  }
}

int find_divisor(int n) {
  for (int i = n - 1; i > 1; i--) {
    if (n % i == 0)  {
      return i > 12 ? 1 : i;
    }
  }
  return 1;
}

int main(int argc, const char * argv[]) {
  srand (time(nullptr));

  int iterationsCount;
  iterationsCount = 1;
//  std::cout << "Enter iterations count\n";
//  std::cin >> iterationsCount;

  int thread_num = find_divisor(iterationsCount);
  iterationsCount = iterationsCount / thread_num;

  std::mutex file_mtx;
  std::mutex print_mtx;
  std::ofstream outfile;
  outfile.open("result.txt", std::ios_base::app);

  Queue source_queue;
  Queue result_queue;
  std::thread produceThread[thread_num];
  std::thread consumeThread[thread_num];
  std::thread writeThread[thread_num];

  makeInitProducers(source_queue, produceThread, iterationsCount, print_mtx, thread_num);
  makeSumConsumers(source_queue, result_queue, consumeThread, iterationsCount, thread_num);
  makeResultWriters(result_queue, writeThread, outfile, file_mtx, iterationsCount, thread_num);

  for (int i = 0; i < thread_num; i++) {
    produceThread[i].join();
    consumeThread[i].join();
    writeThread[i].join();
  }
  return 0;
}