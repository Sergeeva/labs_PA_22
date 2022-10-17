#ifndef LAB2_MATRIX_H
#define LAB2_MATRIX_H

#include <fstream>
#include <vector>
#include <thread>
#include <iomanip>


class Matrix{
private:
    int rows;
    int columns;
    std::vector<int> matrix;
public:
    Matrix(int rows, int cols);
    void matrix_generate();
    int size();
    int get_item(int item_index);
    void set_item(int item_index, int element_value);
    void sum(Matrix& first_matrix, Matrix& second_matrix, Matrix& result, int num_threads);
    void write_result();
};

#endif
