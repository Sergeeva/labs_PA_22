#include <iostream>
#include <thread>
#include <vector>
#include "utils.h"

typedef std::vector<std::vector<int>> Matrix;

const int m_width = 15;
const int m_height = 15;


Matrix sum(Matrix m1, Matrix m2) {

    Matrix res_m(m1.size(), std::vector<int>(m1[0].size()));

    for (int i = 0; i < m1.size(); i++) {
        for (int j = 0; j < m1[i].size(); j++) {
            res_m[i][j] = m1[i][j] + m2[i][j];
        }
    }

    return res_m;
}

void sumM(CustomQueue &matrixQueue, CustomQueue &resultQueue, int count) {
    for (int i = 0; i < count; i++) {
        Matrix matrix1 = matrixQueue.pop();
        Matrix matrix2 = matrixQueue.pop();
        resultQueue.push(sum(matrix1, matrix2));
    }
}

void generateM(CustomQueue &stack, int count) {
    for (int i = 0; i < count; i++) {
        stack.push(generateMatrix(m_width, m_height));
        stack.push(generateMatrix(m_width, m_height));
    }
}

void resM(CustomQueue &resultMatrix, int count) {
    for (int i = 0; i < count; i++) {
        Matrix matrix = resultMatrix.pop();
        printMatrix(matrix);
    }
}

int main() {
    CustomQueue matrixQueue;
    CustomQueue resultQueue;

    int count = 1000;
    int consumer_threads = 10;
    int producer_threads = 10;

    auto start = std::chrono::steady_clock::now();

    std::vector<std::thread> consumers;
    for (int i = 0; i < consumer_threads; ++i) {
        std::thread consumer{sumM, std::ref(matrixQueue), std::ref(resultQueue), count};
        consumers.push_back(std::move(consumer));
    }

    std::vector<std::thread> producers;
    for (int i = 0; i < producer_threads; ++i) {
        std::thread producer{generateM, std::ref(matrixQueue), count};
        producers.push_back(std::move(producer));
    }

    std::thread writer{resM, std::ref(resultQueue), count * consumer_threads};

    for (auto &thread: consumers) {
        thread.join();
    }

    for (auto &thread: producers) {
        thread.join();
    }

    writer.join();


    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Elapsed time: " << (double) duration.count() / 1000000 << "\n";

    return 0;
}

