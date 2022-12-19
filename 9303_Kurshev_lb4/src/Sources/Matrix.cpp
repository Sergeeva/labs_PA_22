#include "../Headers/Matrix.h"


Matrix::Matrix(int rows, int cols): rows(rows), columns(cols), matrix(std::vector<int>(rows * cols)){}

int Matrix::get_item(int item_index){
    return this->matrix[item_index];
}

void Matrix::set_item(int item_index, int element_value){
    this->matrix[item_index] = element_value;
}

void Matrix::matrix_generate(){
    for (int i = 0; i < this->rows * this->columns; i++)
        this->matrix[i] = std::rand() % 12 - 6;
}

int Matrix::size() {
    return this->matrix.size();
}

int Matrix::getRows() const{
    return this->rows;
}

int Matrix::getColumns() const{
    return this->columns;
}

void sum_matrices(Matrix& first_matrix, Matrix& second_matrix, Matrix& result, int start_index, int end_index){
    for (int i = start_index; i <= end_index; i++)
        result.set_item(i, first_matrix.get_item(i) + second_matrix.get_item(i));
}

void sub_matrices(Matrix& first_matrix, Matrix& second_matrix, Matrix& result, int start_index, int end_index){
    for (int i = start_index; i <= end_index; i++)
        result.set_item(i, first_matrix.get_item(i) - second_matrix.get_item(i));
}

void mult_matrices(Matrix& first_matrix, Matrix& second_matrix, Matrix* result, int start_index, int end_index){
    for (int i = start_index; i < end_index; i++){
        int elem = 0;
        int curRow = i / result->getRows();
        int curColumn = i % result->getColumns();
        for (int j = 0; j < first_matrix.getColumns(); j++)
            elem += (first_matrix.get_item(curRow * first_matrix.getColumns() + j) * second_matrix.get_item(j * second_matrix.getColumns() + curColumn));
        result->set_item(i, elem);
    }
}

Matrix sum(Matrix& first_matrix, Matrix& second_matrix, int num_threads) {
    Matrix result(first_matrix.getRows(), first_matrix.getColumns());
    std::vector<std::thread> sum_threads(num_threads);
    int min_count = 0;
    int min_thread_rows_size = first_matrix.size() / num_threads;
    int max_thread_rows_size = min_thread_rows_size + 1;
    if (first_matrix.size() % num_threads == 0) {
        min_count = num_threads;
    }
    else {
        for (int i = 0; i < first_matrix.size(); i++) {
            if ((first_matrix.size() - min_thread_rows_size * i) % max_thread_rows_size == 0) {
                min_count = i;
                break;
            }
        }
    }
    for (int i = 0; i < num_threads; i++) {
        if (i < min_count) {
            sum_threads[i] = std::thread(sum_matrices, std::ref(first_matrix), std::ref(second_matrix),
                                         std::ref(result), i * min_thread_rows_size,
                                         (i + 1) * min_thread_rows_size - 1);
        } else {
            sum_threads[i] = std::thread(sum_matrices, std::ref(first_matrix), std::ref(second_matrix),
                                         std::ref(result),
                                         min_count * min_thread_rows_size + (i - min_count) * max_thread_rows_size,
                                         min_count * min_thread_rows_size + (i - min_count + 1) * max_thread_rows_size -
                                         1);
        }
    }

    for (auto & sum_thread : sum_threads){
        sum_thread.join();
    }
    return result;
}

void Matrix::multiply(Matrix& first_matrix, Matrix& second_matrix, int num_threads) {
    std::vector<std::thread> sum_threads(num_threads);
    int min_count = 0;
    int min_thread_rows_size = first_matrix.size() / num_threads;
    int max_thread_rows_size = min_thread_rows_size + 1;
    if (first_matrix.size() % num_threads == 0) {
        min_count = num_threads;
    }
    else {
        for (int i = 0; i < first_matrix.size(); i++) {
            if ((first_matrix.size() - min_thread_rows_size * i) % max_thread_rows_size == 0) {
                min_count = i;
                break;
            }
        }
    }
    for (int i = 0; i < num_threads; i++) {
        if (i < min_count) {
            sum_threads[i] = std::thread(mult_matrices, std::ref(first_matrix), std::ref(second_matrix),
                                         this, i * min_thread_rows_size,
                                         (i + 1) * min_thread_rows_size - 1);
        } else {
            sum_threads[i] = std::thread(mult_matrices, std::ref(first_matrix), std::ref(second_matrix),
                                         this,
                                         min_count * min_thread_rows_size + (i - min_count) * max_thread_rows_size,
                                         min_count * min_thread_rows_size + (i - min_count + 1) * max_thread_rows_size -
                                         1);
        }
    }

    for (auto & sum_thread : sum_threads){
        sum_thread.join();
    }
}

