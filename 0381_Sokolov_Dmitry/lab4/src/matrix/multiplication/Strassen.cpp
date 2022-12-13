#include "Strassen.h"

Strassen::Strassen(bool parallelism): parallel_execution(parallelism) {}

std::vector<Matrix> Strassen::get_squares(int size, int amount) {
    std::vector<Matrix> _result;

    for (int i = 0; i < amount; i++) {
        _result.emplace_back(size);
    }

    return _result;
}

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

int Strassen::split_size(Matrix &matrix) {
    if (!matrix.is_square()) {
        throw std::runtime_error("Matrix is not square!");
    }

    return matrix.get_rows() / 2;
}

void Strassen::split_matrix(Matrix& source, int split,
                            Matrix &a11, Matrix &a12, Matrix &a21, Matrix &a22) {

    std::thread first(&Matrix::get_tile, &source, std::ref(a11), 0, 0, split);
    std::thread second(&Matrix::get_tile, &source, std::ref(a12), 0, split, split);
    std::thread third(&Matrix::get_tile, &source, std::ref(a21), split, 0, split);
    std::thread fourth(&Matrix::get_tile, &source, std::ref(a22), split, split, split);

    first.join();
    second.join();
    third.join();
    fourth.join();
}

void Strassen::merge_matrix(Matrix* source, int split,
                            Matrix &a11, Matrix &a12, Matrix &a21, Matrix &a22) {

    std::thread first(&Matrix::set_tile, source, std::ref(a11), 0, 0, split);
    std::thread second(&Matrix::set_tile, source, std::ref(a12), 0, split, split);
    std::thread third(&Matrix::set_tile, source, std::ref(a21), split, 0, split);
    std::thread fourth(&Matrix::set_tile, source, std::ref(a22), split, split, split);

    first.join();
    second.join();
    third.join();
    fourth.join();
}

void Strassen::strassen_algorithm(Matrix &first, Matrix &second, Matrix* result, int thread_number, int depth) {
    if (!(result->get_size() <= Config::size_floor || depth > Config::recursion_limit)) {
        int new_size = split_size(*result);

        // first matrix partition
        std::vector<Matrix> a = get_squares(new_size, 4);
        // second matrix partition
        std::vector<Matrix> b = get_squares(new_size, 4);

        split_matrix(first, new_size, a[0], a[1], a[2], a[3]);
        split_matrix(second, new_size, b[0], b[1], b[2], b[3]);

        std::vector<std::thread> execution (7);
        std::vector<Matrix> p = MatrixHandler::get_squares(new_size, 7);

        // recursive call for sum(a11, a22) and sum(b11, b22)
        execution[0] = std::thread(std::bind(&Strassen::strassen_algorithm, this,
                                             MatrixHandler::parallel_sum(a[0], a[3], thread_number), // a11 + a22
                                             MatrixHandler::parallel_sum(b[0], b[3], thread_number), // b11 + b22
                                             &p[0], // p1
                                             thread_number,
                                             depth + 1));

        // recursive call for sum(a21, a22) and b11
        execution[1] = std::thread(std::bind(&Strassen::strassen_algorithm, this,
                                             MatrixHandler::parallel_sum(a[2], a[3], thread_number), // a21 + a22
                                             b[0],  // b11
                                             &p[1], // p2
                                             thread_number,
                                             depth + 1));

        // recursive call for a11 and sub(b12, b22)
        execution[2] = std::thread(std::bind(&Strassen::strassen_algorithm, this,
                                             a[0], // a11
                                             MatrixHandler::parallel_sub(b[1], b[3], thread_number), // b12 - b22
                                             &p[2], // p3
                                             thread_number,
                                             depth + 1));

        // recursive call for a22 and (b21, b11)
        execution[3] = std::thread(std::bind(&Strassen::strassen_algorithm, this,
                                             a[3], // a22
                                             MatrixHandler::parallel_sub(b[2], b[0], thread_number), // b21 - b11
                                             &p[3], // p4
                                             thread_number,
                                             depth + 1));

        // recursive call for sum(a11, a12) and b22
        execution[4] = std::thread(std::bind(&Strassen::strassen_algorithm, this,
                                             MatrixHandler::parallel_sum(a[0], a[1], thread_number), // a11 - a12
                                             b[3],  // b22
                                             &p[4], // p5
                                             thread_number,
                                             depth + 1));

        // recursive call for sub(a21, a11) and sum(b11, b12)
        execution[5] = std::thread(std::bind(&Strassen::strassen_algorithm, this,
                                             MatrixHandler::parallel_sub(a[2], a[0], thread_number), // a21 - a11
                                             MatrixHandler::parallel_sum(b[0], b[1], thread_number), // b11 + b12
                                             &p[5], // p6
                                             thread_number,
                                             depth + 1));

        // recursive call for sub(a12, a22) and sum(b21, b22)
        execution[6] = std::thread(std::bind(&Strassen::strassen_algorithm, this,
                                             MatrixHandler::parallel_sub(a[1], a[3], thread_number), // a12 - a22
                                             MatrixHandler::parallel_sum(b[2], b[3], thread_number), // b21 + b22
                                             &p[6], // p7
                                             thread_number,
                                             depth + 1));

        for (auto &exec: execution) {
            exec.join();
        }

        auto c = get_squares(new_size, 4);

        c[0] = MatrixHandler::parallel_sum(
                MatrixHandler::parallel_sum(p[0], p[3], thread_number),
                MatrixHandler::parallel_sub(p[6], p[4], thread_number),
                thread_number
                );

        c[1] = MatrixHandler::parallel_sum(p[2], p[4], thread_number);
        c[2] = MatrixHandler::parallel_sum(p[1], p[3], thread_number);

        c[3] = MatrixHandler::parallel_sum(
                MatrixHandler::parallel_sub(p[0], p[1], thread_number),
                MatrixHandler::parallel_sum(p[2], p[5], thread_number),
                thread_number
        );

        merge_matrix(result, new_size, c[0], c[1], c[2], c[3]);

    }
    else {
        *result = first * second;
    }
}

Matrix Strassen::parallel_mult(Matrix &A, Matrix &B, int thread_number) {
    if(!MatrixHandler::strassen_compatible(A)) {
        MatrixHandler::strassen_convert(A);
    }

    if(!MatrixHandler::strassen_compatible(B)) {
        MatrixHandler::strassen_convert(B);
    }

    std::string check = MatrixHandler::check_data(A, B, thread_number, true);

    if (!check.empty()) {
        throw std::runtime_error(check);
    }

    Matrix* result = new Matrix(A.get_rows(), B.get_columns());

    strassen_algorithm(A, B, result, thread_number);

    Matrix ret = *result;
    free(result);

    return ret;
}

