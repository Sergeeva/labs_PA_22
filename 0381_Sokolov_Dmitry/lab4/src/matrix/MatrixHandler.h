#ifndef PA_LAB2_MATRIXHANDLER_H
#define PA_LAB2_MATRIXHANDLER_H

#include <thread>
#include <functional>
#include <fstream>
#include <filesystem>

#include "./Matrix.h"
#include "../utilities/Generator.h"

class MatrixHandler {
    static const Generator gen;

    static void row_multiply(Matrix& A, Matrix& B, int current_row, int rows_to_iterate, Matrix& result);

    static void strassen_mult(Matrix& A, Matrix& B, Matrix* result, int thread_number, int depth=1);


public:

    static std::string check_data(Matrix& first, Matrix& second, int thread_number,
                                  bool multiplication = false) noexcept;

    static std::vector<Matrix> get_squares(int size, int amount);

    static void split_matrix(Matrix* source, int split, Matrix& a11, Matrix& a12, Matrix& a21, Matrix& a22);

    static void merge_matrix(Matrix* source, int split, Matrix& a11, Matrix& a12, Matrix& a21, Matrix& a22);

    static bool strassen_compatible(Matrix& matrix);

    static void strassen_convert(Matrix& matrix);

    static Matrix create(int rows, int columns, unsigned int seed);

    static Matrix parallel_sum(Matrix A, Matrix B, int thread_number);

    static Matrix parallel_sub(Matrix A, Matrix B, int thread_number);

    static int split_size(Matrix& matrix);

    static Matrix parallel_mult(Matrix& A, Matrix& B, int thread_number);

    static Matrix strassen_algorithm(Matrix& A, Matrix& B, int thread_number);

    static void output(Matrix& matrix, const std::string& path, const std::string& name);

};


#endif //PA_LAB2_MATRIXHANDLER_H
