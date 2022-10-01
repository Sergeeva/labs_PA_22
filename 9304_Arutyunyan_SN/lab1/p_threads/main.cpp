#include <iostream>
#include <future>
#include <chrono>
#include <atomic>

#include "../common/matrix.hpp"
#include "../common/matrix_generation.hpp"
#include "../common/matrix_write.hpp"

constexpr std::size_t N = 15000;
constexpr std::size_t M = 15000;
constexpr std::size_t THREADS = 1;
std::atomic<long> common_time{0};

// 5763 2339 586 19 (4)
// 3665 1487 348 22 (1)

void MatrixAddition(
        const Matrix& matrix1, const Matrix& matrix2, Matrix& result,
        std::size_t row_start, std::size_t row_end,
        std::size_t column_start, std::size_t column_end) {
    std::cout << "start: " << std::this_thread::get_id() << std::endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (std::size_t i = row_start; i <= row_end; ++i) {
        for (std::size_t j = column_start; j <= column_end; ++j) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    common_time += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
}
 
Matrix MatrixAdditionHelper(const Matrix& matrix1, const Matrix& matrix2) {
    std::size_t row_start = 0, row_middle = N / 2, row_end = N - 1;
    std::size_t column_start = 0, column_middle = M / 2, column_end = M - 1;

    Matrix result(N, Row(M));
    std::array<std::thread, THREADS - 1> threads = {
        // std::thread{MatrixAddition, std::ref(matrix1), std::ref(matrix2), std::ref(result),
        //                                   row_start, row_middle, column_start, column_middle},
        // std::thread{MatrixAddition, std::ref(matrix1), std::ref(matrix2), std::ref(result),
        //                                   row_middle + 1, row_end, column_start, column_middle},
        // std::thread{MatrixAddition, std::ref(matrix1), std::ref(matrix2), std::ref(result),
        //                                   row_start, row_middle, column_middle + 1, column_end},
    };

    // MatrixAddition(matrix1, matrix2, result, row_middle + 1, row_end, column_middle + 1, column_end);
    MatrixAddition(matrix1, matrix2, result, row_start, row_end, column_start, column_end);

    for (auto& thread : threads) thread.join();

    return result;
}

int main() {
    auto generating_future = std::async(std::launch::async, GenerateMatrixPair, N, M);
    auto [matrix1, matrix2] = generating_future.get();

    auto result_matrix = MatrixAdditionHelper(matrix1, matrix2);

    // std::ofstream file("result.txt");
    // auto write_future = std::async(std::launch::async, MatrixWrite<N, M>, std::ref(result_matrix), std::ref(file));
    // write_future.get();

    std::cout << "4 threads computing took " << common_time.load() << " ms" << std::endl;

    return 0;
}
