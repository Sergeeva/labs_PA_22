#include <iostream>
//#include "./matrix/MatrixHandler.h"
#include "./matrix/multiplication/Strassen.h"
#include "./utilities/session_timer.h"

int main() {

    Strassen st = Strassen();

    Matrix A = MatrixHandler::create(Config::R, Config::C, 228);
    Matrix B = MatrixHandler::create(Config::R, Config::C, 322);

    Matrix C = Matrix(A.get_rows(), B.get_columns());

    C = st.parallel_mult(A, B, Config::threads);

    Matrix correct = A * B;
    Matrix parallel = MatrixHandler::parallel_mult(A, B, Config::threads);

    std::cout << (C == correct && C == parallel) << std::endl;

    return 0;

}
