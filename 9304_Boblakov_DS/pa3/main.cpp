#include <iostream>
#include <thread>
#include <vector>
#include "utils.h"

typedef std::vector<std::vector<int>> Matrix;

const int m_width = 1000;
const int m_height = 1000;

int count = 4;

int consumer_threads = 4;
int producer_threads = 4;

int writer_threads = 1;


Matrix sum(Matrix m1, Matrix m2) {

    Matrix res_m(m1.size(), std::vector<int>(m1[0].size()));

    for (int i = 0; i < m1.size(); i++) {
        for (int j = 0; j < m1[i].size(); j++) {
            res_m[i][j] = m1[i][j] + m2[i][j];
        }
    }

    return res_m;
}

void generateM(CustomStack &matrixStorage, int count) {
    for (int i = 0; i < count; i++) {
        matrixStorage.push(generateMatrix(m_width, m_height));
        matrixStorage.push(generateMatrix(m_width, m_height));
    }
}

void sumM(CustomStack &matrixStorage, CustomStack &resultStorage, int count) {
    for (int i = 0; i < count; i++) {
        Matrix matrix1 = matrixStorage.pop();
        Matrix matrix2 = matrixStorage.pop();
        resultStorage.push(sum(matrix1, matrix2));
    }
}

void resM(CustomStack &resultStorage, int count) {
    for (int i = 0; i < count; i++) {
        Matrix matrix = resultStorage.pop();
        printMatrix(matrix);
    }
}

int main() {
    CustomStack matrixStorage;
    CustomStack resultStorage;

    auto start = std::chrono::steady_clock::now();

    std::vector<std::thread> consumers;
    std::vector<std::thread> producers;
    std::vector<std::thread> writers;

    for (int i = 0; i < consumer_threads; ++i) {
        std::thread consumer{sumM, std::ref(matrixStorage), std::ref(resultStorage), count};
        consumers.push_back(std::move(consumer));
    }

    for (int i = 0; i < producer_threads; ++i) {
        std::thread producer{generateM, std::ref(matrixStorage), count};
        producers.push_back(std::move(producer));
    }


    for (int i = 0; i < writer_threads; ++i) {
        std::thread writer{resM, std::ref(resultStorage), count};
        producers.push_back(std::move(writer));
    }
    for (auto &thread: consumers) {
        thread.join();
    }

    for (auto &thread: producers) {
        thread.join();
    }
    for (auto &thread: writers) {
        thread.join();
    }


    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time: " << (double) duration.count() / 1000000 << "\n";

    return 0;
}

