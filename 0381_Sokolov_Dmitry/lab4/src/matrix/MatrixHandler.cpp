#include "MatrixHandler.h"

const Generator MatrixHandler::gen = Generator();

Matrix MatrixHandler::create(int rows, int columns, unsigned int seed) {
    return Matrix(MatrixHandler::gen.generate(rows, columns, seed));
}

//----------------------------------------------------------------------------------------------------------------------

std::string MatrixHandler::check_data(Matrix& first, Matrix &second, int thread_number, bool multiplication) noexcept {
    if (thread_number < Config::min_threads) {
        return "Too little threads to perform calculation: " + std::to_string(thread_number);
    }

    if (thread_number > Config::max_threads) {
        return "Thread limit exceeded! Number of threads: " + std::to_string(thread_number);
    }

    if(multiplication) {
        if (!first.check_dimensions(second, false)) {
            return "Matrix dimensions do not match the multiplication requirement!";
        }
    }
    else {
        if (!first.check_dimensions(second)) {
            return "Matrix dimensions do not match!";
        }
    }

    return "";
}

std::vector<Matrix> MatrixHandler::get_squares(int size, int amount) {
    std::vector<Matrix> result;

    for (int i = 0; i < amount; i++) {
        result.emplace_back(size);
    }

    return result;
}

//----------------------------------------------------------------------------------------------------------------------

Matrix MatrixHandler::parallel_sum(Matrix& A, Matrix& B, int thread_number) {

    std::string check = MatrixHandler::check_data(A, B, thread_number);

    if (!check.empty()) {
        throw std::runtime_error(check);
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

Matrix MatrixHandler::parallel_sub(Matrix& A, Matrix& B, int thread_number) {
    std::string check = MatrixHandler::check_data(A, B, thread_number);

    if (!check.empty()) {
        throw std::runtime_error(check);
    }

    Matrix result = Matrix(A.get_rows(), A.get_columns());

    int size = result.get_rows() * result.get_columns();
    int per_thread = (size / thread_number) + (size % thread_number > 0 ? 1 : 0);

    std::vector<std::thread> execution;

    for (int i = 0; i < size; i += per_thread) {
        std::thread thread(std::bind(&Matrix::partial_sub, std::ref(A), std::ref(B), i, per_thread,
                                     std::ref(result)));
        execution.emplace_back(std::move(thread));
    }

    for (auto &thread: execution) {
        thread.join();
    }

    return result;
}

//----------------------------------------------------------------------------------------------------------------------

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

void MatrixHandler::multiply_part(Matrix &A, Matrix &B, int start, int length, Matrix &result) {
    Logger::trace("multiplying part: " + std::to_string(start) + " - " + std::to_string(start + length) +
    "\n", true);

    int size = result.get_rows() * result.get_columns();

    for (int i = start; (i < (start + length)) && (i < size); ++i) {
        int y = i / result.get_columns();
        int x = i % result.get_columns();

        Matrix::partial_mult(A, B, y, x, result);
    }
}

Matrix MatrixHandler::by_row_mult(Matrix &A, Matrix &B, int thread_number) {
    std::string check = MatrixHandler::check_data(A, B, thread_number, true);

    if (!check.empty()) {
        throw std::runtime_error(check);
    }

    Matrix result = Matrix(A.get_rows(), B.get_columns());

    int size = result.get_rows();
    int per_thread = (size / thread_number) + (size % thread_number > 0 ? 1 : 0);

    if (per_thread > Config::depth_limit) {
        throw std::runtime_error("Maximum recursion depth exceeded! (" + std::to_string(per_thread) +
                                 " - current \\ " + std::to_string(Config::depth_limit) + " - allowed)");
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

Matrix MatrixHandler::parallel_mult(Matrix &A, Matrix &B, int thread_number) {
    std::string check = MatrixHandler::check_data(A, B, thread_number, true);

    if (!check.empty()) {
        throw std::runtime_error(check);
    }

    Matrix result = Matrix(A.get_rows(), B.get_columns());

    int size = result.get_rows() * result.get_columns();
    int per_thread = (size / thread_number) + (size % thread_number > 0 ? 1 : 0);

    std::vector<std::thread> execution;

    Logger::info("Starting simple parallelization! Each thread processes " + std::to_string(per_thread) +
    " {\n", true);

    for (int i = 0; i < size; i += per_thread) {
        std::thread thread([&A, &B, i, per_thread, &result]
        { MatrixHandler::multiply_part(std::ref(A), std::ref(B), i, per_thread,
                                       std::ref(result)); }
        );

        execution.emplace_back(std::move(thread));
    }

    for (auto &thread: execution) {
        thread.join();
    }

    Logger::info("} Simple parallelization ended!\n\n", true);

    return result;
}

//----------------------------------------------------------------------------------------------------------------------

//void MatrixHandler::threaded_sum(Matrix &A, Matrix &B, std::promise<Matrix> result, int thread_number) {
//    result.set_value(MatrixHandler::parallel_sum(A, B, thread_number));
//}
//
//void MatrixHandler::threaded_sub(Matrix &A, Matrix &B, std::promise<Matrix> result, int thread_number) {
//    result.set_value(MatrixHandler::parallel_sub(A, B, thread_number));
//}

//----------------------------------------------------------------------------------------------------------------------

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
