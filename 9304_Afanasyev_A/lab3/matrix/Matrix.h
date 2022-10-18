#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <thread>
#include <future>

#include "../util/util.h"

class Matrix {
    std::vector<std::vector<int>> matrix;
    int width = 0;
    int height = 0;

    void random(int limitValue);

    Matrix(int height, int width);

public:
    Matrix(const Matrix &matrix) = default;

    ~Matrix() = default;

    static Matrix randomMatrix(int height, int width, int limitValue);

    int getHeight() { return height; };

    int getWidth() { return width; };

    Matrix operator+(const Matrix &secondMatrix) const;

    friend std::istream &operator>>(std::istream &in, Matrix &matrix);

    friend std::ostream &operator<<(std::ostream &out, const Matrix &matrix);
};
