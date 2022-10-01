#include <future>

#include "../common/matrix.hpp"
#include "../common/matrix_generation.hpp"
#include "../common/matrix_write.hpp"

constexpr std::size_t N = 5;
constexpr std::size_t M = 5;

template <std::size_t N, std::size_t M>
Matrix<N, M> MatrixAddition(const Matrix<N, M>& matrix1, const Matrix<N, M>& matrix2) {
    Matrix<N, M> result;

    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < M; ++j) {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }

    return result;
}

int main() {
    auto generating_future = std::async(std::launch::async, GenerateMatrixPair<N, M>);
    auto [matrix1, matrix2] = generating_future.get();

    auto addition_future = std::async(std::launch::async, MatrixAddition<N, M>, std::ref(matrix1), std::ref(matrix2));
    auto result_matrix = addition_future.get();

    std::ofstream file("result.txt");
    auto write_future = std::async(std::launch::async, MatrixWrite<N, M>, std::ref(result_matrix), std::ref(file));
    write_future.get();

    return 0;
}