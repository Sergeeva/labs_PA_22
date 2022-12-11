#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <thread>
#include <iomanip>
#include <iostream>
#include <cmath>
#include <algorithm>


class Matrix{
private:
    int rows_;
    int cols_;
    int real_cols_;
    std::vector<int> matrix_;
public:
    Matrix(int rows, int cols);
    void generate();
    int get_rows();
    void set_rows(int rows);
    int get_cols();
    void set_cols(int cols);
    void set_real_cols(int cols);
    int get_element(int index);
    void set_element(int index, int element);
    std::vector<Matrix> get_parts();
    void mult(Matrix& first_matrix, Matrix& second_matrix, int threads_count);
    void concat_parts(std::vector<Matrix> &parts);
    friend std::ostream& operator<<(std::ostream& stream, const Matrix& matrix);
};

Matrix strassen(Matrix &first_matrix, Matrix &second_matrix, int depth, int thread_count);

#endif