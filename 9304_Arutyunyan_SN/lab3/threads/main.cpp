#include <thread>
#include <queue>
#include <chrono>

#include "../common/matrix.hpp"
#include "../common/matrix_generation.hpp"
#include "../common/matrix_write.hpp"
#include "../common/lock_free_queue.hpp"

constexpr std::size_t N = 2000;
constexpr std::size_t M = 2000;
constexpr std::size_t total_iterations = 100;

lock_free_queue<MatrixPtrPair> addition_queue;
lock_free_queue<MatrixPtr> output_queue;

MatrixPtr MatrixAddition(const MatrixPtr& matrix1, const MatrixPtr& matrix2) {
    auto result = std::make_shared<Matrix>(N, Row(M));

    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < M; ++j) {
            (*result)[i][j] = (*matrix1)[i][j] + (*matrix2)[i][j];
        }
    }

    return result;
}


void ProducerThread() {
    std::size_t iteration = 0;
    while (iteration++ < total_iterations) {
        auto matrixes = GenerateMatrixPair(N, M);
        addition_queue.push(std::move(matrixes));
    }
}

void ConsumerThread() {
    std::size_t iteration = 0;
    while (iteration++ < total_iterations) {
        auto [matrix1, matrix2] = addition_queue.front();
        addition_queue.pop();

        auto result = MatrixAddition(matrix1, matrix2);
        output_queue.push(std::move(result));
    }
}

void WriterThread() {
    std::ofstream file("result.txt");

    std::size_t iteration = 0;
    while (iteration++ < total_iterations) {
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
