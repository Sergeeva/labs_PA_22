#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <thread>
#include <future>

#include "../util/util.h"

struct Matrix {
    std::vector<std::vector<int>> matrix;
    int width = 0;
    int height = 0;

    Matrix() = default;

    Matrix(const Matrix &matrix) = default;

    Matrix(int height, int width);

    ~Matrix() = default;

    Matrix operator+(const Matrix &matrix) const;

    Matrix parallelSum(const Matrix &matrix, int threadsCount) const;

    static void sumPart(const Matrix &first, const Matrix &second, int start, int length, Matrix &result);

    static std::pair<Matrix, int *> readByPtr(int *ptr);

    int *writeByPtr(int *ptr) const;

    friend std::istream &operator>>(std::istream &in, Matrix &matrix);

    friend std::ostream &operator<<(std::ostream &out, const Matrix &matrix);

    static void writeMatrix(const Matrix &matrix);

    static void readTwoMatrices(Matrix &first, Matrix &second);
};
