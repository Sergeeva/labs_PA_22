#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>

class Matrix {
	size_t m;
	size_t n;

	std::vector<std::vector<int>> matrix;

public:

	Matrix() = default;

	Matrix(const Matrix& matrix) = default;

	Matrix(std::vector<std::vector<int>> values);

	size_t get_rows() const;

	size_t get_columns() const;

	int get(int row, int column) const;

	bool check_dimensions(const Matrix& other) const;

	Matrix operator+(const Matrix& other) const;

	std::vector<int>& operator[](int row);

	friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);

	friend std::istream& operator>>(std::istream& in, Matrix& matrix);

	~Matrix() = default;

};