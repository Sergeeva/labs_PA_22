#include <ostream>
#include <spdlog/spdlog.h>
#include "../headers/Matrix.h"

int Matrix::get_width() const {
    return width;
}

int Matrix::get_height() const {
    return height;
}

int Matrix::get(int height, int width) const {
    return matrix[height][width];
}

void Matrix::set(int height, int width, int value) {
    matrix[height][width] = value;
}

Matrix Matrix::operator+(const Matrix &matrix) const {
    auto newMatrix = Matrix(*this);

    for (int y = 0; y < newMatrix.get_height(); y++) {
        for (int x = 0; x < newMatrix.get_width(); x++) {
            newMatrix.matrix[y][x] += matrix.get(y, x);
        }
    }

    return newMatrix;
}
