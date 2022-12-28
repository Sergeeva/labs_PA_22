#include <complex>
#include <iostream>
#include <thread>
#include <fstream>
#include "Matrix.h"

int main() {
    
    std::cout << "Введите размер матрицы\n";

    int size = 0;
    std::cin >> size;

    Matrix left(size, size);
    left.random();

    Matrix right(size, size);
    right.random();

    int depth_strass = 1;
    std::cout << "Введите глубину алгоритма Штрассена\n";
    std::cin >> depth_strass;

    auto start = std::chrono::steady_clock::now();
    auto result = left.strassenMultiply(right, depth_strass);
    auto end = std::chrono::steady_clock::now();

    std::cout << "Умножение по алогритму Штрассена: " <<std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "ms" << std::endl;

    std::ofstream outfile1("strassen_mult.txt");
    outfile1 << result;

    int threadCount = 1;
    std::cout << "Введите количество потоков\n";
    std::cin >> threadCount;

    start = std::chrono::steady_clock::now();
    auto result2 = left.parallelMultiply(right, threadCount);
    end = std::chrono::steady_clock::now();

    std::cout << "Параллельное умножение: " <<std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << "ms" << std::endl;

    std::ofstream outfile2("parallel_mult.txt");
    outfile2 << result2;

    return 0;
}