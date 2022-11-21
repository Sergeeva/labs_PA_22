#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>

#include "../configuration/config.h"

class Matrix {
    int rows = Config::R;
	int columns = Config::C;

	std::vector<std::vector<int>> matrix;

public:

	Matrix() = default;

    Matrix(int _n, int _m);

	Matrix(const Matrix& other);

	explicit Matrix(std::vector<std::vector<int>> values);

	int get_rows() const;

	int get_columns() const;

	int get(int row, int column) const;

	bool check_dimensions(const Matrix& other) const;

	Matrix operator+(const Matrix& other) const;

    static void partial_sum(const Matrix &first, const Matrix &second, int start, int length, Matrix& result);

	std::vector<int>& operator[](int row);

	friend std::ostream& operator<<(std::ostream& out, const Matrix& matrix);

	friend std::istream& operator>>(std::istream& in, Matrix& matrix);

	~Matrix() = default;

};