#include <fstream>
#include "Matrix.h"

Matrix::Matrix(const int height, const int width)
        : matrix(std::vector<std::vector<int>>(height, std::vector<int>(width))), width(width), height(height) {}

int *Matrix::writeByPtr(int *ptr) const {
    *ptr = height;
    ++ptr;

    *ptr = width;
    ++ptr;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            *ptr = matrix[i][j];
            ++ptr;
        }
    }

    return ptr;
}

Matrix Matrix::operator+(const Matrix &matrix) const {
    auto newMatrix = Matrix(*this);

    for (int i = 0; i < newMatrix.height; ++i) {
        for (int j = 0; j < newMatrix.width; ++j) {
            newMatrix.matrix[i][j] += matrix.matrix[i][j];
        }
    }

    return newMatrix;
}

Matrix Matrix::parallelSum(const Matrix &matrix, const int threadsCount) const {
    int size = matrix.width * matrix.height;
    if (threadsCount <= 0 || threadsCount >= size) {
        throw std::runtime_error("Invalid threads count");
    }

    int elementsPerThread = (size / threadsCount) + (size % threadsCount > 0 ? 1 : 0);
    auto result = Matrix(height, width);
    std::vector<std::thread> threads;
    for (int i = 0; i < size; i += elementsPerThread) {
        std::thread thread(sumPart, std::ref(*this), std::ref(matrix), i, elementsPerThread, std::ref(result));
        threads.emplace_back(std::move(thread));
    }

    for (auto &thread: threads) {
        thread.join();
    }

    return result;
}

void Matrix::sumPart(const Matrix &first, const Matrix &second, int start, int length, Matrix &result) {
    int size = result.width * result.height;
    for (int i = start; i < start + length && i < size; ++i) {
        int h = i / result.width;
        int w = i % result.width;
        result.matrix[h][w] = first.matrix[h][w] + second.matrix[h][w];
    }
}

std::pair<Matrix, int *> Matrix::readByPtr(int *ptr) {
    int w = *ptr;
    ptr++;

    int h = *ptr;
    ptr++;

    auto result = Matrix(w, h);
    for (int i = 0; i < result.height; i++) {
        for (int j = 0; j < result.width; j++) {
            result.matrix[i][j] = *ptr;
            ++ptr;
        }
    }

    return std::make_pair(result, ptr);
}

std::istream &operator>>(std::istream &in, Matrix &matrix) {
    in >> matrix.height;
    in >> matrix.width;

    matrix.matrix.resize(matrix.height);

    for (int i = 0; i < matrix.height; ++i) {
        matrix.matrix[i].resize(matrix.width);
        for (int j = 0; j < matrix.width; ++j) {
            in >> matrix.matrix[i][j];
        }
    }

    return in;
}

std::ostream &operator<<(std::ostream &out, const Matrix &matrix) {
    out << matrix.height << ' ';
    out << matrix.width << '\n';

    for (int i = 0; i < matrix.height; ++i) {
        for (int j = 0; j < matrix.width; ++j) {
            out << matrix.matrix[i][j];
            if (j < matrix.width - 1) {
                out << ' ';
            }
        }
        out << '\n';
    }

    return out;
}

void Matrix::writeMatrix(const Matrix &matrix) {
    std::ofstream outputFile(Filenames::outputFilename);
    outputFile << matrix;
    log("Matrix written to file: " + std::string(Filenames::outputFilename));
    outputFile.close();
}

void Matrix::readTwoMatrices(Matrix &first, Matrix &second) {
    std::ifstream matrixInputFile(Filenames::inputFilename);

    matrixInputFile >> first;
    matrixInputFile >> second;
    log("Matrices loaded from: " + std::string(Filenames::inputFilename));

    matrixInputFile.close();
}
