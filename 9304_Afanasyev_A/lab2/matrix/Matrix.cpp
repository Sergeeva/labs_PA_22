#include <fstream>
#include "Matrix.h"

Matrix::Matrix() : Matrix(100, 20) {
    random();
}

void Matrix::random() {
    std::srand(std::time(nullptr));
    for (int i = 0; i < height * width; i++) {
        matrix[i / width][i % width] = (std::rand() % 100);
    }
}

Matrix::Matrix(int height, int width)
        : matrix(std::vector<std::vector<int>>(height, std::vector<int>(width))), width(width), height(height) {}


Matrix Matrix::operator+(const Matrix &secondMatrix) const {
    auto newMatrix = Matrix(*this);

    for (int i = 0; i < newMatrix.height; ++i) {
        for (int j = 0; j < newMatrix.width; ++j) {
            newMatrix.matrix[i][j] += secondMatrix.matrix[i][j];
        }
    }

    return newMatrix;
}

Matrix Matrix::parallelSum(const Matrix &secondMatrix, const int threadsCount) const {
    int size = secondMatrix.width * secondMatrix.height;
    if (threadsCount <= 0 || threadsCount >= size) {
        throw std::runtime_error("Invalid threads count");
    }

    int elementsPerThread = (size / threadsCount) + (size % threadsCount > 0 ? 1 : 0);
    auto result = Matrix(height, width);
    std::vector<std::thread> threads;
    for (int i = 0; i < size; i += elementsPerThread) {
        std::thread thread(sumPart, std::ref(*this), std::ref(secondMatrix), i, elementsPerThread, std::ref(result));
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
