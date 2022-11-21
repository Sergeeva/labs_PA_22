#include <complex>
#include "task.h"

int main() {
    int size = 8;
    std::cout << "Enter matrix size\n";
    std::cin >> size;

    Matrix left(size, size);
    left.random();
    Matrix right(size, size);
    right.random();

    int strassenDepth = 1;
    std::cout << "Enter strassen depth\n";
    std::cin >> strassenDepth;

    auto start = std::chrono::steady_clock::now();
    auto result = left.strassenMultiply(right, strassenDepth);
    auto end = std::chrono::steady_clock::now();
    log("Strassen duration: " +
        std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()));

    std::ofstream outfile1("output1.txt");
    outfile1 << result;

    int threadCount = 1;
    std::cout << "Enter thread count\n";
    std::cin >> threadCount;

    start = std::chrono::steady_clock::now();
    auto result2 = left.parallelMultiply(right, threadCount);
    end = std::chrono::steady_clock::now();
    log("Parallel duration: " +
        std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()));

    std::ofstream outfile2("output2.txt");
    outfile2 << result2;

    return 0;
}
