#ifndef SRC_STRASSEN_H
#define SRC_STRASSEN_H

#include <vector>
#include <memory>

#include "../MatrixHandler.h"


class Strassen {

    bool parallel_execution = false;

    std::vector<Matrix> get_squares(int size, int amount);

    void split_matrix(Matrix& source, int split,
                      Matrix& a11, Matrix& a12, Matrix& a21, Matrix& a22);

    void merge_matrix(Matrix* source, int split,
                             Matrix& a11, Matrix& a12, Matrix& a21, Matrix& a22);

public:

    Strassen() = default;

    explicit Strassen(bool parallelism);

    bool is_compatible(Matrix& matrix);

    void convert_to_square(Matrix& matrix);

    int split_size(Matrix& matrix);

    void strassen_algorithm(Matrix& first, Matrix& second, Matrix* result, int thread_number, int depth= 1);

    Matrix parallel_mult(Matrix& A, Matrix& B, int thread_number);

    ~Strassen() = default;
};


#endif //SRC_STRASSEN_H
