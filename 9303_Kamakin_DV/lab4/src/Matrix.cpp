#include "../headers/Matrix.h"

int Matrix::get_width() const {
    return width;
}

int Matrix::get_height() const {
    return height;
}

bool Matrix::is_changed() const {
    return changed.load();
}

bool Matrix::is_not_changed() const {
    return !is_changed();
}

int Matrix::get(int height, int width) const {
    return matrix[height][width];
}

void Matrix::set(int height, int width, int value) {
    matrix[height][width] = value;
}

void Matrix::set(int index, const std::vector<int> &row) {
    matrix[index] = row;
}

void Matrix::mark_changed() {
    changed = true;
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
