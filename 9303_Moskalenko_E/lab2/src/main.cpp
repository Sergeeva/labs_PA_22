#include <iostream>
#include "threads/threads.h"
#include "matrixBuffer/Buffer.h"
#include "matrixFunc/matrixFunc.h"
#include "fileWriter/fileWriter.h"
#include <thread>

using namespace std;

void produceMatrixPair(Buffer& buffer, int iterations) {
    for (int i = 0; i < iterations; i++) {
        MatrixPair pairOfMatrix = generateMatrixPair();
        buffer.produce(pairOfMatrix);
    }
}

void consumeMatrixPair(Buffer& buffer, Buffer& fileWriterBuffer, int iterations) {
    for (int i = 0; i < iterations; i++) {
        MatrixPair pairOfMatrix = buffer.consume();
        Matrix result(N, vector<int>(M, 0));
        Matrix empty(N, vector<int>(M, 0));

        multiThreadsAddition(pairOfMatrix.first, pairOfMatrix.second, result);
        fileWriterBuffer.produce(std::make_pair(result, empty));
    }
}

void consumeResultMatrix(Buffer& fileWriterBuffer, int iterations) {
    for (int i = 0; i < iterations; i++) {
        MatrixPair resultMatrix = fileWriterBuffer.consume();
        writeToFile("resultMatrix", resultMatrix.first, "");
    }
}

int main() {
    int iterations = 3;
    int countOfThreads = 15;
    Buffer generateBuffer;
    Buffer writerBuffer;
    std::vector<std::thread> vecOfThreads;
    vecOfThreads.reserve(countOfThreads);

    for (int i = 0; i < countOfThreads; i++) {
        vecOfThreads.emplace_back(std::thread(consumeMatrixPair, std::ref(generateBuffer),
                             std::ref(writerBuffer), iterations));

        vecOfThreads.emplace_back(std::thread(produceMatrixPair, std::ref(generateBuffer), iterations));
        vecOfThreads.emplace_back(std::thread(consumeResultMatrix, std::ref(writerBuffer),
                           iterations));
    }

    for (int i = 0; i < vecOfThreads.size(); i++) {
        vecOfThreads[i].join();
    }
    return 0;
}