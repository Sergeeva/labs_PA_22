#include <iostream>
#include "threadsafe_queue.h"

#include "utils.h"

#define threadsSize 1

std::mutex producerMutex;
std::mutex consumerMutex;


Matrix sumMatrices(std::pair<Matrix, Matrix> &matricesForSum) {
    Matrix firstMatrix = matricesForSum.first;
    Matrix secondMatrix = matricesForSum.second;

    Matrix_type resultMatrix(firstMatrix.matrix.size(), std::vector<int>(firstMatrix.matrix[0].size()));


    for (int i = 0; i < firstMatrix.matrix.size(); i++) {
        for (int j = 0; j < firstMatrix.matrix[i].size(); j++) {
            resultMatrix[i][j] = firstMatrix.matrix[i][j] + secondMatrix.matrix[i][j];
        }
    }

    Matrix matrixObj;
    matrixObj.matrix = resultMatrix;

    return matrixObj;
}

template <typename T>
void produceMatrices(threadsafe_queue<T> &queue) {
    Matrix firstMatrix;
    Matrix secondMatrix;
    std::pair<Matrix, Matrix> matricesForSum;
    matricesForSum.first = firstMatrix;
    matricesForSum.second = secondMatrix;
    std::lock_guard<std::mutex> ul(producerMutex);
    queue.push(matricesForSum);
}

template <typename T>
void consumeMatrices(threadsafe_queue<T> &queue) {
    std::pair<Matrix, Matrix> matrices = *queue.wait_and_pop();
    Matrix summedMatrix = sumMatrices(matrices);
    std::lock_guard<std::mutex> ul(consumerMutex);
    summedMatrix.printMatrix(std::cout);
}



int main() {
    threadsafe_queue<std::pair<Matrix, Matrix>> queue;
    std::vector<std::thread> threads;
    threads.reserve(threadsSize * 2 );

    auto begin = std::chrono::steady_clock::now();
    int iterationsNumber = 10000;
    for (int i = 0; i < threadsSize; i++) {
        std::thread addMatrixThread([&](){
            for (int i = 0; i < iterationsNumber / 2; i++) {
                produceMatrices(queue);
            }
        });

        threads.push_back(std::move(addMatrixThread));

        std::thread addMatrixThread1([&](){
            for (int i = 0; i < iterationsNumber / 2; i++) {
                produceMatrices(queue);
            }
        });

        threads.push_back(std::move(addMatrixThread1));

        std::thread getMatrixThread([&](){
            for (int i = 0; i < iterationsNumber / 2; i++) {
                consumeMatrices(queue);
            }
        });

        threads.push_back(std::move(getMatrixThread));

        std::thread getMatrixThread1([&](){
            for (int i = 0; i < iterationsNumber / 2; i++) {
                consumeMatrices(queue);
            }
        });

        threads.push_back(std::move(getMatrixThread1));
    }

    for (auto &thread : threads) {
        thread.join();
    }

    auto end = std::chrono::steady_clock::now();
    std::cout << std::fixed << "Time: " <<  std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()  << " milliseconds" << "\n\n";


    return 0;
}
