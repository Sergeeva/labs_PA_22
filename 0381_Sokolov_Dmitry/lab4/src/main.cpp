#include <iostream>
#include "./matrix/MatrixHandler.h"

int main() {
    Matrix A = MatrixHandler::create(4, 4, 228);
    std::cout << A;

    Matrix B = MatrixHandler::create(4, 4, 322);
    std::cout << B;

    Matrix C = Matrix(4, 4);

    for (int i = 0; i < A.get_rows(); i++) {
        for (int j = 0; j < B.get_columns(); j++) {
            Matrix::partial_mult(A, B, i, j, C);
        }
    }

    std::cout << C;

    return 0;
}
