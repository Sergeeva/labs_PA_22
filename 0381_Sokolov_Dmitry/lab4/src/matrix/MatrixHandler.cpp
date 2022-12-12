#include "MatrixHandler.h"

const Generator MatrixHandler::gen = Generator();

Matrix MatrixHandler::create(int rows, int columns, unsigned int seed) {
    return Matrix(MatrixHandler::gen.generate(rows, columns, seed));
}

Matrix MatrixHandler::parallel_sum(Matrix &A, Matrix &B, int thread_number) {
    if (thread_number < Config::min_threads) {
        throw std::runtime_error("Too little threads to perform calculation: " + std::to_string(thread_number));
    }

    if (thread_number > Config::max_threads) {
        throw std::runtime_error("Thread limit exceeded! Number of threads: " + std::to_string(thread_number));
    }

    if (!A.check_dimensions(B)) {
        throw std::runtime_error("Matrix dimensions do not match!");
    }

    Matrix result = Matrix(A.get_rows(), A.get_columns());

    int size = result.get_rows() * result.get_columns();
    int per_thread = (size / thread_number) + (size % thread_number > 0 ? 1 : 0);

    std::vector<std::thread> execution;

    for (int i = 0; i < size; i += per_thread) {
        std::thread thread(std::bind(&Matrix::partial_sum, std::ref(A), std::ref(B), i, per_thread,
                                     std::ref(result)));
        execution.emplace_back(std::move(thread));
    }

    for (auto &thread: execution) {
        thread.join();
    }

    return result;
}

void MatrixHandler::row_multiply(Matrix& A, Matrix& B, int current_row, int rows_to_iterate, Matrix& result) {

    if (rows_to_iterate <= 0) {
        return;
    }

    rows_to_iterate--;

    for (int i = 0; i < B.get_columns(); i++) {
        Matrix::partial_mult(A, B, current_row, i, result);
    }

    current_row++;

    if (current_row >= result.get_rows()) {
        return;
    }

    row_multiply(A, B, current_row, rows_to_iterate, result);

}

Matrix MatrixHandler::parallel_mult(Matrix &A, Matrix &B, int thread_number) {
    if (thread_number < Config::min_threads) {
        throw std::runtime_error("Too little threads to perform calculation: " + std::to_string(thread_number));
    }

    if (thread_number > Config::max_threads) {
        throw std::runtime_error("Thread limit exceeded! Number of threads: " + std::to_string(thread_number));
    }

    if (!A.check_dimensions(B, false)) {
        throw std::runtime_error("Matrix dimensions do not match the multiplication requirement!");
    }

    Matrix result = Matrix(A.get_rows(), B.get_columns());

    int size = result.get_rows();
    int per_thread = (size / thread_number) + (size % thread_number > 0 ? 1 : 0);

    if (per_thread > Config::recursion_limit) {
        throw std::runtime_error("Maximum recursion depth exceeded! (" + std::to_string(per_thread) +
                                 " - current \\ " + std::to_string(Config::recursion_limit) + " - allowed)");
    }

    std::vector<std::thread> execution;

    for (int i = 0; i < size; i += per_thread) {

        std::thread thread(std::bind(&MatrixHandler::row_multiply, A, B, i, per_thread,
                                             std::ref(result)));

        execution.emplace_back(std::move(thread));
    }

    for (auto &thread: execution) {
        thread.join();
    }

    return result;

}

void MatrixHandler::strassen_compitable(Matrix &matrix) {
    int new_size = std::pow(2, std::ceil(std::log2(std::max(matrix.get_rows(), matrix.get_columns()))));

    matrix.resize(new_size, new_size);
}

void MatrixHandler::output(Matrix& matrix, const std::string& path, const std::string& name) {

    std::string file_path = path + std::to_string(matrix.get_id()) + ".txt";

    std::ofstream output;
    output.open(file_path, std::ofstream::app);

    if (!output.is_open()) {
        try {
            std::filesystem::create_directories(path);
            output.open(file_path, std::ofstream::out);
        }
        catch (...)
        {
            throw std::ofstream::failure("Couldn't open file at {" + file_path + "}!");
        }
    }

    output << name << ":\n";
    output << matrix;
    output << "\n";
    output.close();
}
