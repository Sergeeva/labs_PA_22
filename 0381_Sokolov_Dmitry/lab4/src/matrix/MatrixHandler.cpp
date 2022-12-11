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

void MatrixHandler::output(Matrix& matrix, const std::string path, const std::string& name) {

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
