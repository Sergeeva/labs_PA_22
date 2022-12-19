#ifndef LAB2_MATRIX_H
#define LAB2_MATRIX_H

#include <fstream>
#include <vector>
#include <thread>
#include <complex>


class Matrix{
    private:
        int rows;
        int columns;
        std::vector<int> matrix;
    public:
        int getRows() const;
        int getColumns() const;
        Matrix(int rows, int cols);
        ~Matrix() = default;
        void matrix_generate();
        std::vector<int> get_matrix();
        int size();
        int get_item(int item_index);
        void set_item(int item_index, int element_value);
        void multiply(Matrix& first_matrix, Matrix& second_matrix, int num_threads);
        std::vector<Matrix> parts();
        void concatenation(std::vector<Matrix> &parts);
        void write_result();

};

Matrix strassenAlgorithm(Matrix &first_matrix, Matrix &second_matrix, int depth, int num_threads);

#endif
