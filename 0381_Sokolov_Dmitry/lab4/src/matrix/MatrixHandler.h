#ifndef PA_LAB2_MATRIXHANDLER_H
#define PA_LAB2_MATRIXHANDLER_H

#include <thread>
#include <functional>
#include <fstream>
#include <filesystem>
#include <future>

#include "./Matrix.h"
#include "../utilities/Generator.h"

class MatrixHandler {
    static const Generator gen;

    static void row_multiply(Matrix& A, Matrix& B, int current_row, int rows_to_iterate, Matrix& result);

    static void multiply_part(Matrix& A, Matrix& B, int start, int length, Matrix& result);


public:

    static std::string check_data(Matrix& first, Matrix& second, int thread_number,
                                  bool multiplication = false) noexcept;

    static std::vector<Matrix> get_squares(int size, int amount);

    static Matrix create(int rows, int columns, unsigned int seed);

    static Matrix parallel_sum(Matrix& A, Matrix& B, int thread_number);

    static Matrix parallel_sub(Matrix& A, Matrix& B, int thread_number);

    static Matrix by_row_mult(Matrix& A, Matrix& B, int thread_number);

    static Matrix parallel_mult(Matrix& A, Matrix& B, int thread_number);

    static void output(Matrix& matrix, const std::string& path, const std::string& name);

};


#endif //PA_LAB2_MATRIXHANDLER_H
