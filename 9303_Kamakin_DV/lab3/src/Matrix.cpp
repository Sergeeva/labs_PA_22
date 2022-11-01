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

void Matrix::set_width(int width) {
    Matrix::width = width;

    for (auto y = 0; y < height; y++) {
        matrix[y].resize(width);
    }
}

void Matrix::set_height(int height) {
    Matrix::height = height;

    matrix.resize(height);
}
