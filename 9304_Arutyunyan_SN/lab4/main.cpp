#include <iostream>

#include "common/matrix.hpp"
#include "common/matrix_generation.hpp"
#include "algorithms/parallel.hpp"

constexpr std::size_t N = 256;
constexpr std::size_t M = 256;

int main() {
    auto [left, right] = GenerateMatrixPair<int>(N, M);

    {
        auto start = std::chrono::steady_clock::now();
        auto result = ParallelMultiply<int>(*left, *right);
        auto end = std::chrono::steady_clock::now();
        std::cout << "Parallel multiplying time: " <<
                    std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
    }

    {
        auto start = std::chrono::steady_clock::now();
        auto result = StrassenMultiply<int>(*left, *right, 0);
        auto end = std::chrono::steady_clock::now();
        std::cout << "Strassen multiplying time: " <<
                    std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << std::endl;
    }

    return 0;
}