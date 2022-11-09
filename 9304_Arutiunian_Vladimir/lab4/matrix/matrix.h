#ifndef LABS_PA_22_LAB4_MATRIX_H
#define LABS_PA_22_LAB4_MATRIX_H

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
    static const size_t THREADS_COUNT = 4;
    static const size_t MAX_DEPTH = 5;
    static const size_t STRASSEN_MIN_MATRIX_SIZE = 128;

    void assert_correct_threads_count(size_t threads_count) const;

    void assert_correct_matrices_sizes_for_multiplying(const Matrix& matrix) const;

    static void multiply_part(
            size_t begin_row,
            size_t length,
            const Matrix& first_matrix,
            const Matrix& second_matrix,
            Matrix& result_matrix
    );

    void assert_correct_matrices_sizes_for_strassen_multiplying(const Matrix& matrix) const;

    static bool is_power_of_two(size_t number);

    static Matrix strassen_multiply_inner(const Matrix& first_matrix, const Matrix& second_matrix, size_t max_depth);

    std::vector<std::vector<Matrix>> split_to_2x2() const;

    Matrix copy_part(size_t first_row, size_t after_last_row, size_t first_column, size_t after_last_column) const;

    static Matrix merge_from_2x2(const Matrix& c11, const Matrix& c12, const Matrix& c21, const Matrix& c22);

public:
    Matrix() = default;

    Matrix(const Matrix& matrix) = default;

    Matrix(size_t height, size_t width);

    Matrix(size_t height, size_t width, std::vector<std::vector<int>> matrix);

    ~Matrix() = default;

    size_t get_width() const;

    size_t get_height() const;

    int get(size_t height, size_t width) const;

    int set(size_t height, size_t width, int value);

    Matrix operator+(const Matrix& matrix) const;

    Matrix operator-(const Matrix& matrix) const;

    bool operator==(const Matrix& matrix) const;

    Matrix multiply(const Matrix& matrix, size_t threads_count = 1) const;

    Matrix strassen_multiply(const Matrix& matrix, size_t max_depth = 0) const;

    friend std::istream& operator>>(std::istream& in, Matrix& matrix);

    friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);

};

#endif //LABS_PA_22_LAB4_MATRIX_H
