#ifndef LABS_PA_22_MATRIX_H
#define LABS_PA_22_MATRIX_H

#include <iostream>
#include <vector>
#include <utility>
#include <thread>
#include <future>

class Matrix {
private:
    std::vector<std::vector<int>> matrix_;
    int width_;
    int height_;

public:
    Matrix() = default;

    Matrix(const Matrix& matrix) = default;

    Matrix(int height, int width);

    ~Matrix() = default;

    int getWidth() const;

    int getHeight() const;

    int get(int height, int width) const;

    int set(int height, int width, int value);

    Matrix operator+(const Matrix& matrix) const;

    Matrix parallelSum(const Matrix& matrix, size_t threads_count) const;

    friend std::istream& operator>>(std::istream& in, Matrix& matrix);

    friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);
};

void sumSubVector(Matrix& summed_matrix, const Matrix& first, const Matrix& second, int begin_index, int length);

#endif //LABS_PA_22_MATRIX_H
