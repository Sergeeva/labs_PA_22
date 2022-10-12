#include "main.h"

int main() {
    Logger::printCPlusPlusVersion();

    Matrix first_matrix;
    Matrix second_matrix;
    enter_matrices(first_matrix, second_matrix);

    auto start = Time::now();
    auto calculated_matrix_default = first_matrix.multiply(second_matrix, 4);
    auto elapsed_time = Time::get_elapsed_time_in_microseconds(start);
    Logger::printThreadIdWithText("Elapsed time [default]: " + std::to_string(elapsed_time));

    start = Time::now();
    auto calculated_matrix_strassen = first_matrix.strassen_multiply(second_matrix);
    elapsed_time = Time::get_elapsed_time_in_microseconds(start);
    Logger::printThreadIdWithText("Elapsed time [strassen]: " + std::to_string(elapsed_time));

    auto are_matrices_equal = calculated_matrix_default == calculated_matrix_strassen;
    Logger::printThreadIdWithText(std::string("Calculated matrices are") + (are_matrices_equal ? "" : " NOT") + " equal");

    print_matrix(calculated_matrix_strassen);
}

void enter_matrices(Matrix& first_matrix, Matrix& second_matrix) {
    std::ifstream input(filenames::kMatrixInputFilename);
    input >> first_matrix >> second_matrix;
}

void print_matrix(const Matrix& matrix) {
    std::ofstream output(filenames::kMatrixOutputFilename);
    output << matrix;
}
