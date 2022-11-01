#include <iostream>
#include "threads/threads.h"
#include "matrixFunc/matrixFunc.h"
#include "fileWriter/fileWriter.h"
#include "queue/LockFreeQueue.h"
#include <thread>

using namespace std;

void produceMatrixPair(LockFreeQueue<MatrixPair>& buffer, int iterations) {
    for (int i = 0; i < iterations; i++) {
//        Matrix matrix1(N, vector<int>(M, 10));
//        Matrix matrix2(N, vector<int>(M, 5));
//        MatrixPair pairOfMatrix = std::make_pair(matrix1, matrix2);
        MatrixPair pairOfMatrix = generateMatrixPair();
        buffer.produce(pairOfMatrix);
    }
}

void consumeMatrixPair(LockFreeQueue<MatrixPair>& buffer,
                       LockFreeQueue<Matrix>& fileWriterBuffer,
                       int iterations) {
    for (int i = 0; i < iterations; i++) {
        MatrixPair pairOfMatrix = *buffer.consume();
        Matrix result(N, vector<int>(M, 0));
        multiThreadsAddition(pairOfMatrix.first, pairOfMatrix.second, result);
        fileWriterBuffer.produce(result);
    }
}

void consumeResultMatrix(LockFreeQueue<Matrix>& fileWriterBuffer, int iterations) {
    for (int i = 0; i < iterations; i++) {
        Matrix resultMatrix = *fileWriterBuffer.consume();
        writeToFile("result_lab3", resultMatrix, "");
    }
}

int main() {
    int iterations = 1000;
    int countOfThreads = 30;
//    Buffer generateBuffer;
//    Buffer writerBuffer;
    auto generateBuffer = new LockFreeQueue<MatrixPair>();
    auto writerBuffer = new LockFreeQueue<Matrix>();
    std::vector<std::thread> vecOfThreads;
    vecOfThreads.reserve(countOfThreads);

    for (int i = 0; i < countOfThreads; i++) {
        vecOfThreads.emplace_back(std::thread(consumeMatrixPair, std::ref(*generateBuffer), std::ref(*writerBuffer),
                                               iterations));

        vecOfThreads.emplace_back(std::thread(produceMatrixPair, std::ref(*generateBuffer), iterations));
        vecOfThreads.emplace_back(std::thread(consumeResultMatrix, std::ref(*writerBuffer),
                           iterations));
    }

    for (int i = 0; i < vecOfThreads.size(); i++) {
        vecOfThreads[i].join();
    }
    return 0;
}