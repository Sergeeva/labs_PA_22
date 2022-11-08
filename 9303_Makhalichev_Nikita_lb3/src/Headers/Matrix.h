#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <thread>
#include <iomanip>


class Matrix{
private:
    int rows_;
    int cols_;
    std::vector<int> matrix_;
public:
    Matrix(int rows, int cols);
    void generate();
    int get_rows();
    int get_cols();
    int get_element(int index);
    void set_element(int index, int element);
    void sum(Matrix& first_matrix, Matrix& second_matrix, int threads_count);
    friend std::ostream& operator<<(std::ostream& stream, const Matrix& matrix);
};

#endif