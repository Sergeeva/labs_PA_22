#include "threads.h"

Threads::Threads(int matrixAmount, char *fileName) : matrixAmount(matrixAmount) {
    outputFileName = fileName;
    inputBuffer = std::make_unique<Buffer<std::pair<Matrix, Matrix>>>();
    outputBuffer = std::make_unique<Buffer<Matrix>>();
}

void Threads::run() {
    std::thread *generationThreads[matrixAmount];
    std::thread *calculationThreads[matrixAmount];
    for (int i = 0; i < matrixAmount; i++) {
        generationThreads[i] = new std::thread(&Threads::generation, this);
        calculationThreads[i] = new std::thread(&Threads::calculation, this);
    }
    std::thread writingThread(&Threads::writing, this);

    for (int i = 0; i < matrixAmount; i++) {
        generationThreads[i]->join();
        delete generationThreads[i];
        calculationThreads[i]->join();
        delete calculationThreads[i];
    }
    writingThread.join();
}

void Threads::generation() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> valueDistribution(100, 499);
    std::uniform_int_distribution<> sizesDistribution(2, 20);

    Matrix matrix1;
    Matrix matrix2;
    int height = sizesDistribution(gen);
    int width = sizesDistribution(gen);
    matrix1.resize(height);
    matrix2.resize(height);
    for (int y = 0; y < height; y++) {
        matrix1[y].resize(width);
        matrix2[y].resize(width);
        for (int x = 0; x < width; x++) {
            matrix1[y][x] = valueDistribution(gen);
            matrix2[y][x] = valueDistribution(gen);
        }
    }
    inputBuffer->push(std::make_pair(matrix1, matrix2));
    matrix1.clear();
    matrix2.clear();
}

void Threads::calculation() {
    Matrix outputMatrix;
    auto inputMatrices = inputBuffer->pop();
    outputMatrix.resize(inputMatrices.first.size());
    for (int y = 0; y < outputMatrix.size(); y++) {
        outputMatrix[y].resize(inputMatrices.first[y].size());
        for (int x = 0; x < outputMatrix[y].size(); x++) {
            outputMatrix[y][x] = inputMatrices.first[y][x] + inputMatrices.second[y][x];
        }
    }
    outputBuffer->push(outputMatrix);
    outputMatrix.clear();
}

void Threads::writing() {
    Matrix matrix;

    std::ofstream file(outputFileName);
    if (!file.is_open()) {
        throw std::runtime_error("Writing error: invalid file");
    }

    for (int i = 0; i < matrixAmount; i++) {
        matrix = outputBuffer->pop();
        for (auto &line: matrix) {
            for (auto &element: line) {
                file << element << ' ';
            }
            file << '\n';
        }
        file << '\n';
    }

    file.close();
}
