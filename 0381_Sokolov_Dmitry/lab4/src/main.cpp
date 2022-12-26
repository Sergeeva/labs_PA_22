#include <iostream>
#include <chrono>

#include "./matrix/multiplication/Strassen.h"


using namespace std::chrono;


int main() {

    Matrix A = MatrixHandler::create(Config::R, Config::C, 322);
    Matrix B = MatrixHandler::create(Config::R, Config::C, 228);

    auto start = steady_clock::now();
    Matrix C = MatrixHandler::parallel_mult(A, B, Config::execution_threads);
    Logger::info("Simple parallelization finished within: " +
                  std::to_string(duration_cast<microseconds>(steady_clock::now() - start).count()));

    Logger::info("-----------------------------------------------------------------\n\n", true);

    Logger::dump(Config::log_path);

    Logger::info("Strassen algorithm started\n", true);

    start = steady_clock::now();

    Matrix D = Strassen::parallel_mult(A, B, Config::execution_threads);

    Logger::info("Strassen finished within: " +
                 std::to_string(duration_cast<microseconds>(steady_clock::now() - start).count()));

    Logger::info("Validation: " + std::to_string(C == D) + "\n");

    Logger::dump(Config::log_path);

    return 0;

}
