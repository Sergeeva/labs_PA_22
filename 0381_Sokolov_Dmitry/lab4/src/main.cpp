#include <iostream>

#include "./matrix/multiplication/Strassen.h"


int main() {

    Config::log_config();

    Logger::toggle_log(INFO);

    Matrix A = MatrixHandler::create(Config::R, Config::C, 322);
    Matrix B = MatrixHandler::create(Config::R, Config::C, 228);

    MatrixHandler::output(A, Config::Data_path, "A");
    MatrixHandler::output(A, Config::Data_path, "B");

    auto start = steady_clock::now();
    Matrix C = MatrixHandler::parallel_mult(A, B, Config::threads);
    Logger::info("Simple parallelization finished within: " +
                  std::to_string(duration_cast<milliseconds>(steady_clock::now() - start).count()) + " ms\n");

    MatrixHandler::output(C, Config::Data_path, "S1mple");

    Logger::dump(Config::log_path, "s1mple");

    start = steady_clock::now();

    Matrix D = Strassen::parallel_mult(A, B, Config::threads);

    Logger::info("Strassen finished within: " +
                 std::to_string(duration_cast<milliseconds>(steady_clock::now() - start).count()) + " ms\n");

    MatrixHandler::output(D, Config::Data_path, "Strassen");

    Logger::info("Validation: " + std::to_string(C == D) + "\n");

    Logger::dump(Config::log_path, "Strassen");

    return 0;
}
