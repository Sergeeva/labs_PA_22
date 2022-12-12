#include <iostream>
#include "./matrix/MatrixHandler.h"
#include "./utilities/session_timer.h"

int main() {

    SessionTimer tm = SessionTimer();

    Matrix A = MatrixHandler::create(Config::R, Config::C, 228);
    MatrixHandler::output(A, Config::Data_path, "A");

    Matrix B = MatrixHandler::create(Config::R, Config::C, 322);
    MatrixHandler::output(B, Config::Data_path, "B");

    //------------------------------------------------------------------------------------------------------------------

    tm.start_session();
    Matrix C = A * B;
    tm.finish_session();

    MatrixHandler::output(C, Config::Data_path, "Consecutive");

    std::cout << "Consecutive finished within: " << std::to_string(tm.last()) << std::endl;

    //------------------------------------------------------------------------------------------------------------------

    tm.start_session();
    Matrix D = MatrixHandler::parallel_mult(A, B, Config::execution_threads);
    tm.finish_session();

    MatrixHandler::output(D, Config::Data_path, "multiple threads");

    std::cout << "Parallel finished within: " << std::to_string(tm.last()) << std::endl;

    std::cout << "Results are equal: " << (C == D);


    return 0;
}
