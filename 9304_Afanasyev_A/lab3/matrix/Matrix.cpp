#include <fstream>
#include "Matrix.h"

Matrix Matrix::randomMatrix(int height, int width, int limitValue) {
    Matrix m(height, width);
    m.random(limitValue);

    return m;
}

void Matrix::random(int limit) {
    std::srand(std::time(nullptr));
    for (int i = 0; i < height * width; i++) {
        matrix[i / width][i % width] = (std::rand() % limit);
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
