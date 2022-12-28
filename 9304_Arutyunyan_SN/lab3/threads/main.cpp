#include <thread>
#include <chrono>

#include "../common/matrix.hpp"
#include "../common/matrix_generation.hpp"
#include "../common/matrix_write.hpp"
#include "../common/lock_free_stack.hpp"

constexpr std::size_t N = 500;
constexpr std::size_t M = 500;
constexpr std::size_t total_iterations = 100;

lock_free_stack<MatrixPtrPair> addition_stack;
lock_free_stack<MatrixPtr> output_stack;

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
        addition_stack.push(std::move(matrixes));
    }
}

void ConsumerThread() {
    std::size_t iteration = 0;
    while (iteration++ < total_iterations) {
        auto [matrix1, matrix2] = addition_stack.pop();

        auto result = MatrixAddition(matrix1, matrix2);
        output_stack.push(std::move(result));
    }
}

void WriterThread() {
    std::ofstream file("result.txt");

    std::size_t iteration = 0;
    while (iteration++ < total_iterations) {
        auto summed_matrix = output_stack.pop();

        MatrixWrite(summed_matrix, file);
    }
}

int main() {
    std::array<std::thread, 9> threads = {
        std::thread{ProducerThread},
        std::thread{ProducerThread},
        std::thread{ProducerThread},
        std::thread{ConsumerThread},
        std::thread{ConsumerThread},
        std::thread{ConsumerThread},
        std::thread{WriterThread},
        std::thread{WriterThread},
        std::thread{WriterThread}
    };

    for (auto& thread : threads) thread.join();

    return 0;
}
