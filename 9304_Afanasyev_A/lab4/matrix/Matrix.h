#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <thread>
#include <future>
#include <complex>

#include "../util/util.h"

class Matrix {
    std::vector<std::vector<int>> matrix;
    int width = 0;
    int height = 0;

    static const int THREADS_COUNT = 4;
    static const int STRASSEN_MIN_MATRIX_SIZE = 128;

    static void sumPart(const Matrix &left, const Matrix &right, int start, int length, Matrix &result);

    static void multiplyPart(int start, int end, const Matrix &left, const Matrix &right, Matrix &resultMatrix);

    static Matrix strassenMultiplyInnerParallel(const Matrix &l, const Matrix &r, int maxDepth, int currentDepth);

    static Matrix strassenMultiplyInner(const Matrix &l, const Matrix &r, int maxDepth, int currentDepth);

    static Matrix merge2x2(const Matrix &c11, const Matrix &c12, const Matrix &c21, const Matrix &c22);

public:

    Matrix();
    Matrix getBlock(int y, int x) const;

    Matrix(int height, int width);

    void random();

    Matrix(const Matrix &matrix) = default;

    ~Matrix() = default;

    Matrix(int height, int width, std::vector<std::vector<int>> matrix);

    Matrix parallelMultiply(const Matrix &right, int threadsCount) const;

    Matrix strassenMultiply(const Matrix &right, int maxDepth, int currentDepth = 0) const;

    Matrix operator+(const Matrix &right) const;

    Matrix operator-(const Matrix &right) const;

    Matrix parallelSum(const Matrix &right, int threadsCount) const;

    friend std::istream &operator>>(std::istream &in, Matrix &matrix);

    friend std::ostream &operator<<(std::ostream &out, const Matrix &matrix);
};
