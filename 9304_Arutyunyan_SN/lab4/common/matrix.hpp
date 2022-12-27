#pragma once

#include <vector>
#include <utility>
#include <memory>
#include <cassert>
#include <iostream>


template <typename T>
struct MatrixBlockReference;

template <typename T>
class Matrix {
    std::vector<std::vector<T>> matrix;

public:
    const std::size_t width;
    const std::size_t height;

    Matrix(std::size_t width, std::size_t height) : width(width),
                                                    height(height) {
        matrix.resize(height);
        for (std::size_t i = 0; i < height; ++i) matrix[i].resize(width, T{});
    }

    std::size_t Width() const { return width; }
    std::size_t Height() const { return height; }

    const std::vector<T>& operator[](int index) const {
        return matrix[index];
    }
    std::vector<T>& operator[](int index) {
        return matrix[index];
    }

    Matrix operator-(const Matrix& right) {
        assert(width == right.width);
        assert(height == right.height);

        Matrix<T> result(width, height);
        for (std::size_t i = 0; i < height; ++i) {
            for (std::size_t j = 0; j < width; ++j) {
                result[i][j] = this->matrix[i][j] - right[i][j];
            }
        }

        return result;
    }

    Matrix operator+(const Matrix& right) {
        if (height != right.height) {
            std::cout << "height=" << height << ", right.height=" << right.height << std::endl; 
        }
        assert(width == right.width);
        assert(height == right.height);

        Matrix<T> result(width, height);
        for (std::size_t i = 0; i < height; ++i) {
            for (std::size_t j = 0; j < width; ++j) {
                result[i][j] = this->matrix[i][j] + right[i][j];
            }
        }

        return result;
    }

    MatrixBlockReference<T> GetBlock(int top_x, int top_y,
                                  int bottom_x, int bottom_y) const {
        return MatrixBlockReference<T>(*this, top_x, top_y, bottom_x, bottom_y);
    }

    auto begin() {
        return matrix.begin();
    }
    auto end() {
        return matrix.end();
    }
};

template <typename T>
struct MatrixBlockReference {
    const Matrix<T>& matrix;
    int top_x; int top_y;
    int bottom_x; int bottom_y;

    MatrixBlockReference(const Matrix<T>& matrix,
                         int top_x, int top_y, int bottom_x, int bottom_y)
            : matrix(matrix), top_x(top_x), top_y(top_y),
                              bottom_x(bottom_x), bottom_y(bottom_y) { }

    Matrix<T> operator+(const MatrixBlockReference& other) const {
        // assert(top_x == other.top_x); assert(top_y == other.top_y);
        // assert(bottom_x == other.bottom_x); assert(bottom_y == other.bottom_y);
        assert(std::abs(top_x - bottom_x) == std::abs(other.top_x - other.bottom_x));
        assert(std::abs(top_y - bottom_y) == std::abs(other.top_y - other.bottom_y));

        Matrix<T> result(std::abs(top_x - bottom_x), std::abs(top_y - bottom_y));

        for (int x = top_x; x < bottom_x; ++x) {
            for (int y = top_y; y < bottom_y; ++y) {
                result[y - top_y][x - top_x] = matrix[y][x] + other.matrix[y][x];
            }
        }

        return result;
    }

    Matrix<T> operator-(const MatrixBlockReference& other) const {
        assert(std::abs(top_x - bottom_x) == std::abs(other.top_x - other.bottom_x));
        assert(std::abs(top_y - bottom_y) == std::abs(other.top_y - other.bottom_y));

        Matrix<T> result(std::abs(top_x - top_y), std::abs(bottom_x - bottom_y));

        for (int x = top_x; x < bottom_x; ++x) {
            for (int y = top_y; y < bottom_y; ++y) {
                result[y - top_y][x - top_x] = matrix[y][x] - other.matrix[y][x];
            }
        }

        return result;
    }

    Matrix<T> GetMatrix() const {
        Matrix<T> block(std::abs(top_x - bottom_x), std::abs(top_y - bottom_y));
        for (int x = top_x; x < bottom_x; ++x) {
            for (int y = top_y; y < bottom_y; ++y) {
                block[y - top_y][x - top_x] = matrix[y][x];
            }
        }

        return block;
    }
};

template <typename T>
struct SignedMatrix {
    const Matrix<T>& matrix;
    int sign;
    
    SignedMatrix(const Matrix<T>& matrix, int sign = 1) : matrix(matrix), sign(sign) { }
};


template <typename T, typename... Args>
void SumAllMatrixes(Matrix<T>& result) {
    return;
}

template <typename T, typename... Args>
void SumAllMatrixes(
        Matrix<T>& result, const SignedMatrix<T>& a) {
    for (std::size_t i = 0; i < result.height; ++i) {
        for (std::size_t j = 0; j < result.width; ++j) {
            result[i][j] += a.sign * a.matrix[i][j];
        }
    }

    return;
}

template <typename T, typename... Args>
void SumAllMatrixes(
        Matrix<T>& result, const SignedMatrix<T>& a, const SignedMatrix<T>& b,
        const Args&... matrixes) {
    for (std::size_t i = 0; i < result.height; ++i) {
        for (std::size_t j = 0; j < result.width; ++j) {
            result[i][j] += a.sign * a.matrix[i][j] + b.sign * b.matrix[i][j];
        }
    }

    return SumAllMatrixes(result, matrixes...);
}

template <typename T, typename... Args>
Matrix<T> BaseSumAllMatrixes(int n, const Args&... matrixes) {
    Matrix<T> result(n, n);
    SumAllMatrixes<T>(result, matrixes...);
    return result;
}

template <typename T, typename U = T>
using MatrixPair = std::pair<Matrix<T>, Matrix<U>>;
template <typename T>
using MatrixPtr = std::shared_ptr<Matrix<T>>;
template <typename T, typename U = T>
using MatrixPtrPair = std::pair<MatrixPtr<T>, MatrixPtr<U>>;
