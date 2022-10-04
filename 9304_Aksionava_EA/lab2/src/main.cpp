#include <thread>
#include <iostream>
#include "Buffer.hpp"
#include "Matrix.hpp"
#include <stdlib.h>


void produceMatrix(Buffer& buffer, int iterationsCount) {
  for (int thread_num = 0; thread_num < iterationsCount; thread_num++) {
    Matrix matrix;
    buffer.produce(matrix, true);
  }
}

void consumeSumProduceMatrix(Buffer& buffer, Buffer& writeBuffer, int iterationsCount) {
  for (int thread_num = 0; thread_num < iterationsCount; thread_num++) {
    Matrix matrix = buffer.consume();
    matrix.sum();
    writeBuffer.produce(matrix, false);
  }
}

void consumeAndWrite(Buffer& buffer, std::ofstream& file, std::mutex& file_mtx, int iterationsCount) {
  for (int thread_num = 0; thread_num < iterationsCount; thread_num++) {
    Matrix matrix = buffer.consume();
    matrix.printToFile(file, file_mtx);
  }
}

void makeSumConsumers(Buffer &buffer, Buffer &writeBuffer, std::thread& consumeThread, int iterationsCount) {
    consumeThread = std::thread{consumeSumProduceMatrix, std::ref(buffer), std::ref(writeBuffer), iterationsCount};
//    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}
void makeInitProducers(Buffer &buffer, std::thread& produceThread, int iterationsCount) {
    produceThread = std::thread{produceMatrix, std::ref(buffer), iterationsCount};
//    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

}

void makeResultWriters(Buffer &buffer, std::thread& writeThread, std::ofstream& file, std::mutex& file_mtx, int iterationsCount) {
    writeThread = std::thread{consumeAndWrite, std::ref(buffer), std::ref(file), std::ref(file_mtx), iterationsCount};
//    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

int main(int argc, const char * argv[]) {
  srand (time(NULL));

  int iterationsCount;
  std::cout << "Enter iterations count\n";
  std::cin >> iterationsCount;

  std::mutex file_mtx;
  std::ofstream outfile;
  outfile.open("result.txt", std::ios_base::app);

  Buffer source_buffer;
  Buffer result_buffer;

  std::thread consumeThread;
  std::thread produceThread;
  std::thread writeThread;

  makeSumConsumers(source_buffer, result_buffer, consumeThread, iterationsCount);
  makeInitProducers(source_buffer, produceThread, iterationsCount);
  makeResultWriters(result_buffer, writeThread, outfile, file_mtx, iterationsCount);

  produceThread.join();
  consumeThread.join();
  writeThread.join();

  return 0;
}