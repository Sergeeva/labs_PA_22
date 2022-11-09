#ifndef LABS_PA_22_LAB3_MATRIX_H
#define LABS_PA_22_LAB3_MATRIX_H

#include <iostream>
#include <vector>
#include <utility>
#include <thread>
#include <future>
#include <numeric>

class Matrix {
private:
    std::vector<std::vector<int>> matrix_;
    size_t width_{};
    size_t height_{};

    static void sum_part_of_vector(
            Matrix& summed_matrix,
            const Matrix& first,
            const Matrix& second,
            int begin_index,
            int length
    );

    void assert_correct_threads_count(size_t threads_count) const;

public:
    Matrix() = default;

    Matrix(const Matrix& matrix) = default;

    Matrix(size_t height, size_t width);

    ~Matrix() = default;

    size_t get_width() const;

    size_t get_height() const;

    size_t get_dimension() const;

    int get(size_t height, size_t width) const;

    int set(size_t height, size_t width, int value);

    Matrix operator+(const Matrix& matrix) const;

    Matrix parallel_sum(const Matrix& matrix, size_t threads_count) const;

    friend std::istream& operator>>(std::istream& in, Matrix& matrix);

    friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);
};

#endif //LABS_PA_22_LAB3_MATRIX_H
