#include "Strassen.h"
#include "../../utilities/session_timer.h"

std::atomic<int> thread_number{};

int get_num(){
    return thread_number++;
}

Strassen::Strassen(bool parallelism): parallel_execution(parallelism) {}

bool Strassen::is_compatible(Matrix &matrix) {
    if (!matrix.is_square()) {
        return false;
    }

    if (matrix.get_rows() % 2 != 0) {
        return false;
    }

    return true;
}

void Strassen::convert_to_square(Matrix &matrix) {
    int new_size = std::pow(2, std::ceil(std::log2(std::max(matrix.get_rows(), matrix.get_columns()))));

    matrix.resize(new_size, new_size);
}

int Strassen::split_size(int size) {
    return size / 2;
}

Matrix Strassen::assemble_parts(const Matrix &c11, const Matrix &c12, const Matrix &c21, const Matrix &c22, int size) {
    Matrix result = Matrix(size);

    result.set_tile(c11, 0, 0, c11.get_rows());
    result.set_tile(c12, 0, c12.get_rows(), c12.get_rows());
    result.set_tile(c21, c21.get_rows(), 0, c21.get_rows());
    result.set_tile(c22, c22.get_rows(), c22.get_rows(), c22.get_rows());

    return result;
}

Matrix Strassen::parallel_mult(Matrix &A, Matrix &B, int thread_number) {
    if(!Strassen::is_compatible(A)) {
        Strassen::convert_to_square(A);
    }

    if(!Strassen::is_compatible(B)) {
        Strassen::convert_to_square(B);
    }

    std::string check = MatrixHandler::check_data(A, B, thread_number, true);

    if (!check.empty()) {
        throw std::runtime_error(check);
    }

    return Strassen::strassen_algorithm(A, B);

}

Matrix Strassen::strassen_algorithm(const Matrix &A, const Matrix &B, int depth) {
//    Logger::trace(std::to_string(depth) + "depth reached! \n", true);

    if (A.get_size() <= Config::size_floor) {
//        Logger::trace("Size floor reached! Starting simple multiplication \n", true);
        return A * B;
    }

    else {
        if (depth < Config::recursion_limit) {
            return parallel_strassen(A, B, depth);
        }
    }

//    Logger::trace("recursion limit reached! Starting consecutive Strassen \n", true);
    return Strassen::serial_strassen(A, B, depth);
}

Matrix Strassen::serial_strassen(const Matrix &A, const Matrix &B, int depth) {
    depth++;

    int split_size = Strassen::split_size(A.get_rows());

    Matrix a11 = A.get_tile(0, 0, split_size);
    Matrix a12 = A.get_tile(0, split_size, split_size);
    Matrix a21 = A.get_tile(split_size, 0, split_size);
    Matrix a22 = A.get_tile(split_size, split_size, split_size);

    Matrix b11 = B.get_tile(0, 0, split_size);
    Matrix b12 = B.get_tile(0, split_size, split_size);
    Matrix b21 = B.get_tile(split_size, 0, split_size);
    Matrix b22 = B.get_tile(split_size, split_size, split_size);

    Matrix p1 = Strassen::strassen_algorithm(a11 + a22, b11 + b22, depth);
    Matrix p2 = Strassen::strassen_algorithm(a21 + a22, b11, depth);
    Matrix p3 = Strassen::strassen_algorithm(a11, b12 - b22, depth);
    Matrix p4 = Strassen::strassen_algorithm(a22, b21 - b11, depth);
    Matrix p5 = Strassen::strassen_algorithm(a11 + a22, b21, depth);
    Matrix p6 = Strassen::strassen_algorithm(a21 - a11, b11 + b12, depth);
    Matrix p7 = Strassen::strassen_algorithm(a12 - a22, b21 + b22, depth);

    Matrix c11 = p1 + p4 - p5 + p7;
    Matrix c12 = p3 + p5;
    Matrix c21 = p2 + p4;
    Matrix c22 = p1 - p2 + p3 + p6;

    return assemble_parts(c11, c12, c21, c22, A.get_rows());
}

Matrix Strassen::parallel_strassen(const Matrix &A, const Matrix &B, int depth) {
    depth++;

    int split_size = Strassen::split_size(A.get_rows());

    Matrix a11 = A.get_tile(0, 0, split_size);
    Matrix a12 = A.get_tile(0, split_size, split_size);
    Matrix a21 = A.get_tile(split_size, 0, split_size);
    Matrix a22 = A.get_tile(split_size, split_size, split_size);

    Matrix b11 = B.get_tile(0, 0, split_size);
    Matrix b12 = B.get_tile(0, split_size, split_size);
    Matrix b21 = B.get_tile(split_size, 0, split_size);
    Matrix b22 = B.get_tile(split_size, split_size, split_size);

    std::vector<std::thread> execution;

    std::promise<Matrix> P1;
    auto F1 = P1.get_future();
    Strassen::execute(execution, a11 + a22, b11 + b22, std::ref(P1), depth);

    std::promise<Matrix> P2;
    auto F2 = P2.get_future();
    Strassen::execute(execution, a21 + a22, b11, std::ref(P2), depth);

    std::promise<Matrix> P3;
    auto F3 = P3.get_future();
    Strassen::execute(execution, a11, b12 - b22, std::ref(P3), depth);

    std::promise<Matrix> P4;
    auto F4 = P4.get_future();
    Strassen::execute(execution, a22, b21 - b11, std::ref(P4), depth);

    std::promise<Matrix> P5;
    auto F5 = P5.get_future();
    Strassen::execute(execution, a11 + a12, b22, std::ref(P5), depth);

    std::promise<Matrix> P6;
    auto F6 = P6.get_future();
    Strassen::execute(execution, a21 - a11, b11 + b12, std::ref(P6), depth);

    std::promise<Matrix> P7;
    auto F7 = P7.get_future();
    Strassen::execute(execution, a12 - a22, b21 + b22, std::ref(P7), depth);

    Matrix p1 = F1.get();
    Matrix p2 = F2.get();
    Matrix p3 = F3.get();
    Matrix p4 = F4.get();
    Matrix p5 = F5.get();
    Matrix p6 = F6.get();
    Matrix p7 = F7.get();

    Matrix c11 = p1 + p4 - p5 + p7;
    Matrix c12 = p3 + p5;
    Matrix c21 = p2 + p4;
    Matrix c22 = p1 - p2 + p3 + p6;

    for (auto &thread: execution) {
        thread.join();
    }

    return Strassen::assemble_parts(c11, c12, c21, c22, A.get_rows());
}

void Strassen::execute(std::vector<std::thread> &execution,
                       const Matrix &A, const Matrix &B, std::promise<Matrix> &result, int depth)
{
     execution.emplace_back([]
            (const Matrix& first, const Matrix& second, std::promise<Matrix>& result, int depth)
            {
                result.set_value(Strassen::strassen_algorithm(first, second, depth));
            }, A, B, std::ref(result), depth);
}
