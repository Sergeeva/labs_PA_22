#include "Matrix.h"
#include <iostream>
#include <fstream>


int main() {
    int matrixSize = 8;
    std::cout << "Enter matrix matrixSize\n";
    std::cin >> matrixSize;

    Matrix firstMatrix(matrixSize, matrixSize);
    firstMatrix.createRandomMatrix();
    Matrix secondMatrix(matrixSize, matrixSize);
    secondMatrix.createRandomMatrix();

    int strassenDepth = 1;
    std::cout << "Enter strassen depth\n";
    std::cin >> strassenDepth;

    auto start = std::chrono::steady_clock::now();
    auto result = firstMatrix.strassenMultiplyMatrices(secondMatrix, strassenDepth);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Strassen duration: " +
        std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) << + "\n";

    std::ofstream outfile1("output1.txt");
    result.printMatrix(outfile1);

    int threadNumber = 1;
    std::cout << "Enter thread count\n";
    std::cin >> threadNumber;

    start = std::chrono::steady_clock::now();
    auto result2 = firstMatrix.parallelSimpleMultiplyMatrices(secondMatrix, threadNumber);
    end = std::chrono::steady_clock::now();
    std::cout << "Simple multiply duration: " +
        std::to_string(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()) + "\n";

    std::ofstream outfile2("output2.txt");
    result2.printMatrix(outfile2);

    return 0;
}