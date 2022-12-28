#ifndef SRC_STRASSEN_H
#define SRC_STRASSEN_H

#include <vector>
#include <memory>
#include <future>
#include <atomic>

#include "../MatrixHandler.h"

int get_num();

class Strassen {

    bool parallel_execution = false;

    static Matrix assemble_parts(const Matrix& c11, const Matrix& c12, const Matrix& c21, const Matrix& c22, int size);

    static void execute(std::vector<std::thread>& execution,
                 const Matrix& A, const Matrix& B, std::promise<Matrix>& result, int depth);

public:

    Strassen() = default;

    explicit Strassen(bool parallelism);

    static bool is_compatible(Matrix& matrix);

    static void convert_to_square(Matrix& matrix);

    static int split_size(int size);

    static Matrix strassen_algorithm(const Matrix& A, const Matrix& B, int depth = 1);

    static Matrix serial_strassen(const Matrix& A, const Matrix& B, int depth);

    static Matrix parallel_strassen(const Matrix& A, const Matrix& B, int depth);

    static Matrix parallel_mult(Matrix& A, Matrix& B, int thread_number);

    ~Strassen() = default;
};


#endif //SRC_STRASSEN_H
