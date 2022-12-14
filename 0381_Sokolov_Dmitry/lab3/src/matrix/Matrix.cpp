#include "Matrix.h"

Matrix::Matrix(int _n, int _m): rows(_n), columns(_m) {
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

bool Matrix::check_dimensions(const Matrix& other) const
{
    if (this->get_rows() != other.get_rows() or this->get_columns() != other.get_columns()) {
        return false;
    }

    return true;
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

void Matrix::set_id(size_t value) {
    this->id = value;
}

size_t Matrix::get_id() const {
    return this->id;
}
