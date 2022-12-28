#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <thread>
#include <future>
#include <complex>

class Matrix {

    std::vector<std::vector<int>> matrix;
    int width = 0;
    int height = 0;

    static const int THREADS_COUNT = 2;

    static void sumPart(const Matrix& left, const Matrix& right, int start, int length, Matrix& result);

    static void multPart(int start, int end, const Matrix& left, const Matrix& right, Matrix& resultMatrix);
    static Matrix strassenMultiply_rec(const Matrix& l, const Matrix& r, int max_depth, int curr_depth);
    static Matrix strassenMultiply_rec_parallel(const Matrix& l, const Matrix& r, int max_depth, int curr_depth);

    static Matrix together(const Matrix& c11, const Matrix& c12, const Matrix& c21, const Matrix& c22);

public:

    Matrix();

    Matrix getBlock(int y, int x) const;

    Matrix(int height, int width);

    void random();

    Matrix(const Matrix& matrix) = default;

    ~Matrix() = default;

    Matrix(int height, int width, std::vector<std::vector<int>> matrix);

    Matrix operator+(const Matrix& right) const;

    Matrix operator-(const Matrix& right) const;

    friend std::istream& operator>>(std::istream& in, Matrix& matrix);

    friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);

    Matrix parallelSum(const Matrix& right, int threadsCount) const;

    Matrix parallelMultiply(const Matrix& right, int threadsCount) const;

    Matrix strassenMultiply(const Matrix& right, int max_depth, int curr_depth = 0) const;

};