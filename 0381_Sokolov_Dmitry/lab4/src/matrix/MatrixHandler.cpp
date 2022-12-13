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

Matrix MatrixHandler::parallel_sum(Matrix A, Matrix B, int thread_number) {

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

Matrix MatrixHandler::parallel_sub(Matrix A, Matrix B, int thread_number) {
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

Matrix MatrixHandler::parallel_mult(Matrix &A, Matrix &B, int thread_number) {
    std::string check = MatrixHandler::check_data(A, B, thread_number, true);

    if (!check.empty()) {
        throw std::runtime_error(check);
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

//----------------------------------------------------------------------------------------------------------------------

bool MatrixHandler::strassen_compatible(Matrix &matrix) {
    if (!matrix.is_square()) {
        return false;
    }

    if (matrix.get_rows() % 2 != 0) {
        return false;
    }

    return true;
}

void MatrixHandler::strassen_convert(Matrix &matrix) {
    int new_size = std::pow(2, std::ceil(std::log2(std::max(matrix.get_rows(), matrix.get_columns()))));

    matrix.resize(new_size, new_size);
}

int MatrixHandler::split_size(Matrix &matrix) {
    if (!matrix.is_square()) {
        throw std::runtime_error("Matrix is not square!");
    }

    return matrix.get_rows() / 2;
}

void MatrixHandler::split_matrix(Matrix* source, int split, Matrix& a11, Matrix& a12, Matrix& a21, Matrix& a22) {

    std::thread first(&Matrix::get_tile, source, std::ref(a11), 0, 0, split);
    std::thread second(&Matrix::get_tile, source, std::ref(a12), 0, split, split);
    std::thread third(&Matrix::get_tile, source, std::ref(a21), split, 0, split);
    std::thread fourth(&Matrix::get_tile, source, std::ref(a22), split, split, split);

    first.join();
    second.join();
    third.join();
    fourth.join();

}

void MatrixHandler::merge_matrix(Matrix *source, int split, Matrix &a11, Matrix &a12, Matrix &a21, Matrix &a22) {
    std::thread first(&Matrix::set_tile, source, std::ref(a11), 0, 0, split);
    std::thread second(&Matrix::set_tile, source, std::ref(a12), 0, split, split);
    std::thread third(&Matrix::set_tile, source, std::ref(a21), split, 0, split);
    std::thread fourth(&Matrix::set_tile, source, std::ref(a22), split, split, split);

    first.join();
    second.join();
    third.join();
    fourth.join();

}

Matrix MatrixHandler::strassen_algorithm(Matrix &A, Matrix &B, int thread_number) {

    std::string check = MatrixHandler::check_data(A, B, thread_number, true);

    if (!check.empty()) {
        throw std::runtime_error(check);
    }

    if(!MatrixHandler::strassen_compatible(A)) {
        MatrixHandler::strassen_convert(A);
    }

    if(!MatrixHandler::strassen_compatible(B)) {
        MatrixHandler::strassen_convert(B);
    }

    Matrix* result = new Matrix(A.get_rows(), B.get_columns());

    strassen_mult((Matrix &) A, (Matrix &) B, result, thread_number);

    return *result;
}

void MatrixHandler::strassen_mult(Matrix &A, Matrix &B, Matrix *result, int thread_number, int depth) {
    if (result->get_size() <= Config::size_floor || depth > Config::recursion_limit) {
        *result = A * B;
        return;
    }

    int new_size = MatrixHandler::split_size(*result);

    // A matrix partition
    std::vector<Matrix> a = MatrixHandler::get_squares(new_size, 4);
    // B matrix partition
    std::vector<Matrix> b = MatrixHandler::get_squares(new_size, 4);

    MatrixHandler::split_matrix(&A, new_size, a[0], a[1], a[2], a[3]);
    MatrixHandler::split_matrix(&B, new_size, b[0], b[1], b[2], b[3]);

    std::vector<std::thread> execution (7);
    std::vector<Matrix> p = MatrixHandler::get_squares(new_size, 7);

//    execution[0] = std::thread([]);


}

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

