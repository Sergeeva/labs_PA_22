#ifndef PA_LAB2_MATRIX_H
#define PA_LAB2_MATRIX_H

#include <iostream>
#include <vector>
#include <stdexcept>

#include "../config/config.h"

class Matrix {
    int rows = Config::R;
    int columns = Config::C;

    std::vector<std::vector<int>> matrix;

    size_t id = 1;

public:

    Matrix() = default;

    Matrix(int _n, int _m);

    Matrix(const Matrix& other) = default;

    explicit Matrix(std::vector<std::vector<int>> values);

    int get_rows() const;

    int get_columns() const;

    int get(int row, int column) const;

    void set_id(size_t value);

    size_t get_id() const;

    bool check_dimensions(const Matrix& other) const;

    Matrix operator+(const Matrix& other) const;

    Matrix& operator=(Matrix& other) = default;

    Matrix& operator=(Matrix&& other) = default;

    static void partial_sum(const Matrix &first, const Matrix &second, int start, int length, Matrix& result);

    std::vector<int>& operator[](int row);

    friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);

    friend std::istream& operator>>(std::istream& in, Matrix& matrix);

    ~Matrix() = default;

};


#endif //PA_LAB2_MATRIX_H
