#include "Matrix.h"

Matrix::Matrix(int size): rows(size), columns(size) {
    this->matrix = std::vector<std::vector<int>>(rows, std::vector<int>(columns, 0));
}

Matrix::Matrix(int _rows, int _columns): rows(_rows), columns(_columns) {
    this->matrix = std::vector<std::vector<int>>(rows, std::vector<int>(columns, 0));
}

Matrix::Matrix(std::vector<std::vector<int>> values) {
    this->rows = values.size();
    this->columns = values[0].size();

    this->matrix = values;
}

int Matrix::get_rows() const {
    return this->rows;
}

int Matrix::get_columns() const {
    return this->columns;
}

int Matrix::get(int row, int column) const
{
    return this->matrix[row][column];
}

std::vector<int> Matrix::get_row(int _row) const {
    return this->matrix[_row];
}

std::vector<int> Matrix::get_column(int _column) const {
    std::vector<int> col = std::vector<int>();
    for (unsigned int i = 0; i < this->rows; i++) {
        col.emplace_back(this->matrix[i][_column]);
    }

    return col;
}

int Matrix::get_size() const {
    return this->rows * this->columns;
}

bool Matrix::check_dimensions(const Matrix& other, bool equality) const noexcept
{

    if (equality) {
        return this->check_sum(other.get_rows(), other.get_columns());
    }

    return this->check_mult(other.get_rows());
}

bool Matrix::is_square() {
    if (this->rows == this->columns) {
        return true;
    }

    return false;
}

bool Matrix::check_sum(int _rows, int _columns) const noexcept {
    if (this->rows != _rows or this->columns != _columns) {
        return false;
    }

    return true;
}

bool Matrix::check_mult(int _rows) const noexcept {
    if (this->columns != _rows) {
        return false;
    }

    return true;
}

void Matrix::resize(int _rows, int _columns) {

    if (this->columns != _columns) {
        this->columns = _columns;

        for (unsigned int i = 0; i < this->rows; i++) {
            this->matrix[i].resize(_columns, 0);
        }
    }

    if (this->rows != _rows) {
        this->rows = _rows;
        this->matrix.resize(_rows, std::vector<int>(this->columns, 0));
    }
}

void Matrix::get_tile(Matrix& tile, int row_start, int column_start, int size) const {

    for (int i = row_start; i < row_start + size; i++) {
        for (int j = column_start; j < column_start + size; j++) {
            tile[i - row_start][j - column_start] = this->matrix[i][j];
        }
    }
}

void Matrix::set_tile(Matrix &tile, int row_start, int column_start, int size) {
    for (int i = row_start; i < row_start + size; i++) {
        for (int j = column_start; j < column_start + size; j++) {
            this->matrix[i][j] = tile[i - row_start][j - column_start];
        }
    }
}


Matrix Matrix::operator+(const Matrix& other) const {

    if (this->check_dimensions(other)) {

        Matrix result = Matrix(*this);

        for (int i = 0; i < result.get_rows(); i++) {
            for (int j = 0; j < result.get_columns(); j++) {
                result[i][j] += other.get(i, j);
            }
        }

        return result;
    }

    throw std::invalid_argument("matrices dimensions do not match!");
}

Matrix Matrix::operator-(const Matrix &other) const {
    if (this->check_dimensions(other)) {

        Matrix result = Matrix(*this);

        for (int i = 0; i < result.get_rows(); i++) {
            for (int j = 0; j < result.get_columns(); j++) {
                result[i][j] -= other.get(i, j);
            }
        }

        return result;
    }

    throw std::invalid_argument("matrices dimensions do not match!");
}

Matrix Matrix::operator*(const Matrix &other) const {
    if (this->check_dimensions(other, false)) {

        Matrix result = Matrix(this->rows, other.get_columns());

        for (int i = 0; i < result.get_rows(); i++) {
            for (int j = 0; j < result.get_columns(); j++) {
                for (int k = 0; k < other.get_rows(); k++) {
                    result[i][j] += this->matrix[i][k] * other.get(k, j);
                }
            }
        }

        return result;
    }

    throw std::invalid_argument("matrices dimensions do not match!" +
                                std::to_string(this->get_rows()) + "x" + std::to_string(this->get_columns()) + ") of first != (" +
                                std::to_string(other.get_rows()) + "x" + std::to_string(other.get_columns()) + ") of second!");
}

std::vector<int>& Matrix::operator[](int row)
{
    return this->matrix[row];
}

std::ostream& operator<<(std::ostream& out, const Matrix& matrix)
{
    out << matrix.rows << std::endl;
    out << matrix.columns << std::endl;

    for (int i = 0; i < matrix.get_rows(); i++) {

        for (int j = 0; j < matrix.get_columns(); j++) {
            out << matrix.matrix[i][j] << ' ';
        }

        out << '\n';

    }

    return out;
}

std::istream& operator>>(std::istream& in, Matrix& matrix)
{
    in >> matrix.rows;
    in >> matrix.columns;

    int rows = matrix.get_rows();
    int columns = matrix.get_columns();

    matrix.matrix.resize(rows);

    for (int i = 0; i < rows; i++) {
        matrix.matrix[i].resize(columns);
        for (int j = 0; j < columns; j++) {
            in >> matrix.matrix[i][j];
        }

    }

    return in;
}

void Matrix::partial_sum(const Matrix &first, const Matrix &second, int start, int length, Matrix& result) {
    int size = result.get_rows() * result.get_columns();

    for (int i = start; i < start + length && i < size; ++i) {
        int y = i / result.get_columns();
        int x = i % result.get_columns();

        result[y][x] = first.get(y, x) + second.get(y, x);
    }
}

void Matrix::partial_sub(const Matrix &first, const Matrix &second, int start, int length, Matrix &result) {
    int size = result.get_rows() * result.get_columns();

    for (int i = start; i < start + length && i < size; ++i) {
        int y = i / result.get_columns();
        int x = i % result.get_columns();

        result[y][x] = first.get(y, x) - second.get(y, x);
    }
}

void Matrix::partial_mult(const Matrix &first, const Matrix &second,
                          int _row, int _column, Matrix& result) {

    auto f_row = first.get_row(_row);
    auto s_col = second.get_column(_column);

    int element = 0;

    for (unsigned int i = 0; i < f_row.size(); i++) {
        element += f_row[i] * s_col[i];
    }

    result[_row][_column] = element;

}

void Matrix::set_id(size_t value) {
    this->id = value;
}

size_t Matrix::get_id() const {
    return this->id;
}

bool operator==(const Matrix &that, const Matrix &other) {

    if (!that.check_dimensions(other)) {
        return false;
    }

    if (that.matrix != other.matrix) {
        return false;
    }

    return true;
}