Matrix sub(Matrix& first_matrix, Matrix& second_matrix, int num_threads) {
    Matrix result(first_matrix.getRows(), first_matrix.getColumns());
    std::vector<std::thread> sum_threads(num_threads);
    int min_count = 0;
    int min_thread_rows_size = first_matrix.size() / num_threads;
    int max_thread_rows_size = min_thread_rows_size + 1;
    if (first_matrix.size() % num_threads == 0) {
        min_count = num_threads;
    }
    else {
        for (int i = 0; i < first_matrix.size(); i++) {
            if ((first_matrix.size() - min_thread_rows_size * i) % max_thread_rows_size == 0) {
                min_count = i;
                break;
            }
        }
    }
    for (int i = 0; i < num_threads; i++) {
        if (i < min_count) {
            sum_threads[i] = std::thread(sub_matrices, std::ref(first_matrix), std::ref(second_matrix),
                                         std::ref(result), i * min_thread_rows_size,
                                         (i + 1) * min_thread_rows_size - 1);
        } else {
            sum_threads[i] = std::thread(sub_matrices, std::ref(first_matrix), std::ref(second_matrix),
                                         std::ref(result),
                                         min_count * min_thread_rows_size + (i - min_count) * max_thread_rows_size,
                                         min_count * min_thread_rows_size + (i - min_count + 1) * max_thread_rows_size -
                                         1);
        }
    }

    for (auto & sum_thread : sum_threads){
        sum_thread.join();
    }
    return result;
}

std::vector<Matrix> Matrix::parts(){
    int size = this->getRows() / 2;
    std::vector<Matrix> result = {Matrix(size, size), Matrix(size, size), Matrix(size, size), Matrix(size, size)};
    for (int i = 0; i < this->getRows(); i++)
        for (int j = 0; j < this->getColumns(); j++)
            result[2 * (i / size) + j / size].set_item(size * (i % size) + j % size, matrix[i * this->getColumns() + j]);
    return result;
}

void Matrix::concatenation(std::vector<Matrix> &parts){
    int size = parts[0].getRows();
    for (int i = 0; i < this->getRows(); i++)
        for (int j = 0; j < this->getColumns(); j++)
            this->matrix[i * this->getColumns() + j] = parts[2 * (i / size) + j / size].get_item(size * (i % size) + j % size);
}

