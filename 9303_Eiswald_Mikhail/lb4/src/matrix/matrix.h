#ifndef MATRIX_H
#define MATRIX_H

#include <time.h>
#include <cstdlib>
#include <cmath>

#include <limits>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

class Matrix {
private:
    vector<vector<int>> data;
    int rows_num, cols_num;
    int chars_for_element;
public:
    Matrix(int rows_num, int cols_num);
    Matrix(int rows_num, int cols_num, int max_module);
    Matrix(int* buf);
    int get_rows_num() const;
    int get_cols_num() const;
    int get(int row_index, int col_index) const;
    void set(int row_index, int col_index, int value);
    int* serialize() const;
    Matrix slice(int from_row, int to_row, int from_column, int to_column) const;
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    friend ostream& operator<<(ostream& out, const Matrix& self);
};

#endif // MATRIX_H
