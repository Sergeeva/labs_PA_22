#include <iostream>
#include <condition_variable>

#include "utils.h"

#define bufferSize 10
#define threadsSize 10
#define N 2
#define M 2


std::mutex mutexForOriginMatrices;
std::mutex mutexForResultMatrices;

std::condition_variable originBufferNotFull, originBufferNotEmpty;
std::condition_variable resultBufferNotFull, resultBufferNotEmpty;

std::vector<std::pair<Matrix, Matrix>> originMatricesBuffer;
std::vector<Matrix> resultMatricesBuffer;

Matrix sumMatrices(std::pair<Matrix, Matrix> &matricesForSum) {
    Matrix firstMatrix = matricesForSum.first;
    Matrix secondMatrix = matricesForSum.second;

    Matrix resultMatrix(firstMatrix.size(), std::vector<int>(firstMatrix[0].size()));

    for (int i = 0; i < firstMatrix.size(); i++) {
        for (int j = 0; j < firstMatrix[i].size(); j++) {
            resultMatrix[i][j] = firstMatrix[i][j] + secondMatrix[i][j];
        }
    }

    return resultMatrix;
}

void consumeResultMatrices() {
    while (true) {
        std::unique_lock<std::mutex> ul(mutexForResultMatrices);

        resultBufferNotEmpty.wait(ul, []() {
            return resultMatricesBuffer.size() > 0;
        });

        Matrix resultMatrix = resultMatricesBuffer.back();
        resultMatricesBuffer.pop_back();

        printMatrix(resultMatrix, std::cout);

        ul.unlock();

        resultBufferNotFull.notify_one();
    }
}

void produceResultMatrices(std::pair<Matrix, Matrix> &matricesForSum) {
    Matrix resultMatrix = sumMatrices(matricesForSum);

    std::unique_lock<std::mutex> ul(mutexForResultMatrices);

    resultBufferNotFull.wait(ul, []() {
        return resultMatricesBuffer.size() != bufferSize;
    });

    resultMatricesBuffer.push_back(resultMatrix);

    ul.unlock();

    resultBufferNotEmpty.notify_one();
}

void consumeOriginMatrices() {
    while (true) {
        std::unique_lock<std::mutex> ul(mutexForOriginMatrices);

        originBufferNotEmpty.wait(ul, []() {
           return originMatricesBuffer.size() > 0;
        });

        std::pair<Matrix, Matrix> matricesForSum = originMatricesBuffer.back();
        originMatricesBuffer.pop_back();

        ul.unlock();

        produceResultMatrices(matricesForSum);

        originBufferNotFull.notify_one();
    }
}



void produceOriginMatrices() {
    while (true) {
        Matrix firstMatrix = generateMatrix(N, M);
        Matrix secondMatrix = generateMatrix(N, M);
        std::pair<Matrix, Matrix> matricesForSum;
        matricesForSum.first = firstMatrix;
        matricesForSum.second = secondMatrix;

        std::unique_lock<std::mutex> ul(mutexForOriginMatrices);

        originBufferNotFull.wait(ul, []() {
            return originMatricesBuffer.size() != bufferSize;
        });

        originMatricesBuffer.push_back(matricesForSum);

        ul.unlock();

        originBufferNotEmpty.notify_one();
    }
}


int main() {
    originMatricesBuffer.reserve(bufferSize);
    resultMatricesBuffer.reserve(bufferSize);
    std::vector<std::thread> threads;
    threads.reserve(threadsSize * 3);

    for (int i = 0; i < threadsSize; i++) {
        std::thread producerThread(produceOriginMatrices);
        threads.push_back(std::move(producerThread));

        std::thread originConsumerThread(consumeOriginMatrices);
        threads.push_back(std::move(originConsumerThread));

        std::thread resultConsumerThread(consumeResultMatrices);
        threads.push_back(std::move(resultConsumerThread));
    }

    for (auto &thread : threads) {
        thread.join();
    }

    return 0;
}