void strassen(Matrix &first_matrix, Matrix &second_matrix, Matrix& result_matrix, int depth, int num_threads, bool isInit){
    if ((depth == 0) || (result_matrix.getRows() * result_matrix.getColumns() <= 4 * 4)){
        result_matrix.multiply(first_matrix, second_matrix, 1);
    } else {
        std::vector<Matrix> first_matrix_parts = first_matrix.parts();
        std::vector<Matrix> second_matrix_parts = second_matrix.parts();
        int size = first_matrix_parts[0].getRows();
        std::vector<Matrix> m = {
                Matrix(size, size), Matrix(size, size), Matrix(size, size), Matrix(size, size), Matrix(size, size), Matrix(size, size), Matrix(size, size)
        };
        if (!isInit){
            Matrix first = sum(first_matrix_parts[0], first_matrix_parts[3], num_threads);
            Matrix second = sum(second_matrix_parts[0], second_matrix_parts[3], num_threads);
            strassen(first,
                     second,
                     m[0], depth - 1, num_threads, false);

            first = sum(first_matrix_parts[2], first_matrix_parts[3], num_threads);
            strassen(first,
                     second_matrix_parts[0],
                     m[1], depth - 1, num_threads, false);

            second = sub(second_matrix_parts[1], second_matrix_parts[3], num_threads);
            strassen(first_matrix_parts[0],
                     second,
                     m[2], depth - 1, num_threads, false);

            second = sub(second_matrix_parts[2], second_matrix_parts[0], num_threads);
            strassen(first_matrix_parts[3],
                     second,
                     m[3], depth - 1, num_threads, false);

            first = sum(first_matrix_parts[0], first_matrix_parts[1], num_threads);
            strassen(first,
                     second_matrix_parts[3],
                     m[4], depth-1, num_threads, false);

            first = sub(first_matrix_parts[2], first_matrix_parts[0], num_threads);
            second = sum(second_matrix_parts[0], second_matrix_parts[1], num_threads);
            strassen(first,
                     second,
                     m[5], depth-1, num_threads, false);

            first = sub(first_matrix_parts[1], first_matrix_parts[3], num_threads);
            second = sum(second_matrix_parts[2], second_matrix_parts[3], num_threads);
            strassen(first,
                     second,
                     m[6], depth-1, num_threads, false);
        }
        else {
            std::vector<std::thread> threads(7);

            Matrix first = sum(first_matrix_parts[0], first_matrix_parts[3], num_threads);
            Matrix second = sum(second_matrix_parts[0], second_matrix_parts[3], num_threads);
            threads[0] = std::thread(strassen, std::ref(first), std::ref(second), std::ref(m[0]), depth - 1, num_threads, false);

            first = sum(first_matrix_parts[2], first_matrix_parts[3], num_threads);
            threads[1] = std::thread(strassen, std::ref(first), std::ref(second_matrix_parts[0]), std::ref(m[1]), depth - 1, num_threads, false);

            second = sub(second_matrix_parts[1], second_matrix_parts[3], num_threads);
            threads[2] = std::thread(strassen, std::ref(first_matrix_parts[0]), std::ref(second), std::ref(m[2]), depth - 1, num_threads, false);

            second = sub(second_matrix_parts[2], second_matrix_parts[0], num_threads);
            threads[3] = std::thread(strassen, std::ref(first_matrix_parts[3]), std::ref(second), std::ref(m[3]), depth - 1, num_threads, false);

            first = sum(first_matrix_parts[0], first_matrix_parts[1], num_threads);
            threads[4] = std::thread(strassen, std::ref(first), std::ref(second_matrix_parts[3]), std::ref(m[4]), depth - 1, num_threads, false);

            first = sum(first_matrix_parts[2], first_matrix_parts[0], num_threads);
            second = sum(second_matrix_parts[0], second_matrix_parts[1], num_threads);
            threads[5] = std::thread(strassen, std::ref(first), std::ref(second), std::ref(m[5]), depth - 1, num_threads, false);

            first = sum(first_matrix_parts[1], first_matrix_parts[3], num_threads);
            second = sum(second_matrix_parts[2], second_matrix_parts[3], num_threads);
            threads[6] = std::thread(strassen, std::ref(first), std::ref(second), std::ref(m[6]), depth - 1, num_threads, false);

            for (auto &thread: threads)
                thread.join();
        }

        std::vector<Matrix> third_matrix = {Matrix(size, size), Matrix(size, size), Matrix(size, size), Matrix(size, size)};
        third_matrix[0] = sum(m[0], m[3], num_threads);
        third_matrix[0] = sub(third_matrix[0], m[4], num_threads);
        third_matrix[0] = sum(third_matrix[0], m[6], num_threads);
        third_matrix[1] = sum(m[2], m[4], num_threads);
        third_matrix[2] = sum(m[1], m[3], num_threads);
        third_matrix[3] = sub(m[0], m[1], num_threads);
        third_matrix[3] = sum(third_matrix[3], m[2], num_threads);
        third_matrix[3] = sum(third_matrix[3], m[5], num_threads);

        result_matrix.concatenation(third_matrix);
    }
}

Matrix strassenAlgorithm(Matrix &first_matrix, Matrix &second_matrix, int depth, int num_threads){
    Matrix result_matrix(first_matrix.getRows(), first_matrix.getRows());
    strassen(first_matrix, second_matrix, result_matrix, depth, num_threads, true);
    return result_matrix;
}

void Matrix::write_result() {
    std::ofstream file;
    file.open("Result.txt", std::ios_base::app);
    for (long int i = 0; i < this->rows; i++){
        for (long int j = 0; j < this->columns; j++)
            file << this->matrix[i * this->columns + j] << ' ';
        file << std::endl;
    }
    file << std::endl;
    file.close();
}

std::vector<int> Matrix::get_matrix() {
    return this->matrix;
}



