#include <iostream>
#include <future>
#include <chrono>
#include <atomic>
#include <thread>
#include <queue>

#include "../common/matrix.hpp"
#include "../common/matrix_generation.hpp"
#include "../common/matrix_write.hpp"
#include "../common/semaphore.hpp"

constexpr std::size_t N = 15000;
constexpr std::size_t M = 15000;
constexpr std::size_t THREADS = 1;
std::atomic<long> common_time{0};

std::queue<MatrixPtrPair> addition_queue;
semaphore addition_semaphore;
std::queue<MatrixPtr> output_queue;
semaphore output_semaphore; 

void MatrixAddition(
        const MatrixPtr& matrix1, const MatrixPtr& matrix2, MatrixPtr& result,
        std::size_t row_start, std::size_t row_end,
        std::size_t column_start, std::size_t column_end) {
    std::cout << "start: " << std::this_thread::get_id() << std::endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (std::size_t i = row_start; i <= row_end; ++i) {
        for (std::size_t j = column_start; j <= column_end; ++j) {
            (*result)[i][j] = (*matrix1)[i][j] + (*matrix2)[i][j];
        }
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    common_time += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}
 
MatrixPtr MatrixAdditionHelper(const MatrixPtr& matrix1, const MatrixPtr& matrix2) {
    std::size_t row_start = 0, row_middle = N / 2, row_end = N - 1;
    std::size_t column_start = 0, column_middle = M / 2, column_end = M - 1;

    auto result = std::make_shared<Matrix>(N, Row(M));
    std::array<std::thread, THREADS - 1> threads = {
        // std::thread{MatrixAddition, std::ref(matrix1), std::ref(matrix2), std::ref(result),
        //                                   row_start, row_middle, column_start, column_middle},
        // std::thread{MatrixAddition, std::ref(matrix1), std::ref(matrix2), std::ref(result),
        //                                   row_middle + 1, row_end, column_start, column_middle},
        // std::thread{MatrixAddition, std::ref(matrix1), std::ref(matrix2), std::ref(result),
        //                                   row_start, row_middle, column_middle + 1, column_end},
    };

    // MatrixAddition(matrix1, matrix2, result, row_middle + 1, row_end, column_middle + 1, column_end);
    // MatrixAddition(matrix1, matrix2, result, row_start, row_end, column_start, column_end);

    for (auto& thread : threads) thread.join();

    return result;
}

void ProducerThread() {
    while (true) {
        auto matrixes = GenerateMatrixPair(N, M);

        addition_queue.push(std::move(matrixes));
        addition_semaphore.release();
    }
}

void ConsumerThread() {
    while (true) {
        addition_semaphore.acquire();
        auto [matrix1, matrix2] = addition_queue.front();
        addition_queue.pop();

        auto result = MatrixAdditionHelper(matrix1, matrix2);
        output_queue.push(std::move(result));
        output_semaphore.release();
    }
}

void WriterThread() {
    std::ofstream file("result.txt");

    while (true) {
        output_semaphore.acquire();
        auto summed_matrix = output_queue.front();
        output_queue.pop();

        MatrixWrite(summed_matrix, file);
    }
}

int main() {
    std::array<std::thread, 3> threads = {
        std::thread{ProducerThread},
        std::thread{ConsumerThread},
        std::thread{WriterThread},
    };

    for (auto& thread : threads) thread.join();

    return 0;
}
