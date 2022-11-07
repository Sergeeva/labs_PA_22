#include "threads.h"

Threads::Threads(int capacity, char *fileName) {
    outputFileName = fileName;
    inputBuffer = new Buffer<std::pair<Matrix, Matrix>>(capacity);
    outputBuffer = new Buffer<Matrix>(capacity);
}

Threads::~Threads() {
    delete inputBuffer;
    delete outputBuffer;
}

void Threads::run() {
    std::thread generationThread(&Threads::generation, this);
    std::thread calculationThread(&Threads::calculation, this);
    std::thread writingThread(&Threads::writing, this);

    generationThread.join();
    calculationThread.join();
    writingThread.join();
}

void Threads::generation() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> valueDistribution(100, 499);
    std::uniform_int_distribution<> sizesDistribution(2, 20);

    Matrix matrix1;
    Matrix matrix2;

    for (int i = 0; i < MATRIX_AMOUNT; i++) {
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
        inputBuffer->produce(std::make_pair(matrix1, matrix2));
        matrix1.clear();
        matrix2.clear();
    }
}

void Threads::calculation() {
    Matrix outputMatrix;
    for (int i = 0; i < MATRIX_AMOUNT; i++) {
        auto inputMatrices = inputBuffer->consume();
        outputMatrix.resize(inputMatrices.first.size());
        for (int y = 0; y < outputMatrix.size(); y++) {
            outputMatrix[y].resize(inputMatrices.first[y].size());
            for (int x = 0; x < outputMatrix[y].size(); x++) {
                outputMatrix[y][x] = inputMatrices.first[y][x] + inputMatrices.second[y][x];
            }
        }
        outputBuffer->produce(outputMatrix);
        outputMatrix.clear();
    }
}

void Threads::writing() {
    std::ofstream file(outputFileName);
    if (!file.is_open()) {
        throw std::runtime_error("Writing error: invalid file");
    }

    Matrix matrix;
    for (int i = 0; i < MATRIX_AMOUNT; i++) {
        matrix = outputBuffer->consume();
        for (int y = 0; y < matrix.size(); y++) {
            for (int x = 0; x < matrix[y].size(); x++) {
                file << matrix[y][x] << ' ';
            }
            file << '\n';
        }
        file << '\n';
    }
}
