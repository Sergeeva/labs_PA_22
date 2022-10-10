#include "Matrix.h"

Matrix::Matrix(std::vector<std::vector<int>> values) {
	this->m = values.size();
	this->n = values[0].size();

	this->matrix = values;
}

size_t Matrix::get_rows() const {
	return this->m;
}

size_t Matrix::get_columns() const {
	return this->n;
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

		for (int i = 0; i < this->get_rows(); i++) {
			for (int j = 0; j < this->get_columns(); j++) {
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
	out << "[\n";

	for (int i = 0; i < matrix.get_rows(); i++) {
		out << "[ ";

		for (int j = 0; j < matrix.get_columns(); j++) {
			out << matrix.get(i, j) << " ";
		}

		out << "]\n";

	}

	out << "]\n";

	return out;
}

std::istream& operator>>(std::istream& in, Matrix& matrix)
{
	in >> matrix.m;
	in >> matrix.n;

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
