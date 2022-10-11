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

    void random();

    static void sumPart(const Matrix &first, const Matrix &second, int start, int length, Matrix &result);

    Matrix(int height, int width);

public:
    Matrix();

    Matrix(const Matrix &matrix) = default;

    ~Matrix() = default;

    Matrix operator+(const Matrix &secondMatrix) const;

    Matrix parallelSum(const Matrix &secondMatrix, int threadsCount) const;

    friend std::istream &operator>>(std::istream &in, Matrix &matrix);

    friend std::ostream &operator<<(std::ostream &out, const Matrix &matrix);
};
