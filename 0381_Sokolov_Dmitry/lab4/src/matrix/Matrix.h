#ifndef PA_LAB2_MATRIX_H
#define PA_LAB2_MATRIX_H

#include <iostream>
#include <vector>
#include <utility>
#include <stdexcept>

#include "../config/config.h"
#include "../logging/Logger.h"

class Matrix {
    int rows = Config::R;
    int columns = Config::C;

    std::vector<std::vector<int>> matrix =
            std::vector<std::vector<int>>(rows, std::vector<int>(columns, 0));

    // Used to identify matrices and result of arithmetic operation on them.
    size_t id = 1;

public:

    Matrix() = default;

    explicit Matrix(int size);

    Matrix(int _rows, int _columns);

    Matrix(const Matrix& other) = default;

    explicit Matrix(std::vector<std::vector<int>> values);

    //------------------------------------------------------------------------------------------------------------------
    // Getter-setter methods

    int get_rows() const;

    int get_columns() const;

    int get(int row, int column) const;

    void set_id(size_t value);

    size_t get_id() const;

    int get_size() const;

    //------------------------------------------------------------------------------------------------------------------
    // Dimension checks

    bool check_dimensions(const Matrix& other, bool equality=true) const noexcept;

    bool check_sum(int _rows, int _columns) const noexcept;

    bool check_mult(int _rows) const noexcept;

    bool is_square();

    //------------------------------------------------------------------------------------------------------------------
    // Operator overloads

    Matrix& operator=(Matrix& other) = default;

    Matrix& operator=(Matrix&& other) = default;

    Matrix operator+(const Matrix& other) const;

    Matrix operator*(const Matrix& other) const;

    Matrix operator-(const Matrix& other) const;

    std::vector<int>& operator[](int row);

    std::vector<int> operator[](int row) const;

    friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);

    friend std::istream& operator>>(std::istream& in, Matrix& matrix);

    friend bool operator==(const Matrix& that, const Matrix& other);

    //------------------------------------------------------------------------------------------------------------------
    // Parallel sum and subtract functionality

    static void partial_sum(const Matrix &first, const Matrix &second, int start, int length, Matrix& result);

    static void partial_sub(const Matrix& first, const Matrix& second, int start, int length, Matrix& result);

    //------------------------------------------------------------------------------------------------------------------
    // Parallel multiplication functionality

    void resize(int _rows, int _columns);

    Matrix get_tile(int row_start, int column_start, int size) const;

    void set_tile(const Matrix& tile, int row_start, int column_start, int size);

    std::vector<int> const& get_row(int _row) const;

    std::vector<int> get_column(int _column) const;

    static void partial_mult(const Matrix &first, const Matrix &second, int _row, int _column, Matrix& result);

    //------------------------------------------------------------------------------------------------------------------

    ~Matrix() = default;

};


#endif //PA_LAB2_MATRIX_H
